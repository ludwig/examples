#!/usr/bin/env python

def f1(x,y):
    print 'x+2*y =', x + 2*y
    return x + 2*y

def verify1():
    """Basic test of the extension module."""
    from numpy import allclose
    from langtangen import Grid2Deff

    g = Grid2Deff(dx=0.5, dy=1)
    f_exact = g(f1) # numpy computation

    f = g.ext_gridloop2(f1)
    print 'f computed by external gridloop2 function:\n', f
    if allclose(f, f_exact, atol=1.0e-10, rtol=1.0e-12):
        print 'f is correct'

if __name__ == '__main__':
    verify1()
