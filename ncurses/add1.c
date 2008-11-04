#include <ncurses.h>

int main(void)
{
    char text[] = "Greetings from NCurses!";
    char *t;

    initscr();      /* initialize ncurses */
    t = text;       /* initialize the pointer */

    while (*t)      /* loop through the whole string */
    {
        addch(*t);  /* put one char to curscr */
        t++;        /* increment the pointer */
        refresh();  /* update the screen */
        napms(100); /* delay a bit to see the display */
    }
    getch();        /* wait here */

    endwin();       /* cleanup ncurses */
    return 0;
}
