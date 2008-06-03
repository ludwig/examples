// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1576.html
#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace fs = boost::filesystem;
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    fs::path p(argc <= 1 ? "." : argv[1]);
    
    if (!fs::exists(p)) // does not exist
    {
        cout << "Not found: " << argv[1] << endl;
    }
    else if (fs::is_directory(p))   // is a directory
    {
        for (fs::directory_iterator dir_iter(p);
             dir_iter != fs::directory_iterator();
             ++dir_iter)
        {
            // display only the rightmost name in the path
            cout << dir_iter->leaf() << endl;
        }
    }
    else // is a file
    {
        cout << "Found: " << argv[1] << endl;
    }

    return 0;
}
