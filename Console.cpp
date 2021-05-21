#include "Console.h"

Console::Console(){
    blink = false;
    reverse = false;
    bold = false;
    update = true;
}

bool Console::getLine(string& line){
    //if(window == NULL || canvas == nullptr)
    //    return false;
    
    int c = wgetch(window);
    update = true;
    if(c == 8 || c == 127){
        if(buffer.size() > 0){
            buffer.erase(buffer.size()-1);
        }
    }else if(c == '\n' || c == '\r'){
        line = buffer;
        buffer = "";
        return true;
    }else if(iscntrl(c)){
        
    }else if(isprint(c)){
        buffer += c;
    }else{
        update = false;
    }
    line = buffer;
    return false;
}

bool Console::needsRedraw(){
    return update;
}

string Console::getBuffer(){
    return buffer;
}


int Console::getWidth(){
    int height,width;
    getmaxyx(window,height,width);
    return width;
}

int Console::getHeight(){
    int height,width;
    getmaxyx(window,height,width);
    return height;
}

void Console::clearScreen(){
    clear();
}

void Console::drawString(int x,int y,string str){
    mvwaddstr(window,y,x,str.c_str());
}

void Console::toggleAttribute(string attr){
    if(attr == "BLINK"){
        if(blink){
        attroff(A_BLINK);
        }else{
            attron(A_BLINK);
        }
        blink = !blink;
    }else if(attr == "BOLD"){
        if(bold){
            attroff(A_BOLD);
        }else{
            attron(A_BOLD);
        }
        bold = !bold;
    }else if(attr == "REVERSE"){
        if(reverse){
            attroff(A_REVERSE);
        }else{
            attron(A_REVERSE);
        }
        reverse = !reverse;
    }

}


void Console::initialize(){
    window = initscr();
    int height,width;
    getmaxyx(window,height,width);
    noecho();
    nodelay(window,true);
    refresh();
}

void Console::shutdown(){
    delwin(window);
    endwin();
    refresh();
}


