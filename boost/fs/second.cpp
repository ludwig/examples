#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main()
{
    fs::path p("second.cpp");
    
    if (!fs::exists(p))
    {
        std::cout << p.leaf() << " exists.\n";
        return -1;
    }

    if (fs::is_directory(p))
    {
        std::cout << p.leaf() << " is a directory.\n";
    }
    else if (fs::symbolic_link_exists(p))
    {
        std::cout << p.leaf() << " is a symbolic link.\n";
    }
    else
    {
        std::cout << p.leaf() << " is a regular file.\n";
    }

    std::cout << "file size: " << fs::file_size(p) << "\n";
    std::cout << "last modification time: " << fs::last_write_time(p);
    std::cout << std::endl;

    return 0;
}
