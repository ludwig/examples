/* http://www.gnu.org/software/guile/docs/guile-tut/tutorial.html */
/* .tortoise file version of the program */
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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
    return SCM_UNSPECIFIED;
}

SCM tortoise_pendown()
{
    int prevValue = penDown;
    penDown = 1;
    return (prevValue ? SCM_BOOL_T : SCM_BOOL_F);
}

SCM tortoise_penup()
{
    int prevValue = penDown;
    penDown = 0;
    return (prevValue ? SCM_BOOL_T : SCM_BOOL_F);
}

SCM tortoise_turn(SCM s_degrees)
{
    int prevValue = (int)currentDirection;
    double degrees;
    SCM_ASSERT(SCM_NUMBERP(s_degrees), s_degrees, SCM_ARG1, "tortoise-turn");
    //degrees = SCM_NUM2DOUBLE(s_degrees);
    degrees = scm_to_double(s_degrees);
    currentDirection += degrees;
    return SCM_MAKINUM(prevValue);
}

SCM tortoise_move(SCM s_steps)
{
    double oldX = currentX;
    double oldY = currentY;
    double newX, newY;
    double steps;
    SCM_ASSERT(SCM_NUMBERP(s_steps), s_steps, SCM_ARG1, "tortoise-move");
    //steps = SCM_NUM2DOUBLE(s_steps);
    steps = scm_to_double(s_steps);
    /* first work out the new endpoint */
    newX = currentX + sin(currentDirection*DEGREES_TO_RADIANS) * steps;
    newY = currentY - cos(currentDirection*DEGREES_TO_RADIANS) * steps;
    /* if the pen is down, draw a line */
    if (penDown)
        XDrawLine(theDisplay, theWindow, theGC,
                  (int)currentX, (int)currentY,
                  (int)newX, (int)newY);
    /* in either case, move the tortoise */
    currentX = newX;
    currentY = newY;
    return gh_cons(gh_double2scm(oldX), gh_cons(gh_double2scm(oldY), SCM_EOL));
}

void register_procs(void)
{
    gh_new_procedure("tortoise-reset",   tortoise_reset,   0, 0, 0);
    gh_new_procedure("tortoise-pendown", tortoise_pendown, 0, 0, 0);
    gh_new_procedure("tortoise-penup",   tortoise_penup,   0, 0, 0);
    gh_new_procedure("tortoise-turn",    tortoise_turn,    1, 0, 0);
    gh_new_procedure("tortoise-move",    tortoise_move,    1, 0, 0);
}

#define CONFIGFILENAME             ".tortoise"
#define DIRECTORYSEPARATOR         "/"
void read_config_file(void)
{
    char *homedir;
    char *filename;
    FILE *file;
    char inputLine[1024];  /* hack: assume each line is less than 1024 chars */
    char *p;
    /* open $HOME/.tortoise */
    homedir = getenv("HOME");
    if (homedir == NULL)
        return;
    filename = (char *)malloc(strlen(homedir)
                              + strlen(CONFIGFILENAME)
                              + strlen(DIRECTORYSEPARATOR) + 1);
    if (filename == NULL)
        return;
    sprintf(filename, "%s%s%s", homedir, DIRECTORYSEPARATOR, CONFIGFILENAME);
    file = fopen(filename, "r");
    free(filename);
    if (file == NULL)
        return;
    /* spin through the file */
    while (1)
    {
        p = fgets(inputLine, sizeof(inputLine), file);
        if (p == NULL)
            break;
        gh_eval_str(p);
    }
    fclose(file);
}

void inner_main(int argc, char **argv)
{
    register_procs();
    read_config_file();
    /* now get on with the main business of the program ... */
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
