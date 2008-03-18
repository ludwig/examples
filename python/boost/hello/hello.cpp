#include <boost/python.hpp>
using namespace boost::python;

const char* greet();

BOOST_PYTHON_MODULE(hello)
{
    def("greet", greet);
}

const char* greet()
{
    return "hello, world";
}
