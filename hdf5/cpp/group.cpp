/*
 * This example creates a group in the file and a dataset in the group.
 * Hard link to the group object is created and the dataset is accessed
 * under different names.
 *
 * Iterator function is used to find the object names in the root group.
 * Note that the C++ API iterator function is not completed yet, thus
 * the C version is used in this example.
 */

#include <iostream>
#include <string>
#include "H5Cpp.h"

using namespace std;

const H5std_string filename("group.h5");
const int RANK = 2;


// operator function
extern "C" herr_t file_info(hid_t loc_id, const char *name, void *opdata)
{
    hid_t group;

    /* Open the group using its name. */
    group = H5Gopen(loc_id, name);

    /* Display group name. */
    cout << "Name: " << name << endl;

    H5Gclose(group);

    return 0;
}


int main(void)
{
    hsize_t dims[2];
    hsize_t cdims[2];

    // try-block to detect exceptions raised by any of the calls inside it
    try
    {
        /* Turn off auto-printing on failure */
        H5::Exception::dontPrint();

        /* Create file and group */
        H5::H5File *file = new H5::H5File(filename, H5F_ACC_TRUNC);
        H5::Group *group = new H5::Group(file->createGroup("/Data"));

        /* Create dataset "Compressed Data" in the group using absolute name.
         * Dataset creation property list is modified to use GZIP compression
         * with the compression effort set to 6. Note that the compression
         * can be used only when dataset is chunked.
         */
        dims[0] = 1000;
        dims[1] = 20;
        cdims[0] = 20;
        cdims[1] = 20;
        H5::DataSpace dataspace(RANK, dims);
        H5::DSetCreatPropList ds_creat_plist;
        ds_creat_plist.setChunk(2, cdims);
        ds_creat_plist.setDeflate(6);

        H5::DataSet *dataset = new H5::DataSet(file->createDataSet("/Data/Compressed_Data",
                                               H5::PredType::NATIVE_INT, dataspace, ds_creat_plist));
        delete dataset;
        delete group;
        delete file;


        /* Reopen file and group */
        file = new H5::H5File(filename, H5F_ACC_RDWR);
        group = new H5::Group(file->openGroup("Data"));
        try {
            dataset = new H5::DataSet(group->openDataSet("Compressed_Data"));
        } catch (H5::GroupIException not_found_error) {
            cout << " dataset is not found.\n";
        }
        cout << "dataset \"/Data/Compressed_Data\" is open\n";
        delete dataset;


        /* Create hard link to data group */
        file->link(H5G_LINK_HARD, "Data", "Data_new");
        try {
            dataset = new H5::DataSet(file->openDataSet("/Data_new/Compressed_Data"));
        } catch (H5::FileIException not_found_error) {
            cout << " dataset is not found.\n";
        }
        cout << "dataset \"/Data_new/Compressed_Data\" is open\n";
        delete dataset;

        /* Use iterator to see the names of the objects in the file root directory. */
        cout << "\nIterating over elements in the file\n";
        herr_t idx = H5Giterate(file->getId(), "/", NULL, file_info, NULL);
        cout << '\n';

        /* Unlink name "Data" and use iterator to see the names of the objects in the file root directory */
        cout << "Unlinking...\n";
        try {
            file->unlink("Data");
        } catch(H5::FileIException unlink_error) {
            cout << " unlink failed.\n";
        }
        cout << "\"Data\" is unlinked\n";
        cout << "\nIterating over elements in the file again\n";
        idx = H5Giterate(file->getId(), "/", NULL, file_info, NULL);

        delete file;
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
    catch (H5::AttributeIException error)
    {
        error.printError();
        return -1;
    }

    return 0;
}
