#include <iostream>
#include <ginac/ginac.h>

int main()
{
    GiNaC::symbol x("x");
    GiNaC::ex a = GiNaC::sin(x);

    std::cout << "Derivative of " << a << " is " << a.diff(x) << std::endl;

    return 0;
}
