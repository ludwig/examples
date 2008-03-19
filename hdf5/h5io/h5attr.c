#include <stdlib.h>
#include <string.h>
#include "hdf5.h"
#include "h5io.h"

/****************************************************************************
 * Some of the following functions were based or taken from the H5ATTR.c    *
 * source file in PyTables, which is a BSD-licensed python extension        *
 * for accessing HDF5 files.                                                *
 *                                                                          *
 * The copyright notice is hereby retained.                                 *
 *                                                                          *
 * NCSA HDF                                                                 *
 * Scientific Data Technologies                                             *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 * Modified versions of H5LT for getting and setting attributes for open    *
 * groups and leaves.                                                       *
 * F. Altet 2005/09/29                                                      *
 *                                                                          *
 ****************************************************************************/


/* 
 * attr_find_op - operator function used by h5io_attr_find
 * 
 * (Pedro Vicente, pvn@ncsa.uiuc.edu - June 21, 2001)
 */
static herr_t attr_find_op(hid_t loc_id, const char *name, void *op_data)
{
    /* Define a default zero value for return. This will cause the
     * iterator to continue if the palette attribute is not found yet.
     */
    int ret = 0;

    char *attr_name = (char *)op_data;

    /* Shut the compiler up */
    loc_id = loc_id;

    /*
     * Define a positive value for return value if the attribute was
     * found. This will cause the iterator to immediately return that
     * positive value, indicating short-circuit success
     */

    if (strcmp(name, attr_name) == 0)
    {
        ret = 1;
    }

    return ret;
}



/* 
 * h5io_attr_find - Inquires if an attribute named attr_name exists attached
 * to the object loc_id. This function uses H5Aiterate with the operator
 * function attr_find_op.
 * 
 *   Success: The return value of the first operator that returns
 *            non-zero, or zero if all members were processed with no
 *            operator returning non-zero.
 *
 *   Failure: Negative if something goes wrong within the library,
 *            or the negative value returned by one of the operators.
 *
 * (Pedro Vicente, pvn@ncsa.uiuc.edu - June 21, 2001)
 */
herr_t h5io_attr_find(hid_t loc_id, const char *attr_name)
{
    unsigned int attr_num;
    herr_t ret;

    attr_num = 0;
    ret = H5Aiterate(loc_id, &attr_num, attr_find_op, (void *)attr_name);

    return ret;
}


/* h5io_attr_set - Sets an attribute named attr_name.
 * On success it returns 0, while on failure it returns -1.
 * (Pedro Vicente, pvn@ncsa.uiuc.edu - July 25, 2001)
 */
herr_t h5io_attr_set(hid_t obj_id, const char *attr_name, hid_t type_id, const void *data)
{
    hid_t space_id, attr_id;
    herr_t status;

    int has_attr;

    /* Create the data space for the attribute. */
    space_id = H5Screate(H5S_SCALAR);
    if (space_id < 0)
    {
        goto out;
    }

    /* Verify if the attribute already exists. */
    has_attr = h5io_attr_find(obj_id, attr_name);
    if (has_attr == 1)
    {
        /* The attribute already exists. Delete it. */
        status = H5Adelete(obj_id, attr_name);
        if (status < 0)
        {
            goto out;
        }
    }

    /* Create the attribute. */
    attr_id = H5Acreate(obj_id, attr_name, type_id, space_id, H5P_DEFAULT);
    if (attr_id < 0)
    {
        goto out;
    }

    /* Write the attribute data. */
    status = H5Awrite(attr_id, type_id, data);
    if (status < 0)
    {
        goto out;
    }

    /* Close the attribute. */
    status = H5Aclose(attr_id);
    if (status < 0)
    {
        goto out;
    }

    /* Close the data space. */
    status = H5Sclose(space_id);
    if (status < 0)
    {
        goto out;
    }

    return 0;

out:
    return -1;
}


/* 
 * h5io_attr_get - reads an attribute named attr_name with the memory
 * type type_id. On success it returns 0, while on failure it returns -1.
 *
 * (Pedro Vicente, pvn@ncsa.uiuc.edu - September 19, 2002)
 */
herr_t h5io_attr_get(hid_t obj_id, const char *attr_name, hid_t type_id, void *data)
{
    hid_t attr_id;
    herr_t status;

    attr_id = H5Aopen_name(obj_id, attr_name);
    if (attr_id < 0)
    {
        return -1;
    }

    status = H5Aread(attr_id, type_id, data);
    if (status < 0)
    {
        goto out;
    }

    status = H5Aclose(attr_id);
    if (status < 0)
    {
        return -1;
    }

    return 0;

out:
    H5Aclose(attr_id);
    return -1;
}



