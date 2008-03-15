#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <X11/Xlib.h>

struct color_struct { unsigned long black, white; };

#define SCREEN      (DefaultScreen(display))
#define ROOT_WINDOW (DefaultRootWindow(display))

int main(void)
{
    //int depth;
    struct color_struct *color = malloc(sizeof(struct color_struct));

    Display *display;
    Window main_window, sub_window;
    GC main_gc;     // We need a graphics context for drawing stuff
    XEvent event;   // We need one of these so we can receive mouse events

    display = XOpenDisplay(0);

    color->black = XBlackPixel(display, SCREEN);
    color->white = XWhitePixel(display, SCREEN);
    
    main_window = XCreateSimpleWindow(display, ROOT_WINDOW,
        0, 0, 200, 100, 0, color->black, color->black);

    main_gc = XDefaultGC(display, 0); // Create the GC for drawing commands

    sub_window = XCreateSimpleWindow(display, main_window,
        10, 10, 50, 50, 0, color->black, color->white);
    // Yes, we are creating a new window inside the main one. This is
    // how widgets and so forth are made.

    XMapWindow(display, sub_window);

    XMapWindow(display, main_window);

    XFlush(display);
    
    // Map both windows, flush commands to the server

    XSelectInput(display, main_window, ButtonPressMask);
    XSelectInput(display, sub_window, ButtonPressMask);

    // This means we want the mouse click input for both windows

    XSetForeground(display, main_gc, color->white);

    // This is so we're drawing in white

    while (1)
    {
        XNextEvent(display, &event); // This gets the next event from the server

        // We look at the event's type, and then the internal event
        // which might be xkeyor xfocus or xmap or any other number
        // of things
        if (event.type == ButtonPress)
        {
            if (event.xbutton.window == main_window)
            {
                XDrawLine(display, main_window, main_gc, 70, 20, 150, 40);
                XFlush(display);
            }
            else if (event.xbutton.window == sub_window)
            {
                XCloseDisplay(display);
                exit(0);
            }
        }
    }
    return 0;
}
