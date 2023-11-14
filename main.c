/*
 * quick conways game of life
 * 
 * catroidvania thu nov 9 2023
*/

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

#define ALIVECHAR '@'
#define DEADCHAR '.'


int main(void)
{
    WINDOW *buffer;
    MEVENT mouse;
    int c;
    int width, height, mousex, mousey;
    int delay, prevtime, newtime;
    int run, paused, drawing;

    srand(time(NULL));

    /* init ncurses stuff */
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    mouseinterval(0);

    curs_set(0);

    getmaxyx(stdscr, height, width);

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    /* other window used to update game state */
    buffer = newwin(LINES, COLS, 0, 0);

    paused = drawing = run = 1;
    mousex = mousey = 0;

    delay = 1000;
    prevtime = newtime = timems();

    tick(buffer);

    while (run)
    {
        /* resize checking */
        if (width != COLS || height != LINES)
        {
            getmaxyx(stdscr, height, width);
            wresize(buffer, LINES, COLS);
            resizecopy();
        }


        /* user input */
        c = wgetch(stdscr);

        if (c == 'q')
        {
            break;
        }
        else if (c <= '9' && c >= '1')
        {
            delay = 100 * ((c - '1') + 1);
        }
        else if (c == KEY_MOUSE && getmouse(&mouse) == OK)
        {
            mousex = mouse.x;
            mousey = mouse.y;

            if (mouse.bstate & BUTTON1_PRESSED && mousey < height)
            {
                if (alivehere(mousex, mousey))
                {
                    addch(DEADCHAR); 
                }
                else
                {
                    addch(ALIVECHAR);
                }
            }
        }

        switch (c)
        {
            case ' ':
                paused = !paused;
                break;

            case '-':
            case '<':
                if (!paused && delay > 100) { delay -= 100; }
                break;

            case '+':
            case '>':
                if (!paused) { delay += 100; } 
                break;

            case '0':
                if (!paused) { delay = 1000; }
                break;

            case 'r':
                randomise();
                break;

            case 'c':
                clearstate();
                break;
        }


        /* update and draw */
        newtime = timems();

        if (!paused && (newtime - prevtime >= delay))
        {
            /* update */
            tick(buffer);

            prevtime = newtime;
        }
 
        /* status line */
        attron(A_REVERSE);

        mvhline(height-1, 0, ' ', width);

        if (paused)
        {
            printw("Speed: Paused");
        }
        else
        {
            printw("Speed: %d ms", delay);
        }
        attroff(A_REVERSE);

        /* update stdscr */
        refresh();
    }

    /* clean up stuff */
    delwin(buffer);
    endwin();

    return 0;
}


/* returns time elapsed in miliseconds */
static inline int timems(void)
{
    return (int)clock() / (CLOCKS_PER_SEC / 1000);
}


/* mvinch but we dont care about attributes */
static inline int alivehere(int x, int y)
{
    return (mvinch(y, x) & A_CHARTEXT) == ALIVECHAR;
}


/* count as surrounding alive tiles */
int countneighbors(int x, int y)
{
    int neighbors = 0;
    int n, e, s, w;

    n = y - 1 > -1;
    s = y + 1 < LINES;
    e = x + 1 <= COLS;
    w = x - 1 > -1;

    if (w && alivehere(x-1, y)) { neighbors++; }
    if (e && alivehere(x+1, y)) { neighbors++; }
    if (n && alivehere(x, y-1)) { neighbors++; }
    if (s && alivehere(x, y+1)) { neighbors++; }
    if (n && w && alivehere(x-1, y-1)) { neighbors++; }
    if (n && e && alivehere(x+1, y-1)) { neighbors++; }
    if (s && e && alivehere(x+1, y+1)) { neighbors++; }
    if (s && w && alivehere(x-1, y+1)) { neighbors++; }

    return neighbors;
}


/* update the board */
void tick(WINDOW *buf)
{
    int x, y, neighbors, living;

    for (x = 0; x < COLS; x++)
    {
        for (y = 0; y < LINES-1; y++)
        {
            mvwaddch(buf, y, x, DEADCHAR);

            neighbors = countneighbors(x, y);
            living = alivehere(x, y); 

            if (living)
            {
                if (neighbors < 2 || neighbors > 3)
                {
                    mvwaddch(buf, y, x, DEADCHAR);
                }
                else
                {
                    mvwaddch(buf, y, x, ALIVECHAR);
                }
            }
            else if (neighbors == 3)
            {
                mvwaddch(buf, y, x, ALIVECHAR);
            }
        }
    }

    overwrite(buf, stdscr);
}


/* randomise the contents of the screen */
void randomise(void)
{
    int x, y;
    int weight = rand() % 100;

    for (x = 0; x < COLS; x++)
    {
        for (y = 0; y < LINES-1; y++)
        {
            mvaddch(y, x, DEADCHAR);
            
            if ((rand() % 100) > weight) { mvaddch(y, x, ALIVECHAR); }
        }
    }
}


/* copy over the contents when we resize the window */
void resizecopy(void)
{
    int x, y;

    for (x = 0; x < COLS; x++)
    {
        for (y = 0; y < LINES-1; y++)
        {
            if (!alivehere(x, y)) { mvaddch(y, x, DEADCHAR); }
        }
    }
}

/* clear state */
void clearstate(void)
{
    int x, y;

    for (x = 0; x < COLS; x++)
    {
        for (y = 0; y < LINES-1; y++)
        {
            mvaddch(y, x, DEADCHAR);
        }
    }
}
