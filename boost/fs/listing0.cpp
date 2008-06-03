#include <sys/stat.h>
#include <stdio.h>

int main()
{
    struct stat s1;
    int status = stat(".", &s1);
    printf("Path is a directory: %d\n", S_ISDIR(s1.st_mode));
    return 0;
}
