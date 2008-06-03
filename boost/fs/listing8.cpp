#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

int main()
{
    boost::filesystem::path path1("abc");
    boost::filesystem::path path2("def");

    try 
    {
        boost::filesystem::rename(path1, "def");

        std::cout << "Renamed " 
                  << path1.string()
                  << " to "
                  << path2.string()
                  << std::endl;
    } 
    catch (boost::filesystem::filesystem_error e)
    {
        std::cerr << "Could not rename file " << path1.string() << std::endl;
    }
    return 0;
}
