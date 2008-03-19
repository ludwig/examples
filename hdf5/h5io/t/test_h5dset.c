#include <stdio.h>
#include <stdlib.h>
#include "../h5io.h"

int main(int argc, char *argv[])
{
    char *filename = "test_io.h5";

    hid_t file_id;
    herr_t status;
    int ierr;

    /* Open the file */
    file_id = h5io_file_open(filename, "w");
    if (file_id < 0)
    {
        fprintf(stderr, "Could not access '%s'\n", filename);
        return -1;
    }

    /* Create a dataset */
    {
        int array[15] = { 1,  2,  3,  4,  5,
                          6,  7,  8,  8, 10,
                         11, 12, 13, 14, 15};

        ierr = h5io_dset_write2(file_id, "/myarray", "My integer array",
                                H5T_NATIVE_INT, array, 3, 5);

        if (ierr < 0)
        {
            fprintf(stderr, "Could not write /myarray\n");
            return -2;
        }
    }

    /* Open the file again and try to read the dataset we just wrote */
    {
        int m,n;
        int *myarray;
        hid_t file2;

        file2 = h5io_file_open(filename, "rw");
        if (file2 < 0)
        {
            fprintf(stderr, "Could not open '%s' for reading!\n", filename);
            return -3;
        }

        ierr = h5io_dset_read2(file2, "/myarray", H5T_NATIVE_INT,
                               (void **)&myarray, &m, &n);
        if (ierr < 0)
        {
            fprintf(stderr, "Could not read /myarray\n");
            return -4;
        }

        {
            int i,j;
            for (i = 0; i < m; i++)
            {
                for (j = 0; j < n; j++)
                {
                    printf("%2d\t", myarray[i*n + j]);
                }
                printf("\n");
            }
        }

        free(myarray);
        status = H5Fclose(file2);
    }

    /* Clean up */
    status = H5Fclose(file_id);

    return 0;
}
