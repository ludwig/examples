#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"
#include "../h5io.h"

int main(int argc, char *argv[])
{
    hid_t file_id;
    char *filename = "test_io.h5";

    /* groups */
    hid_t model_id;
    hid_t geometry_id;
    hid_t topology_id;
    hid_t solutions_id;

    hid_t snapshot0;
    hid_t snapshot1;
    hid_t snapshot2;

    hid_t foobarbaz0;
    hid_t foobarbaz1;
    hid_t foobarbaz2;


    /* error handling */
    herr_t status;

    /* open test file, creating it if necessary */
    file_id = h5io_file_open(filename, "rw+");
    if (file_id < 0)
    {
        fprintf(stderr, "Error: failed to open file '%s'\n", filename);
        return EXIT_FAILURE;
    }

    #define CHKERR(id) \
    {\
        fprintf(stderr, "Checking " #id "\n");\
        if ((id) < 0)\
        {\
            fprintf(stderr, "Failed to create " #id " (%d)\n", (id));\
            H5Fclose(file_id);\
            return EXIT_FAILURE;\
        }\
    }

    model_id = h5io_group_create(file_id, "/model"); CHKERR(model_id);
    geometry_id = h5io_group_create(model_id, "geometry"); CHKERR(geometry_id);
    topology_id = h5io_group_create(model_id, "topology"); CHKERR(topology_id);
    solutions_id = h5io_group_create(file_id, "/model/solutions"); CHKERR(solutions_id);

    snapshot0 = h5io_group_touch_path(solutions_id, "snapshot0"); CHKERR(snapshot0);
    snapshot1 = h5io_group_touch_path(model_id, "solutions/snapshot1"); CHKERR(snapshot1);
    snapshot2 = h5io_group_touch_path(file_id, "/model/solutions/snapshot2"); CHKERR(snapshot2);

    foobarbaz0 = h5io_group_touch_path(file_id, "/foo/bar/baz"); CHKERR(foobarbaz0);
    foobarbaz1 = h5io_group_touch_path(model_id, "foo/bar/baz"); CHKERR(foobarbaz1);
    foobarbaz2 = h5io_group_touch_path(model_id, "/geometry/foo/bar/baz"); CHKERR(foobarbaz2);

    /* touch an existing path */
    H5Gclose(foobarbaz2);
    foobarbaz2 = h5io_group_touch_path(model_id, "/geometry/foo/bar/baz"); CHKERR(foobarbaz2);

    /* clean up */
    H5Gclose(model_id);
    H5Gclose(geometry_id);
    H5Gclose(topology_id);
    H5Gclose(solutions_id);

    H5Gclose(snapshot0);
    H5Gclose(snapshot1);
    H5Gclose(snapshot2);

    H5Gclose(foobarbaz0);
    H5Gclose(foobarbaz1);
    H5Gclose(foobarbaz2);

    status = H5Fclose(file_id);

    return EXIT_SUCCESS;
}
