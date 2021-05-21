LabISM server and client written by Geir Anderson
Socket library and Console originally written by Dr. Dymacek
Dcurses library written by Geir Anderson w/ credits given in the Dcurses README.txt
Help received from Alex, Austin, Tejas, and Justin

Compile

g++ labism_server.cpp Socket.cpp -o labismserv -lpthread

g++ labism_client.cpp Socket.cpp Console.cpp -o labismcli -lncurses

Running

Server:

./labismserv <port number w/o brackets here>

Client:

./labismcli <host url w/o brackets here> <port number w/o brackets here>

Client's first input is expected to be a username (!no protections in place at the moment!)

Public messages work
Listing all active users on login works
Multiple, unique users is working
/who command allows a user to see the list (to the requestor only)
Private messages are NOW working
