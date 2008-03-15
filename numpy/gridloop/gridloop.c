#include <Python.h>             /* Python as seen from C */
#include <numpy/arrayobject.h>  /* numpy as seen from C */


static PyObject *
gridloop1(PyObject *self, PyObject *args)
{
    PyArrayObject *a, *xcoor, *ycoor;
    PyObject *func1, *arglist, *result;
    int nx, ny, i, j;
    double *a_ij, *x_i, *y_j;

    /* arguments: a, xcoor, ycoor, func1 */
    if (!PyArg_ParseTuple(args, "O!O!O!O:gridloop1",
                &PyArray_Type, &a,
                &PyArray_Type, &xcoor,
                &PyArray_Type, &ycoor,
                &func1))
    {
        return NULL; /* PyArg_ParseTuple has raised an exception */
    }

    /* alternative parsing without checking the pointer types
    if (!PyArg_ParseTuple(args, "OOOO", &a, &xcoor, &ycoor, &func1))
    {
        return NULL;
    }
    // */

    if (a->nd != 2 || a->descr->type_num != PyArray_DOUBLE)
    {
        PyErr_Format(PyExc_ValueError,
                     "a array is %d-dimensional or not of type float",
                     a->nd);
        return NULL;
    }

    nx = a->dimensions[0];
    ny = a->dimensions[1];

    if (xcoor->nd != 1 || xcoor->descr->type_num != PyArray_DOUBLE ||
        xcoor->dimensions[0] != nx)
    {
        PyErr_Format(PyExc_ValueError,
                     "xcoor array has wrong dimension (%d), type or length (%d)",
                     xcoor->nd, xcoor->dimensions[0]);
        return NULL;
    }

    if (ycoor->nd != 1 || ycoor->descr->type_num != PyArray_DOUBLE ||
        ycoor->dimensions[0] != ny)
    {
        PyErr_Format(PyExc_ValueError,
                     "ycoor array has wrong dimension (%d), type or length (%d)",
                     ycoor->nd, ycoor->dimensions[0]);
        return NULL;
    }

    if (!PyCallable_Check(func1))
    {
        PyErr_Format(PyExc_TypeError, "func1 is not a callable function");
        return NULL;
    }

    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            a_ij = (double *)(a->data + i*a->strides[0] + j*a->strides[1]);
            x_i  = (double *)(xcoor->data + i*xcoor->strides[0]);
            y_j  = (double *)(ycoor->data + j*ycoor->strides[0]);
            // shorter: xcoor[i], ycoor[j] for one-dim arrays

            /* call Python function pointed to by func1: */
            arglist = Py_BuildValue("(dd)", *x_i, *y_j);
            result  = PyEval_CallObject(func1, arglist);
            Py_DECREF(arglist);
            if (result == NULL)
            {
                /* exception in func1 */
                return NULL;
            }

            *a_ij = PyFloat_AS_DOUBLE(result);

            Py_DECREF(result);

#ifdef DEBUG
            printf("a[%d,%d]=func1(%g,%g)=%g\n",i,j,*x_i,*y_j,*a_ij);
#endif
        }
    }
    return Py_BuildValue("");   /* return None */

    /* alternative (return 0 for success):
    return Py_BuildValue("i", 0);
    // */
}


#include "numpy_macros.h"

static PyObject *
gridloop2(PyObject *self, PyObject *args)
{
    PyArrayObject *a, *xcoor, *ycoor;
    int a_dims[2];
    PyObject *func1, *arglist, *result;
    int nx, ny, i, j;
    double *a_ij, *x_i, *y_j;

    /* arguments: xcoor, ycoor, func1 */
    if (!PyArg_ParseTuple(args, "O!O!O:gridloop2",
                &PyArray_Type, &xcoor,
                &PyArray_Type, &ycoor,
                &func1))
    {
        return NULL;    /* PyArg_ParseTuple has raised an exception */
    }

    nx = xcoor->dimensions[0];
    ny = ycoor->dimensions[0];
    
    NDIM_CHECK(xcoor,1); TYPE_CHECK(xcoor, PyArray_DOUBLE);
    NDIM_CHECK(ycoor,1); TYPE_CHECK(ycoor, PyArray_DOUBLE);
    CALLABLE_CHECK(func1);

    /* create return array */
    a_dims[0] = nx;
    a_dims[1] = ny;
    a = (PyArrayObject *)PyArray_FromDims(2, a_dims, PyArray_DOUBLE);
    if (a == NULL)
    {
        printf("Creating %dx%d array failed\n", a_dims[0], a_dims[1]);
        return NULL; /* PyArray_FromDims raised an exception */
    }

    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            arglist = Py_BuildValue("(dd)", IND1(xcoor,i), IND1(ycoor,j));
            result  = PyEval_CallObject(func1, arglist);
            Py_DECREF(arglist);
            if (result == NULL)
            {
                return NULL; /* exception in func1 */
            }

            IND2(a,i,j) = PyFloat_AS_DOUBLE(result);
            
            Py_DECREF(result);

#ifdef DEBUG
            printf("a[%d,%d]=func1(%g,%g)=%g\n", i, j,
                   IND1(xcoor,i), IND1(ycoor,j), IND2(a,i,j));
#endif
        }
    }

    return PyArray_Return(a);
}




/*
 * doc strings
 */

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
     gridloop2_doc},    /* doc string for gridloop1 function */

    {NULL, NULL}        /* required ending of the method table */
};

PyMODINIT_FUNC
initext_gridloop_c()
{
    /* 
     * Assign the name of the module, the name of
     * the method table, and optionally a module
     * doc string:
     */
    
    Py_InitModule3("ext_gridloop_c", ext_gridloop_methods, module_doc);

    /* without module doc string:
    Py_InitModule("ext_gridloop", ext_gridloop_methods);
    // */

    import_array(); /* required numpy initialization */
}
