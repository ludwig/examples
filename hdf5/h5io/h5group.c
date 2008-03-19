#include "hdf5.h"
#include "h5io.h"
#include "split.h"
#include <string.h>

hid_t h5io_group_create(hid_t loc_id, const char *name)
{
    hid_t group_id;
    herr_t status;

    group_id = H5Gcreate(loc_id, name, 0);
    if (group_id < 0)
    {
        return -1;
    }

    status = h5io_attr_set_str(group_id, "TITLE", "");
    status = h5io_attr_set_str(group_id, "CLASS", "GROUP");
    status = h5io_attr_set_str(group_id, "VERSION", "1.0");
    status = h5io_attr_set_str(group_id, "PYTABLES_FORMAT_VERSION", "1.5");

    return group_id;
}


hid_t h5io_group_open(hid_t loc_id, const char *name)
{
    return H5Gopen(loc_id, name);
}


hid_t h5io_group_touch(hid_t loc_id, const char *name)
{
    hid_t group_id;

    group_id = H5Gopen(loc_id, name);

    if (group_id < 0)
    {
        group_id = h5io_group_create(loc_id, name);
        if (group_id < 0)
        {
            return -1;
        }
    }

    return group_id;
}


hid_t h5io_group_touch_path(hid_t loc_id, const char *path)
{
    hid_t group_id;

    /*
     * Temporarily disabling error messages:
     * see http://hdf.ncsa.uiuc.edu/HDF5/doc/Errors.html
     */
    herr_t (*old_func)(void *);
    void *old_client_data;

    /* Save old error handler */
    H5Eget_auto(&old_func, &old_client_data);

    /* Turn off error handling */
    H5Eset_auto(NULL, NULL);

    /* Attempt to open the group */
    group_id = H5Gopen(loc_id, path);

    if (group_id < 0)
    {
        herr_t status;
        hid_t parent_id;
        hid_t child_id;
        char **names;
        int i,n;

        split(path, strlen(path), &names, &n, '/');

        /* first parent */
        parent_id = h5io_group_touch(loc_id, names[0]);
        if (parent_id < 0)
        {
            H5Eset_auto(old_func, old_client_data);
            split_free(names, n);
            return -1;
        }

        for (i = 1; i < n; i++)
        {
            /* get child id */
            child_id = h5io_group_touch(parent_id, names[i]);
            if (child_id < 0)
            {
                H5Eset_auto(old_func, old_client_data);
                split_free(names, n);
                return -2;
            }
            /* move to next parent */
            status = H5Gclose(parent_id);
            parent_id = child_id;
        }

        /* return last group */
        group_id = parent_id;
    }

    /* Restore previous error handler */
    H5Eset_auto(old_func, old_client_data);

    return group_id;
}
