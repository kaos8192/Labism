//Server Application for Lab-ISM (Lab Instant Socket Messenger)
//Socket+Thread messenging system aiming to allow for quick,
//intersystem communication between lab systems.
//Code by Geir Anderson with some ideas from Dr. Dymacek
//Help received from Alex, Austin, Justin, and Tejas
//
//                       
//
//================================================================
#include "Socket.h"
#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <mutex>
#include <sstream>
#include <algorithm>

using namespace std;

map<string, Socket> clients;
mutex mtx;

void broadcast(string msg)
{
    for(auto & mapview : clients)
    {
        mapview.second.writeString(msg);
    }
}

void pm(string sender, string dest, string msg)
{
    string compmsg;
    sender = sender.substr(0, sender.size()-1);

    compmsg += sender;

    compmsg += ": >~~~~>> @:";
    
    compmsg += dest;

    compmsg += '\n'; 
    compmsg += ">>>   ";
    
    compmsg += msg;

    for(auto & mapview : clients)
    {
        if((strcmp(mapview.first.c_str(), dest.c_str()) == 0)
                || (strcmp(mapview.first.c_str(), sender.c_str()) == 0))
        {
            mapview.second.writeString(compmsg);
        }
    }
}

void list_users(string &msg)
{
    for(auto & mapview : clients)
    {
        msg += mapview.first;
    }
}

void parser(string msg, string user, Socket current)
{
    mtx.lock();

    string who = "/who";
    string server = "1server";

    if(strcmp(msg.c_str(), who.c_str()) == 0)
    {
        string listy;
        list_users(listy);

        if(strcmp(user.c_str(), server.c_str()) == 0)
        {
            broadcast(listy);
        }
        else
        {
            current.writeString(listy);
        }
    }
    else if(msg[0] == '@')
    {
        string tomsg;
        string privatemsg;

        msg = msg.substr(1);

        size_t space = msg.find(' ');

        tomsg = msg.substr(0, space);
        privatemsg = msg.substr(space+1);

        pm(user, tomsg, privatemsg);
    }
    else
    {
        string msg2 = "";

        msg2 += user;

        msg2 = msg2.substr(0,msg2.size()-1);

        msg2 += " >~~~~>>";
        msg2 += "  ";

        msg2 += msg;

        broadcast(msg2);
    }
    mtx.unlock();
}

void rm_client(string usrnm)
{
    mtx.lock();

    auto ite = clients.find(usrnm);

    if(ite != clients.end())
    {
        clients.erase(usrnm);
    }

    mtx.unlock();
}

bool add_client(string usrnm, Socket sock)
{
    mtx.lock();

    bool passed = false;

    auto ite = clients.find(usrnm);

    if(ite == clients.end())
    {
        clients[usrnm] = sock;
        passed = true;
    }

    mtx.unlock();

    return passed;
}

void new_client(Socket client)
{
    string usrnm = client.readString();

    if(!add_client(usrnm, client))
    {
        client.close();
    }

    parser("/who", "1server", client);

    vector<Socket*> sockets;

    sockets.push_back(&client);

    while(true)
    {
        waitForEvent(sockets, -1);

        if(client.closed)
        {
            rm_client(usrnm);
        }
        else if(client.hasEvent)
        {
            string msg = client.readString();

            parser(msg, usrnm, client);
        }
    }
}

int main(int argc, char* argv[])
{
    Socket entry;

    if(argc == 1)
    {
        return 1;
    }

    entry.listen(argv[1]);

    while(true)
    {
        Socket new_mem = entry.accept();

        thread thrd (new_client, new_mem);

        thrd.detach();
    }

    return 0;
}
