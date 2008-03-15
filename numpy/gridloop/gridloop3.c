#include "gridloop3.h"

void gridloop3(double **a, double *xcoor, double *ycoor,
               int nx, int ny, Fxy func1)
{
    int i,j;
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            a[i][j] = func1(xcoor[i], ycoor[j]);
        }
    }
}

