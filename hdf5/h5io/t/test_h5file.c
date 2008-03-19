#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"
#include "../h5io.h"

int main(int argc, char *argv[])
{
    hid_t file_id;
    herr_t status;

    char *filename = "test_io.h5";

    file_id = h5io_file_create(filename, "w");
    if (file_id < 0)
    {
        fprintf(stderr, "Error: failed to create file '%s'\n", filename);
        return EXIT_FAILURE;
    }
    printf("Created file '%s'\n", filename);

    status = H5Fclose(file_id);

    return EXIT_SUCCESS;
}
