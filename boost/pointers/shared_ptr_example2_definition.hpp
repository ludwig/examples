#include <boost/shared_ptr.hpp>

class example
{
public:
    example();
    void do_something();
private:
    class implementation;
    boost::shared_ptr<implementation> _imp; // hide implementation details
};
