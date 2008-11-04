#include <vtkConfigure.h>
#include <stdio.h>

#define MAJ 5
#define MIN 1
#define REL 0

int main(void)
{
    printf("VTK version is: %d.%d.%d\n", VTK_MAJOR_VERSION, VTK_MINOR_VERSION, VTK_BUILD_VERSION);
    #if VTK_MAJOR_VERSION < MAJ
    #  error Installed VTK is too old!
    #endif
    #if VTK_MINOR_VERSION < MIN
    #  error Installed VTK is too old!
    #endif
    #if VTK_BUILD_VERSION < REL
    #  error Installed VTK is too old!
    #endif
    return 0;
}

