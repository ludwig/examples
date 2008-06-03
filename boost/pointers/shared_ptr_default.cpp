#include <boost/shared_ptr.hpp>
#include <iostream>
using namespace std;

int main()
{
    boost::shared_ptr<int> p;

    cout << "p.get() = " << p.get() << endl;
}