/* 
 * h5io_attr_get_dims - Gets the dimensionality of an attribute.
 * On success it returns 0, while on failure it returns -1.
 *
 * (Pedro Vicente, pvn@ncsa.uiuc.edu - September 4, 2001)
 */
herr_t h5io_attr_get_dims(hid_t obj_id, const char *attr_name, int *rank, hsize_t *dims)
{
    hid_t attr_id;
    hid_t space_id;
    herr_t status;

    /* Open the attribute */
    attr_id = H5Aopen_name(obj_id, attr_name);
    if (attr_id < 0)
    {
        return -1;
    }

    /* Get the dataspace handle */
    space_id = H5Aget_space(attr_id);
    if (space_id < 0)
    {
        goto out;
    }

    /* Get the number of dimensions */
    *rank = H5Sget_simple_extent_ndims(space_id);
    if (*rank < 0)
    {
        goto out;
    }

    /* Get dimensions */
    status = H5Sget_simple_extent_dims(space_id, dims, NULL);
    if (status < 0)
    {
        goto out;
    }

    /* Terminate access to the dataspace */
    status = H5Sclose(space_id);
    if (status < 0)
    {
        goto out;
    }

    /* End access to the attribute */
    status = H5Aclose(attr_id);
    if (status < 0)
    {
        goto out;
    }

    return 0;

out:
    H5Aclose(attr_id);
    return -1;
}

/* 
 * h5io_attr_set_str - Creates and writes a string attribute named attr_name
 * and attahes it to the object specified by obj_id. If the attribute already
 * exists, it is overwritten. On success it returns 0, while on failure
 * it returns -1.
 *
 * (Pedro Vicente, pvn@ncsa.uiuc.edu - July 23, 2001)
 */
herr_t h5io_attr_set_str(hid_t obj_id,
                         const char *attr_name,
                         const char *attr_data)
{
    hid_t attr_type;
    hid_t attr_size;
    hid_t attr_space_id;
    hid_t attr_id;
    int has_attr;
    herr_t status;

    /* Create the attribute */
    attr_type = H5Tcopy(H5T_C_S1);
    if (attr_type < 0)
    {
        goto out;
    }

    attr_size = strlen(attr_data) + 1; /* extra null term */

    status = H5Tset_size(attr_type, (size_t)attr_size);
    if (status < 0)
    {
        goto out;
    }

    status = H5Tset_strpad(attr_type, H5T_STR_NULLTERM);
    if (status < 0)
    {
        goto out;
    }

    attr_space_id = H5Screate(H5S_SCALAR);
    if (status < 0)
    {
        goto out;
    }

    /* Verify if the attribute already exists */
    has_attr = h5io_attr_find(obj_id, attr_name);

    /* If the attribute already exists, delete it */
    if (has_attr == 1)
    {
        status = H5Adelete(obj_id, attr_name);
        if (status < 0)
        {
            goto out;
        }
    }

    /* Create the attribute. */
    attr_id = H5Acreate(obj_id, attr_name, attr_type, attr_space_id,
                        H5P_DEFAULT);
    if (attr_id < 0)
    {
        goto out;
    }

    /* Write the attribute data. */
    status = H5Awrite(attr_id, attr_type, attr_data);
    if (status < 0)
    {
        goto out;
    }

    status = H5Aclose(attr_id);
    if (status < 0)
    {
        goto out;
    }

    status = H5Sclose(attr_space_id);
    if (status < 0)
    {
        goto out;
    }

    status = H5Tclose(attr_type);
    if (status < 0)
    {
        goto out;
    }

    return 0;

out:
    return -1;
}



/* 
 * h5io_attr_get_str - Reads a string attribute named attr_name.
 * On success it returns 0, while on failure it returns -1.
 * 
 * (Francesc Altet, faltet@carabos.com - February 23, 2005)
 */
