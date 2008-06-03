/*
 * This example writes a dataset to a new HDF5 file.
 */

#include <string>
#include <iostream>
#include "H5Cpp.h"

const H5std_string filename("sds.h5");
const H5std_string dataset_name("IntArray");
const int NX = 5;
const int NY = 6;
const int RANK = 2;

int main(void)
{
    /* 
     * Data initialization.
     */
    int i,j;
    int data[NX][NY];   // buffer for data
    for (i = 0; i < NX; i++)
    {
        for (j = 0; j < NY; j++)
        {
            data[i][j] = i + j;
        }
    }
    /*
     * 0 1 2 3 4 5
     * 1 2 3 4 5 6
     * 2 3 4 5 6 7
     * 3 4 5 6 7 8
     * 4 5 6 7 8 9
     */

    // try-block to detect exceptions raised
    // by any of the calls inside it
    try
    {
        /* Turn off auto-printing when failure occurs so that
         * we can handle the errors appropriately
         */
        H5::Exception::dontPrint();

        /* Create a new file using H5F_ACC_TRUNC access,
         * default file creation properties, and default file
         * access properties.
         */
        H5::H5File file(filename, H5F_ACC_TRUNC);

        /* Define the size of the array and create the dataspace
         * for a fixed size dataset.
         */
        hsize_t dimsf[2];   // dataset dimensions
        dimsf[0] = NX;
        dimsf[1] = NY;
        H5::DataSpace dataspace(RANK, dimsf);

        /* Define datatype for the data in the file.
         * We will store little endian INT numbers.
         */
        H5::IntType datatype(H5::PredType::NATIVE_INT);
        datatype.setOrder(H5T_ORDER_LE);

        /* Create a new dataset within the file using defined
         * dataspace and datatype, and default dataset creation
         * properties.
         */
        H5::DataSet dataset = file.createDataSet(dataset_name, datatype, dataspace);

        /* Write the data to the dataset using default memory space,
         * file space, and transfer properties.
         */
        dataset.write(data, H5::PredType::NATIVE_INT);
    }
    catch (H5::FileIException error)
    {
        // catch failure caused by the H5File operations
        error.printError();
        return -1;
    }
    catch (H5::DataSetIException error)
    {
        // catch failure caused by the DataSet operations
        error.printError();
        return -1;
    }
    catch (H5::DataSpaceIException error)
    {
        // catch failure caused by the DataSpace operations
        error.printError();
        return -1;
    }
    catch (H5::DataTypeIException error)
    {
        error.printError();
        return -1;
    }

    return 0; // successfully terminated
}
