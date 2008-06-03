#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

using namespace std;

int main()
{
    boost::filesystem::path path1("/tmp/dir1/test1.c");
    boost::filesystem::path path2("/tmp/dir1/.test1.profile");
    string result1 = boost::filesystem::basename(path1);
    string result2 = boost::filesystem::basename(path2);
    cout << "Basename 1 : " << result1 << endl;
    cout << "Basename 2 : " << result2 << endl; 
    return 0;
}
