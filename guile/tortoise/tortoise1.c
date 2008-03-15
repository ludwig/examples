/* http://www.gnu.org/software/guile/docs/guile-tut/tutorial.html */

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


void tortoise_reset()
{
    currentX = currentY = WINDOW_SIZE/2;
    currentDirection = 0.0;
    penDown = 1;
}

void tortoise_pendown()
{
    penDown = 1;
}

void tortoise_penup()
{
    penDown = 0;
}

void tortoise_turn(int degrees)
{
    currentDirection += (double)degrees;
}

void tortoise_move(int steps)
{
    double newX, newY;
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
    {
        int ii;
        tortoise_pendown();
        for (ii=0; ii<4; ii++)
        {
            tortoise_move(100);
            tortoise_turn(90.0);
        }
        /* sleep for a bit so the window stays visible */
        sleep(10);
    }
    return 0;
}
