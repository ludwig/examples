#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
using namespace std;

int main(int argc, char *argv[])
{
    try
    {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("compression", po::value<int>(), "set compression level")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            cout << desc << "\n";
            return 1;
        }

        if (vm.count("compression"))
        {
            cout << "Compression level was set to "
                 << vm["compression"].as<int>() << ".\n";
        }
        else
        {
            cout << "Compression level was not set.\n";
        }
    }
    catch (exception& e)
    {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}
