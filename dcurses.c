#include "dcurses.h"

/*TODO dev process
 *  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――
 *
 *  1: PREP
 *      1: learn how enum works
 *      
 *      2: learn how unistd.h works
 *
 *      3: learn how termios.h works
 *
 *  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――
 * 
 *  2: DEVELOPMENT
 *      1: Viewing
 *          1: work through colourations
 *              a: start with the 256 range
 *              b: handle reverse
 *              c: deal with the enum limited
 *              d: toggle text blink
 *
 *          2: work out fonts
 *
 *          3: reset
 *      
 *      2: Technicals
 *          1: Screen
 *              a: fullscreen toggle
 *              b: clear screen
 *
 *          2: Cursour
 *              a: show/hide
 *              b: move around screen
 *
 *          3: Information
 *              a: cursour location
 *              b: screen bounds
 *
 *  ナニ∴: During development, testing should be done frequently
 *
 *  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――
 *
 *  3: README
 *      ッ: This will be created after functions are properly defined and working 
 *
 *  ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― 
 *
 *  4: HANDIN
 *      ッ: Script for this is ┫ handin cmsc242 lab1 <final_directory_name_here> ┣
 *
 *  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――
 */

//Global var for termios
struct termios old_tstate, tstate_cur;
int isfull = 0;

//221a
//Enter full screen mode
//disable echo and buffered input
void dcurses_start_fullscreen()
{
    printf("\033[?1049h");
    int check = 0;

    check = tcgetattr(0, &old_tstate);
    if(check != 0)
    {
        return;
    }
    tstate_cur = old_tstate;
    tstate_cur.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tstate_cur.c_oflag &= ~OPOST;
    tstate_cur.c_lflag &= ~(ECHO | ECHONL | ICANON | ~ISIG | IEXTEN);
    tstate_cur.c_cflag &= ~(CSIZE | PARENB);
    tstate_cur.c_cflag |= CS8;
    tcsetattr(0, TCSANOW, &tstate_cur);

    isfull = 1;
}

//221a
//return to the previous mode
void dcurses_end_fullscreen()
{
    int check = 0;

    check = tcsetattr(0, TCSANOW, &old_tstate);
    if(check != 0)
    {
        return;
    }
    tstate_cur = old_tstate;
    printf("\033[?1049l");

    isfull = 0;
}

//221b
//clear the screen
void dcurses_clearScreen()
{
    printf("\033[2J");
    printf("\033[1;1H");
}

//212
//set the current font
void dcurses_setFont(DcursesFont font)
{
    if(font == 1)
    {
        printf("\033[3m");
    }
    if(font == 2)
    {
        printf("\033[1m");
    }
    if(font == 3)
    {
        printf("\033[2m");
    }
    if(font == 4)
    {
        printf("\033[4m");
    }
    if(font == 5)
    {
        printf("\033[9m");
    }
    else
    {
        printf("\033[10m");
    }
}

//211c
//Use enum
//Use mask
//set the foreground color to 3/4 bit color
void dcurses_fg(DcursesColor color)
{
    //Allie helped me with this
    int x = (color & 0x07) + 30;
    if(color & DCURSES_COLOR_BRIGHT)
    {
        x+=60;
    }
    printf("\033[%dm",x);
}

//211c
//Use enum
//Use mask
//set the background color to 3/4 bit color
void dcurses_bg(DcursesColor color)
{
    //Allie helped me with this
    int x = (color & 0x00ff) + 40;
    if(color & DCURSES_COLOR_BRIGHT)
    {
        x+=60;
    }
    printf("\033[%dm",x);
}

//211a
//Use \033[48;5;%dm
//set the background color to a pallet color 0-255
void dcurses_bg_pallet(int color)
{
    printf("\033[48;5;%dm", color);
}

//211a
//Use \033[38;5;%dm
//set the foreground color to a pallet color 0-255
void dcurses_fg_pallet(int color)
{
    printf("\033[38;5;%dm", color);
}

//222a
//enable/disable cursor
void dcurses_cursor(int val)
{
    if(val == 0)
    {
        printf("\033[?25l");
    }
    else
    {
        printf("\033[?25h");
    }
}

//211d
//enable/disable blink
void dcurses_blink(int val)
{
    if(val == 0)
    {
        printf("\033[25m");
    }
    else
    {
        printf("\033[5m");
    }
}

//211b
//enable/disable reverse
void dcurses_reverse(int val)
{
    if(val == 0)
    {
        printf("\033[27m");

    }
    else
    {
        printf("\033[7m");
    }
}

//213
//Use \033[0m
//reset attributes? just fonts and colors i believe
void dcurses_reset()
{
    printf("\033[0m");
}

//223b
//get the screen bounds
void dcurses_bounds(int* width,int* height)
{
    printf("\033[s");
    int check = 0;

    if(isfull == 0)
    {
        dcurses_start_fullscreen();
        isfull = 2;
    }

    dcurses_moveTo(999999,999999);

    dcurses_location(width, height);

    if(isfull == 2)
    {
        dcurses_end_fullscreen();
    }

    printf("\033[u");
}

//223a
//get the cursor location
void dcurses_location(int* width,int* height)
{
    int check = 0;
    if(isfull == 0)
    {
        dcurses_start_fullscreen();
        isfull = 2;
    }

    printf("\033[6n");
    scanf("\033[%d;%dH", height, width);

    if(isfull == 2)
    {
        dcurses_end_fullscreen();
    }
}

//222b
//move the cursor to the location
void dcurses_moveTo(int x,int y)
{
    printf("\033[%d;%dH", y, x);
}

