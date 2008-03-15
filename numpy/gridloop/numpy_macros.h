#ifndef __NUMPY_MACROS_H__
#define __NUMPY_MACROS_H__

/* 
 * This file defines some macros for programming with
 * numpy arrays in C extension modules.
 */

/* define some macros for array dimension/type check and subscripting */

#define QUOTE(s) # s    /* turn s into string "s" */

#define NDIM_CHECK(a, expected_ndim) \
    if (a->nd != expected_ndim) { \
        PyErr_Format(PyExc_ValueError, \
                     "%s array is %d-dimensional, but expected to be %d-dimensional",\
                     QUOTE(a), a->nd, expected_ndim); \
        return NULL; \
    }

#define DIM_CHECK(a, dim, expected_length) \
    if (dim > a->nd) { \
        PyErr_Format(PyExc_ValueError, \
                     "%s array has no %d dimension (max dim is %d)", \
                     QUOTE(a), dim, a->nd); \
        return NULL; \
    } \
    if (a->dimensions[dim] != expected_length) { \
        PyErr_Format(PyExc_ValueError, \
                     "%s array has wrong %d-dimension=%d (expected %d)", \
                     QUOTE(a), dim, a->dimensions[dim], expected_length); \
        return NULL; \
    }

#define TYPE_CHECK(a, tp) \
    if (a->descr->type_num != tp) { \
        PyErr_Format(PyExc_TypeError, \
                     "%s array is not of correct type (%d)", \
                     QUOTE(a), tp); \
        return NULL; \
    }

#define CALLABLE_CHECK(func) \
    if (!PyCallable_Check(func)) { \
        PyErr_Format(PyExc_TypeError, \
                     "%s is not a callable function", \
                     QUOTE(func)); \
        return NULL; \
    }


#define IND1(a,i)       *((double *)(a->data + i*a->strides[0]))
#define IND2(a,i,j)     *((double *)(a->data + i*a->strides[0] + j*a->strides[1]))
#define IND3(a,i,j,k)   *((double *)(a->data + i*a->strides[0] + j*a->strides[1] + k*a->strides[2]))


#endif
