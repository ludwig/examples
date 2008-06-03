/* This example shows how to support custom options syntax.
 *
 * It's possible to install a 'custom_parser'. It will be invoked on
 * all command line tokens and can return name/value pair, or nothing.
 * If it returns nothing, usual processing will be done.
 */

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;

#include <iostream>
using namespace std;

/* This custom option parse function recognizes gcc-style
 * option "-fbar" / "-fno-bar".
 */
pair<string,string> reg_foo(const string& s)
{
    if (s.find("-f") == 0)
    {
        if (s.substr(2,3) == "no-")
            return make_pair(s.substr(5), string("false"));
        else
            return make_pair(s.substr(2), string("true"));
    }
    else
    {
        return make_pair(string(), string());
    }
}

int main(int ac, char *av[])
{
    try
    {
        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "produce a help message")
        ("foo", po::value<string>(), "just an option")
        ;

        po::variables_map vm;
        po::store(po::command_line_parser(ac,av).options(desc).extra_parser(reg_foo).run(), vm);
        if (vm.count("help"))
        {
            cout << desc;
            cout << "\nIn addition -ffoo and -fno-foo syntax are recognized.\n";
        }
        if (vm.count("foo"))
        {
            cout << "foo value with the value of "
                 << vm["foo"].as<string>()
                 << "\n";
        }
    }
    catch (exception& e)
    {
        cout << e.what() << "\n";
    }
}
