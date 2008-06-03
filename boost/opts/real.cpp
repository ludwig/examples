#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
using namespace std;


/* Auxiliary functions for checking input for validity. */


/* Function used to check that 'opt1' and 'opt2' are
 * not specified at the same time.
 */
void conflicting_options(const po::variables_map& vm,
                         const char *opt1, const char *opt2)
{
    if (vm.count(opt1) && !vm[opt1].defaulted()
        && vm.count(opt2) && !vm[opt2].defaulted())
    {
        throw logic_error(string("Conflicting options '")
            + opt1 + "' and '" + opt2 + "'.");
    }
}


/* Function used to check that if 'for_what' is specified, then
 * 'required_option' is specified too.
 */
void option_dependency(const po::variables_map& vm,
                       const char *for_what, const char *required_option)
{
    if (vm.count(for_what) && !vm[for_what].defaulted())
    {
        if (vm.count(required_option) == 0 || vm[required_option].defaulted())
        {
            throw logic_error(string("Option '") + for_what
                + "' requires option '" + required_option + "'.");
        }
    }
}

int main(int argc, char *argv[])
{
    try
    {
        string ofile;
        string macrofile, libmakfile;
        bool t_given = false;
        bool b_given = false;
        string mainpackage;
        string depends = "deps_file";
        string sources = "src_file";
        string root = ".";

        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "print usage message")
        ("output,o", po::value(&ofile), "pathname for output")
        ("macrofile,m", po::value(&macrofile), "full pathname of macro.h")
        ("two,t", po::bool_switch(&t_given), "preprocess both header and body")
        ("body,b", po::bool_switch(&b_given), "preprocess body in the header context")
        ("libmakfile,l", po::value(&libmakfile), "write include makefile for library")
        ("mainpackage,p", po::value(&mainpackage), "output dependency information")
        ("depends,d", po::value(&depends), "write dependencies to <pathname>")
        ("sources,s", po::value(&sources), "write source package list to <pathname>")
        ("root,r", po::value(&root), "treat <dirname> as project root directory")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help"))
        {
            cout << desc << "\n";
            return 0;
        }

        conflicting_options(vm, "output", "two");
        conflicting_options(vm, "output", "body");
        conflicting_options(vm, "output", "mainpackage");
        conflicting_options(vm, "two", "mainpackage");
        conflicting_options(vm, "body", "mainpackage");

        conflicting_options(vm, "two", "body");
        conflicting_options(vm, "libmakfile", "mainpackage");
        conflicting_options(vm, "libmakfile", "mainpackage");

        option_dependency(vm, "depends", "mainpackage");
        option_dependency(vm, "sources", "mainpackage");
        option_dependency(vm, "root", "mainpackage");

        cout << "two = " << vm["two"].as<bool>() << "\n";
    }
    catch (exception& e)
    {
        cerr << e.what() << "\n";
    }

    return 0;
}
