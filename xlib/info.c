#include <stdio.h>      // Load stdio for printf
#include <assert.h>     // assert() is fluffy
#include <X11/Xlib.h>   // Load Xlib

int main(void)
{
    int depth;
    Display *display;
    char *display_string;

    // Open the default X display
    display = XOpenDisplay(0);
    assert(display);

    // Get the default depth of our display
    depth = DefaultDepth(display, DefaultScreen(display));

    display_string = DisplayString(display);

    printf("Default depth : %d\n", depth);
    printf("Display string: %s\n", display_string);

    return 0;
}
