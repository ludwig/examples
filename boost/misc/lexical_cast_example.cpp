#include <vector>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>


int main(int argc, char *argv[])
{
    using boost::lexical_cast;
    using boost::bad_lexical_cast;
    std::vector<int> args;

    char **p = argv;
    while (*++p)
    {
        try
        {
            args.push_back(lexical_cast<int>(*p));
        }
        catch (bad_lexical_cast &)
        {
            args.push_back(0);
        }
    }

    int i;
    std::vector<int>::iterator it;
    for (i = 1, it = args.begin(); it != args.end(); ++i, ++it)
    {
        std::cout << "argv[" << i << "] = \"" << argv[i] << "\"";
        std::cout << " --> " << (*it) << std::endl;
    }

    return 0;
}
