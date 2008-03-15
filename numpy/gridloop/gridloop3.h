#ifndef __GRIDLOOP3_H__
#define __GRIDLOOP3_H__

typedef double (*Fxy)(double x, double y);

void gridloop3(double **a, double *xcoor, double *ycoor,
               int nx, int ny, Fxy func1);

#endif
