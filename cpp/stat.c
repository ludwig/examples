#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct stat finfo;
    int rc;

    if (argc != 2)
    {
        return 1;
    }


    rc = stat(argv[1], &finfo);
    if (rc)
    {
        printf("file: %s does NOT exist!\n", argv[1]);
    }

    return 0;
}
