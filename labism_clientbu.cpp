#include "Socket.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    Socket pls_no_spam;

    if(argc < 3)
    {
        return 1;
    }

    pls_no_spam.open(argv[1], argv[2]);

    while(true)
    {
        string outstr;
        getline(cin, outstr);
        pls_no_spam.writeString(outstr);
        string instr = pls_no_spam.readString();
    }

    return 0;
}
