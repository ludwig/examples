import numpy
import ext_gridloop_c as ext_gridloop

from numpy import sin
from numpy import zeros, arange
from numpy import newaxis, size

def sequence(min=0.0, max=None, inc=1.0):
    """See Langtangen (pp. 150-151)"""
    return arange(min, max + inc/2.0, inc)
seq = sequence

class Grid2D(object):
    """From Langtangen's Python book (pg. 377)"""
    def __init__(self, xmin=0, xmax=1, dx=0.5,
                       ymin=0, ymax=1, dy=0.5):
        
        self.xcoor = seq(xmin, xmax, dx)
        self.ycoor = seq(ymin, ymax, dy)
        
        self.xcoorv = self.xcoor[:,newaxis]
        self.ycoorv = self.ycoor[newaxis,:]

    def __call__(self, f):
        """
        Treat f either as an expression containing x and y
        or as a standard Python function f(x,y). Evaluate
        the formula or function for all grid points and
        return the corresponding two-dimensional array.
        """
        if isinstance(f, basestring):
            # introduce the names x and y such that
            # a simple eval(f) will work for the arrays directly
            x = self.xcoorv
            y = self.ycoorv
            a = eval(f)
        else:
            a = f(self.xcoorv, self.ycoorv)
        return a

    def gridloop(self, f):
        f_is_str = isinstance(f, basestring)
        if f_is_str:
            f_compiled = compile(f, '<string>', 'eval')
        lx,ly = size(self.xcoor), size(self.ycoor)
        _a = zeros((lx,ly), float)
        for i in xrange(lx):
            x = self.xcoor[i]
            for j in xrange(ly):
                y = self.ycoor[j]
                if f_is_str:
                    _a[i,j] = eval(f_compiled)
                else:
                    _a[i,j] = f(x,y)
        return _a

class Grid2Deff(Grid2D):
    """Langtangen (pg. 433)"""


    def ext_gridloop1(self, f):
        """Compute a[i,j] = f(xi,yj) in an external routine."""
        
        # a is made here, sent to the routine, and then returned
        lx,ly = size(self.xcoor), size(self.ycoor)
        a = zeros((lx,ly), float)
        
        # <Fortran-specific initialization...>
        
        if isinstance(f, basestring):
            f_compiled = compile(f, '<string>', 'eval')
            def f_str(x,y):
                return eval(f_compiled)
            func = f_str
        else:
            func = f

        ext_gridloop.gridloop1(a, self.xcoor, self.ycoor, func)

        return a


    def ext_gridloop2(self, f):
        """Compute a[i,j] = f(xi,yj) in an external routine."""

        # a is made in the external routine
        if isinstance(f, basestring):
            f_compiled = compile(f, '<string>', 'eval')
            def f_str(x,y):
                return eval(f_compiled)
            func = f_str
        else:
            func = f

        a = ext_gridloop.gridloop2(self.xcoor, self.ycoor, func)

        return a


