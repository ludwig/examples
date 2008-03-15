#include <Python.h>             /* Python as seen from C */
#include <numpy/arrayobject.h>  /* numpy as seen from C */
#include <math.h>
#include <stdio.h>              /* for debug output */
#include "numpy_macros.h"
#include "gridloop3.h"

PyObject* _pyfunc_ptr = NULL;   /* Python function to be called */

double _pycall(double x, double y)
{
    PyObject *arglist, *result;
    double C_result;
    
    if (_pyfunc_ptr == NULL)
    {
        printf("global pointer _pyfunc_ptr in %s is not initialized (NULL).",
               __FILE__);
        exit(1);
    }
    
    arglist = Py_BuildValue("(dd)", x, y);
    
    /*
     * the global variable _pyfunc_ptr points to the function provided
     * by the calling Python code
     */
    result = PyEval_CallObject(_pyfunc_ptr, arglist);
    Py_DECREF(arglist);
    if (result == NULL)
    {
        /* cannot return NULL... */
        printf("Error in callback...");
        exit(1);
    }

    C_result = PyFloat_AS_DOUBLE(result);
    Py_DECREF(result);

    return C_result;
}

static PyObject *gridloop1(PyObject *self, PyObject *args)
{
    PyArrayObject *a, *xcoor, *ycoor;
    PyObject *func1;
    int nx, ny, i;
    double **app;
    double *ap, *xp, *yp;

    /* arguments: a, xcoor, ycoor, func1 */
    /* parsing without checking the function types: */
    if (!PyArg_ParseTuple(args, "OOOO", &a, &xcoor, &ycoor, &func1))
    {
        return NULL;
    }

    NDIM_CHECK(a,2);
    TYPE_CHECK(a, PyArray_DOUBLE);
    nx = a->dimensions[0];
    ny = a->dimensions[1];

    NDIM_CHECK(xcoor, 1);
    DIM_CHECK(xcoor, 0, nx);
    TYPE_CHECK(xcoor, PyArray_DOUBLE);

    NDIM_CHECK(ycoor, 1);
    DIM_CHECK(ycoor, 0, ny);
    TYPE_CHECK(ycoor, PyArray_DOUBLE);

    CALLABLE_CHECK(func1);
    _pyfunc_ptr = func1; /* store func1 for use in _pycall */

    /* allocate help array for creating a double pointer: */
    app = (double **) malloc(nx * sizeof(double *));
    ap = (double *) a->data;

    for (i = 0; i < nx; i++)
    {
        app[i] = &(ap[i*ny]);
    }

    xp = (double *) xcoor->data;
    yp = (double *) ycoor->data;

    gridloop3(app, xp, yp, nx, ny, _pycall);

    free(app);
    return Py_BuildValue("");   /* return None */
}


static PyObject *gridloop2(PyObject *self, PyObject *args)
{
    PyArrayObject *a, *xcoor, *ycoor;
    int a_dims[2];
    PyObject *func1;
    int nx, ny, i;
    double **app;
    double *ap, *xp, *yp;

    /* arguments: xcoor, ycoor, func1 */
    if (!PyArg_ParseTuple(args, "OOO", &xcoor, &ycoor, &func1))
    {
        return NULL;
    }

    nx = xcoor->dimensions[0];
    ny = ycoor->dimensions[0];

    NDIM_CHECK(xcoor, 1);
    TYPE_CHECK(xcoor, PyArray_DOUBLE);

    NDIM_CHECK(ycoor, 1);
    TYPE_CHECK(ycoor, PyArray_DOUBLE);

    CALLABLE_CHECK(func1);
    _pyfunc_ptr = func1;

    /* create return array: */
    a_dims[0] = nx;
    a_dims[1] = ny;
    a = (PyArrayObject *)PyArray_FromDims(2, a_dims, PyArray_DOUBLE);
    if (a == NULL)
    {
        printf("creating %dx%d array failed\n", a_dims[0], a_dims[1]);
        return NULL; /* PyArray_FromDims raises an exception */
    }

    /* allocate help array for creating a double pointer: */
    app = (double **)malloc(nx * sizeof(double *));
    ap = (double *)a->data;
    
    for (i = 0; i < nx; i++)
    {
        app[i] = &(ap[i*ny]);
    }

    xp = (double *)xcoor->data;
    yp = (double *)ycoor->data;

    gridloop3(app, xp, yp, nx, ny, _pycall);

    free(app);
    return PyArray_Return(a);
}


/* doc strings: */
static char gridloop1_doc[] = "gridloop1(a, xcoor, ycoor, pyfunc)";
static char gridloop2_doc[] = "a = gridloop2(xcoor, ycoor, pyfunc)";
static char module_doc[] = \
    "module ext_gridloop:\n\
     gridloop1(a, xcoor, ycoor, pyfunc)\n\
     a = gridloop2(xcoor, ycoor, pyfunc)";

/*
 * The method table must always be present - it lists the
 * functions that should be callable from Python:
 */
static PyMethodDef ext_gridloop_methods[] = {
    {"gridloop1",       /* name of func when called from Python */
     gridloop1,         /* corresponding C function */
     METH_VARARGS,      /* ordinary (not keyword) arguments */
     gridloop1_doc},    /* doc string for gridloop1 function */

    {"gridloop2",       /* name of func when called from Python */
     gridloop2,         /* corresponding C function */
     METH_VARARGS,      /* ordinary (not keyword) arguments */
     gridloop2_doc},    /* doc string for gridloop2 function */

    {NULL, NULL}        /* required ending of the method table */
};

void initext_gridloop()
{
    /*
     * Assign the name of the module, the name of the
     * method table and (optionally) a module doc string:
     */
    Py_InitModule3("ext_gridloop", ext_gridloop_methods, module_doc);

    /* without module doc string:
    Py_InitModule("ext_gridloop", ext_gridloop_methods);
    // */

    import_array(); /* required numpy initialization */
}

