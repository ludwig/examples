/* Argp example #1 -- a minimal program using argp */

/* This is probably the smallest possible program that
 * uses argp. It won't do much except give error messages
 * and exit when there are any arguments, and print
 * a (rather pointless) message for --help.
 */

#include <stdlib.h>
#include <argp.h>

int main(int argc, char *argv[])
{
    argp_parse(0, argc, argv, 0, 0, 0);
    exit(0);
}
