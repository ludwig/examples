#include <stdio.h>
#include <boost/filesystem.hpp>

int main()
{
    boost::filesystem::path path("/usr/local/include"); // random pathname
    bool result = boost::filesystem::is_directory(path);
    printf("Path is a directory: %d\n", result);
    return 0;
}
