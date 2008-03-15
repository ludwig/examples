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
    //char *display_name;
    struct color_struct *color = malloc(sizeof(struct color_struct));

    Display *display;
    Window main_window;

    display = XOpenDisplay(0);
    assert(display);

    printf("Default display opened\n");
    printf("Size: %dx%d\n", XDisplayWidth(display, SCREEN),
                            XDisplayHeight(display, SCREEN));
    // Figure it out, or look it up
    

    color->black = XBlackPixel(display, SCREEN); // Black and white are the
    color->white = XWhitePixel(display, SCREEN); // only trivial colors
                                                 // in Xlib
    
    main_window = XCreateSimpleWindow(display, ROOT_WINDOW, 0, 0,
        200, 100, 0, color->black, color->black);
    // Create a window at (0,0) that is 200 x 100 and has a black background

    assert(main_window);                // Make sure it happened
    printf("Main window created\n");    // Hurrah

    XMapWindow(display, main_window);   // We have to 'map' the window
                                        // to make it visible

    XFlush(display);    // XFlush sends the commands
                        // to the server (Network model!)
    
    while(1);   // Stick around

    return 0;
}
