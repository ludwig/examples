#include <boost/noncopyable.hpp>
#include <iostream>

class DontTreadOnMe : private boost::noncopyable
{
public:
    DontTreadOnMe()
    {
        std::cout << "defanged!" << std::endl;
    }
};

int main(void)
{
    DontTreadOnMe object1;
    DontTreadOnMe object2(object1);
    object1 = object2;
    return 0;
}
