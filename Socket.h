#ifndef SOCKET__H
#define SOCKET__H

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/poll.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>


using namespace std;

struct Socket{
	int fd;
	string host;
	string port;
	bool closed = false;
	bool hasEvent = false;

	void open(string host,string porto);
	void close();

	Socket accept();
	void listen(string porto);
    string toString();

	int readInt();
	void writeInt(int i);
    
    string readString();
    void writeString(string str);
};

void waitForEvent(vector<Socket*>& sockets,int timeout);



#endif
