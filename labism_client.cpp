//rot gut ncurses starter code
//dymacek nov. 2015 v0.01
//dymacek apr. 2016 v0.02

//a small class to buffer an input line
#include "Console.h"
#include "Socket.h"
#include <vector>
#include <queue>

#include <iostream>
#include <string>

using namespace std;

deque<string> hist;
vector<Socket*> vec;


void draw(Console& console,string command){

    int width  = console.getWidth();
    int height = console.getHeight();

    console.clearScreen();

    int i = 0;
    for(string s : hist)
    {
        console.drawString(4, i , s);
        i++;
    }

    console.toggleAttribute("BLINK");
    console.drawString(0,height-1,"> ");
    console.toggleAttribute("BLINK");
    console.drawString(2,height-1,command);
}

int main(int argc,char** argv){
    if(argc < 3)
    {
        return 1;
    }

    Console console;
    //start up ncurses
    console.initialize();
    //draw the screen the first time
    draw(console,"");

    string line = "";

    bool running = true;
    string cmd = "";

    Socket serv;
    serv.open(argv[1], argv[2]);
    vec.push_back(&serv);

    while(running){

        waitForEvent(vec, 0);
        if(console.getLine(line)){
            //there was a new line
            if(line == "quit"){
                vec[0]->writeString(line);

                running = false;
            }

            else if(!line.empty())
            {
                vec[0]->writeString(line);
            }
        }

            waitForEvent(vec, 0);

            if(vec[0]->hasEvent)
            {
                hist.push_back(vec[0]->readString());
                if(hist.size()>console.getHeight() - 2)
                {
                    hist.pop_front();
                }
            }

        if(cmd != line or vec[0]->hasEvent){
            //if the buffer changed redraw
            cmd = line;
            draw(console,cmd);
        }
    }
    console.shutdown();
    return 0;
}