herr_t h5io_attr_get_str(hid_t obj_id, const char *attr_name, char **data)
{
    hid_t attr_id;
    hid_t attr_type;
    size_t type_size;
    herr_t status;

    *data = NULL;

    attr_id = H5Aopen_name(obj_id, attr_name);
    if (attr_id < 0)
    {
        return -1;
    }

    attr_type = H5Aget_type(attr_id);
    if (attr_type < 0)
    {
        goto out;
    }

    /* Get the size */
    type_size = H5Tget_size(attr_type);
    if (type_size < 0)
    {
        goto out;
    }

    /* Malloc enough space for the string, plus 1 for the trailing '\0' */
    *data = (char *)malloc((type_size + 1) * sizeof(char));

    status = H5Aread(attr_id, attr_type, *data);
    if (status < 0)
    {
        goto out;
    }

    /* Set the last character to \0 in case we are dealing with
     * space padded strings.
     */
    (*data)[type_size] = '\0';

    /* TODO: check that following statement doesn't fail */
    status = H5Tclose(attr_type);
    if (status < 0)
    {
        goto out;
    }

    status = H5Aclose(attr_id);
    if (status < 0)
    {
        return -1;
    }

    return 0;

out:

    if (attr_type != 0)
        H5Tclose(attr_type);

    H5Aclose(attr_id);

    if (*data)
        free(*data);

    return -1;
}



/*
 * h5io_attr_set_array - write an array attribute. Returns 0 on success,
 * and -1 on failure. (July 25, 2001)
 */
herr_t h5io_attr_set_array(hid_t obj_id,
                           const char *attr_name,
                           size_t rank,
                           hsize_t *dims,
                           hid_t type_id,
                           const void *data)
{
    hid_t space_id, attr_id;
    herr_t status;

    int has_attr;

    /* Create the data space for the attribute. */
    space_id = H5Screate_simple(rank, dims, NULL);
    if (space_id < 0)
    {
        goto out;
    }

    /* Verify if the attribute already exists. */
    has_attr = h5io_attr_find(obj_id, attr_name);
    if (has_attr == 1)
    {
        /* The attribute already exists. Delete it. */
        status = H5Adelete(obj_id, attr_name);
        if (status < 0)
        {
            goto out;
        }
    }

    /* Create the attribute. */
    attr_id = H5Acreate(obj_id, attr_name, type_id, space_id, H5P_DEFAULT);
    if (attr_id < 0)
    {
        goto out;
    }

    /* Write the attribute data. */
    status = H5Awrite(attr_id, type_id, data);
    if (status < 0)
    {
        goto out;
    }

    /* Close the dataspace. */
    status = H5Sclose(space_id);
    if (status < 0)
    {
        goto out;
    }

    return 0;

out:
    return -1;
}

herr_t h5io_attr_set_array1(hid_t obj_id,
                            const char *attr_name,
                            hsize_t dim,
                            hid_t type_id,
                            const void *data)
{
    return h5io_attr_set_array(obj_id, attr_name, 1, &dim, type_id, data);
}

herr_t h5io_attr_set_array1_of_ints(hid_t obj_id,
                                    const char *attr_name,
                                    hsize_t dim,
                                    const int *data)
{
    return h5io_attr_set_array(obj_id, attr_name, 1, &dim, H5T_NATIVE_INT, data);
}

herr_t h5io_attr_set_array1_of_floats(hid_t obj_id,
                                      const char *attr_name,
                                      hsize_t dim,
                                      const float *data)
{
    return h5io_attr_set_array(obj_id, attr_name, 1, &dim, H5T_NATIVE_FLOAT, data);
}

herr_t h5io_attr_set_array1_of_doubles(hid_t obj_id,
                                       const char *attr_name,
                                       hsize_t dim,
                                       const double *data)
{
    return h5io_attr_set_array(obj_id, attr_name, 1, &dim, H5T_NATIVE_DOUBLE, data);
}


herr_t h5io_attr_set_int(hid_t obj_id, const char *attr_name, int n)
{
    return h5io_attr_set(obj_id, attr_name, H5T_NATIVE_INT, &n);
}

herr_t h5io_attr_set_long(hid_t obj_id, const char *attr_name, long n)
{
    return h5io_attr_set(obj_id, attr_name, H5T_NATIVE_LONG, &n);
}

herr_t h5io_attr_set_llong(hid_t obj_id, const char *attr_name, long long n)
{
    return h5io_attr_set(obj_id, attr_name, H5T_NATIVE_LLONG, &n);
}

herr_t h5io_attr_set_float(hid_t obj_id, const char *attr_name, float x)
{
    return h5io_attr_set(obj_id, attr_name, H5T_NATIVE_FLOAT, &x);
}

herr_t h5io_attr_set_double(hid_t obj_id, const char *attr_name, double x)
{
    return h5io_attr_set(obj_id, attr_name, H5T_NATIVE_DOUBLE, &x);
}
