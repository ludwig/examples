#include <iostream>
#include <ginac/ginac.h>

using namespace std;
using namespace GiNaC;

ex HermitePoly(const symbol &x, int n)
{
    // 
    // uses the identity
    //
    //  H_n(x) = (-1)^n exp(x^2) (d/dx)^n exp(-x^2)
    //
    ex HKer = exp(-pow(x,2));
    return normal(pow(-1,n) * diff(HKer, x, n)/ HKer);
}

int main(void)
{
    symbol z("z");

    for (int i = 0; i < 6; i++)
    {
        cout << "H_" << i << "(z) = " << HermitePoly(z,i) << endl;
    }

    return 0;
}
