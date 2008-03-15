/* http://www.gnu.org/software/guile/docs/guile-tut/tutorial.html */

#include <guile/gh.h>

#include <X11/Xlib.h>
#define WINDOW_SIZE 500

Display *theDisplay;
Window theWindow;
Screen *theScreen;
GC theGC;

double currentX;
double currentY;
double currentDirection;
int penDown;

#include <math.h>
#define DEGREES_TO_RADIANS (3.1415926535897932384626433832795029L/180.0)

SCM tortoise_reset()
{
    currentX = currentY = WINDOW_SIZE/2;
    currentDirection = 0.0;
    penDown = 1;
    return SCM_EOL;
}

SCM tortoise_pendown()
{
    penDown = 1;
    return SCM_EOL;
}

SCM tortoise_penup()
{
    penDown = 0;
    return SCM_EOL;
}

SCM tortoise_turn(SCM s_degrees)
{
    int degrees = SCM_INUM(s_degrees);
    currentDirection += (double)degrees;
    return SCM_EOL;
}

SCM tortoise_move(SCM s_steps)
{
    double newX, newY;
    int steps = SCM_INUM(s_steps);
    /* first work out the new endpoint */
    newX = currentX + sin(currentDirection*DEGREES_TO_RADIANS) * (double)steps;
    newY = currentY - cos(currentDirection*DEGREES_TO_RADIANS) * (double)steps;
    /* if the pen is down, draw a line */
    if (penDown)
        XDrawLine(theDisplay, theWindow, theGC,
                  (int)currentX, (int)currentY,
                  (int)newX, (int)newY);
    /* in either case, move the tortoise */
    currentX = newX;
    currentY = newY;
    return SCM_EOL;
}

void register_procs(void)
{
    gh_new_procedure("tortoise-reset",   tortoise_reset,   0, 0, 0);
    gh_new_procedure("tortoise-pendown", tortoise_pendown, 0, 0, 0);
    gh_new_procedure("tortoise-penup",   tortoise_penup,   0, 0, 0);
    gh_new_procedure("tortoise-turn",    tortoise_turn,    1, 0, 0);
    gh_new_procedure("tortoise-move",    tortoise_move,    1, 0, 0);
}

void inner_main(int argc, char **argv)
{
    register_procs();
    gh_repl(argc, argv);
}

int main(int argc, char *argv[])
{
    theDisplay = XOpenDisplay(NULL);
    XSynchronize(theDisplay, True);
    theScreen = DefaultScreenOfDisplay(theDisplay);
    theWindow = XCreateSimpleWindow(theDisplay, RootWindowOfScreen(theScreen),
                                    0, 0,
                                    WINDOW_SIZE, WINDOW_SIZE, 0,
                                    BlackPixelOfScreen(theScreen),
                                    WhitePixelOfScreen(theScreen));
    theGC = XCreateGC(theDisplay, theWindow, 0L, NULL);
    XSetForeground(theDisplay, theGC, BlackPixelOfScreen(theScreen));
    XMapWindow(theDisplay, theWindow);
    /* more stuff to come here... */
    tortoise_reset();
    gh_enter(argc, argv, inner_main);
    return 0; /* never reached */
}
