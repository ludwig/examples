#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main()
{
    fs::path p("first.cpp");
    if (fs::exists(p))
    {
        std::cout << p.leaf() << " exists.\n";
    }
    else
    {
        std::cout << p.leaf() << " does not exist.\n";
    }
    return 0;
}
