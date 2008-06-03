#include <string>
#include <iostream>
#include "H5Cpp.h"

using namespace std;

const H5std_string filename("varlen.h5");
const H5std_string dset_vlstr_name("vlstr_type");
const int SPACE1_DIM1 = 4;
const int SPACE1_RANK = 1;

int main()
{
    const char *wdata[SPACE1_DIM1] = {
        "Four score and seven years ago our forefathers brought forth on this continent a new nation,",
        "conceived in liberty and dedicated to the proposition that all men are created equal.",
        "Now we are engaged in a great civil war,",
        "testing whether that nation or any nation so conceived and so dedicated can long endure."
    };  /* Information to write */

    char *rdata[SPACE1_DIM1];   /* Information read in */
    hid_t native_type;  /* Datatype id */
    hsize_t dims1[] = {SPACE1_DIM1};

    // try-block to detect exceptions raised by any of the calls inside it
    try
    {
        /* Turn off the auto-printing when failure occurs so that we can
         * handle the errors appropriately.
         */
        H5::Exception::dontPrint();

        /* Create a new file */
        H5::H5File file(filename, H5F_ACC_TRUNC);

        /* Create dataspace for datasets */
        H5::DataSpace sid1(SPACE1_RANK, dims1);

        /* Create a variable-length datatype */
        H5::StrType tid1(0, H5T_VARIABLE);

        /* Create a new dataset within the file */
        H5::DataSet dataset = file.createDataSet(dset_vlstr_name, tid1, sid1);

        /* Write the data to the dataset using default */
        dataset.write((void *)wdata, tid1);

        /* Close dataset */
        dataset.close();

        /* Re-open the dataset */
        dataset = file.openDataSet(dset_vlstr_name);

        /* Get datatype for dataset */
        H5::DataType dtype = dataset.getDataType();

        /* Read dataset from disk */
        dataset.read((void *)rdata, dtype);

        /* Print data read in */
        unsigned i;
        cout << "data read:" << endl;
        for (i = 0; i < SPACE1_DIM1; i++)
        {
            cout << wdata[i] << endl;
        }
        cout << endl;

        /* Free memory for rdata */
        for (i = 0; i < SPACE1_DIM1; i++)
        {
            free(rdata[i]); // XXX: free?
        }
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
