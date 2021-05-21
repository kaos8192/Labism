#ifndef CONSOLE__H
#define CONSOLE__H
#include <unistd.h>
#include <curses.h>
#include <string>

using namespace std;


class Console{
public:
    Console();
    void initialize();
    void shutdown();
    
    bool getLine(string& line);
    
    void drawString(int x,int y,string str);
    void toggleAttribute(string attr);
    
    int getWidth();
    int getHeight();
    
    void clearScreen();
    
    bool needsRedraw();
    
    string getBuffer();

private:
    WINDOW* window;
    bool blink,reverse,bold,update;

    string buffer;
    
};

#endif
