#include <stdio.h>
#include <boost/filesystem.hpp>

int main()
{
    boost::filesystem::path path1("/usr/local/include");    // random pathname
    boost::filesystem::path path2("/tmp/../usr/local/include");
    bool result = boost::filesystem::equivalent(path1, path2);
    printf("Paths are equivalent: %d\n", result);
    return 0;
}
