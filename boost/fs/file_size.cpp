// http://www.boost.org/doc/libs/1_35_0/libs/filesystem/example/file_size.cpp
#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: file_size path\n";
        return 1;
    }

    std::cout << "sizeof(intmax_t) is " << sizeof(boost::intmax_t) << '\n';

    fs::path p(argv[1], fs::native);

    if (!fs::exists(p))
    {
        std::cout << "not found: " << argv[1] << std::endl;
        return 1;
    }

    if (!fs::is_regular(p))
    {
        std::cout << "not a regular file: " << argv[1] << std::endl;
        return 1;
    }

    std::cout << "size of " << argv[1] << " is " << fs::file_size(p) << std::endl;

    return 0;
}
