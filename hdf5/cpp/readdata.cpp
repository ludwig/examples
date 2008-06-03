/*
 * This example reads hyperslab from the sds.h5 file into 
 * a two-dimensional plane of a three-dimensional array.
 * Various information about the dataset in the sds.h5 file
 * is obtained.
 */

#include <string>
#include <iostream>
#include "H5Cpp.h"

using namespace std;

const H5std_string filename("sds.h5");
const H5std_string dataset_name("IntArray");

const int NX_SUB = 3;   // hyperslab dimensions
const int NY_SUB = 4;

const int NX = 7;   // output buffer dimensions
const int NY = 7;
const int NZ = 3;

const int RANK_OUT = 3;


int main(void)
{
    /*
     * Output buffer initialization.
     */
    int i, j, k;
    int data_out[NX][NY][NZ];
    for (i = 0; i < NX; i++)
    {
        for (j = 0; j < NY; j++)
        {
            for (k = 0; k < NZ; k++)
            {
                data_out[i][j][k] = 0;
            }
        }
    }

    /*
     * try-block to detect exceptions raised by any of the
     * calls inside it.
     */
    try
    {
        /* Turn off the auto-printing when failure occurs so that
         * we can handle the errors appropriately.
         */
        H5::Exception::dontPrint();

        /* Open the specified file and the specified dataset in the file. */
        H5::H5File file(filename, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(dataset_name);

        /* Get the class of the datatype that is used by the dataset. */
        H5T_class_t type_class = dataset.getTypeClass();

        /* Get class of datatype and print message if it's an integer. */
        if (type_class == H5T_INTEGER)
        {
            cout << "Data set has INTEGER type" << endl;

            /* Get the integer datatype */
            H5::IntType inttype = dataset.getIntType();

            /* Get order of datatype and print message if it's little endian. */
            H5std_string order_string;
            H5T_order_t order = inttype.getOrder(order_string);
            cout << order_string << endl;

            /* Get size of the data element stored in file and print it */
            size_t size = inttype.getSize();
            cout << "Data size is " << size << endl;
        }

        /* Get dataspace of the dataset. */
        H5::DataSpace dataspace = dataset.getSpace();

        /* Get the number of dimensions in the dataspace. */
        int rank = dataspace.getSimpleExtentNdims();

        /* Get the dimension size of each dimension in the dataspace. */
        hsize_t dims_out[2];
        int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);
        cout << "rank " << rank << ", dimensions "
             << (unsigned long)(dims_out[0]) << " x "
             << (unsigned long)(dims_out[1]) << endl;

        /* Define hyperslab in the dataset; implicitly giving strike and block NULL. */
        hsize_t offset[2];  // hyperslab offset in the file
        hsize_t count[2];   // size of the hyperslab in the file
        offset[0] = 1;
        offset[1] = 2;
        count[0] = NX_SUB;
        count[1] = NY_SUB;
        dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        /* Define the memory dataspace. */
        hsize_t dimsm[3];   // memory space dimensions
        dimsm[0] = NX;
        dimsm[1] = NY;
        dimsm[2] = NZ;
        H5::DataSpace memspace(RANK_OUT, dimsm);

        /* Define memory hyperslab. */
        hsize_t offset_out[3];  // hyperslab offset in memory
        hsize_t count_out[3];   // size of the hyperslab in memory
        offset_out[0] = 3;
        offset_out[1] = 0;
        offset_out[2] = 0;
        count_out[0] = NX_SUB;
        count_out[1] = NY_SUB;
        count_out[2] = 1;
        memspace.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);

        /* Read data from hyperslab in the file into the hyperslab in memory. */
        dataset.read(data_out, H5::PredType::NATIVE_INT, memspace, dataspace);

        /* Finally, display a slice of the data */
        for (i = 0; i < NX; i++)
        {
            for (j = 0; j < NY; j++)
            {
                cout << data_out[i][j][0] << " ";
            }
            cout << endl;
        }
        /* 0 0 0 0 0 0 0
         * 0 0 0 0 0 0 0
         * 0 0 0 0 0 0 0
         * 3 4 5 6 0 0 0
         * 4 5 6 7 0 0 0
         * 5 6 7 8 0 0 0
         * 0 0 0 0 0 0 0
         */
    }
    catch (H5::FileIException error)
    {
        error.printError();
        return -1;
    }
    catch (H5::DataSetIException error)
    {
        error.printError();
        return -1;
    }
    catch (H5::DataSpaceIException error)
    {
        error.printError();
        return -1;
    }
    catch (H5::DataTypeIException error)
    {
        error.printError();
        return -1;
    }

    return 0;
}
