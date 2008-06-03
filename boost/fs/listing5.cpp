#include <iostream>
#include <boost/filesystem.hpp>

int main()
{
    boost::filesystem::path path1("/usr/local/include");    // random pathname
    boost::filesystem::path::iterator it = path1.begin();
    while (it != path1.end())
    {
        std::cout << *it << std::endl;
        ++it;
    }
    return 0;
}
