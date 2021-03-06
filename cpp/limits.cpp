// http://en.allexperts.com/q/C-1040/Floating-comparison.htm
//
#include <iostream> // for std::cout
#include <limits>   // for std::numeric_limits

using namespace std;

int main()
{
    cout << "Float format:"
         << "\n                radix: "
         << std::numeric_limits<float>::radix
         << "\n         radix digits: "
         << std::numeric_limits<float>::digits
         << "\n       decimal digits: "
         << std::numeric_limits<float>::digits10
         << "\n       radix exponent: "
         << std::numeric_limits<float>::min_exponent
         << " to "
         << std::numeric_limits<float>::max_exponent
         << "\n     decimal exponent: "
         << std::numeric_limits<float>::min_exponent10
         << " to "
         << std::numeric_limits<float>::max_exponent10
         << '\n'
         ;

    cout << "\nDouble format:"
         << "\n                radix: "
         << std::numeric_limits<double>::radix
         << "\n         radix digits: "
         << std::numeric_limits<double>::digits
         << "\n       decimal digits: "
         << std::numeric_limits<double>::digits10
         << "\n       radix exponent: "
         << std::numeric_limits<double>::min_exponent
         << " to "
         << std::numeric_limits<double>::max_exponent
         << "\n     decimal exponent: "
         << std::numeric_limits<double>::min_exponent10
         << " to "
         << std::numeric_limits<double>::max_exponent10
         << '\n'
         ;

   cout << "\nLong double format:"
         << "\n                radix: "
         << std::numeric_limits<long double>::radix
         << "\n         radix digits: "
         << std::numeric_limits<long double>::digits
         << "\n       decimal digits: "
         << std::numeric_limits<long double>::digits10
         << "\n       radix exponent: "
         << std::numeric_limits<long double>::min_exponent
         << " to "
         << std::numeric_limits<long double>::max_exponent
         << "\n     decimal exponent: "
         << std::numeric_limits<long double>::min_exponent10
         << " to "
         << std::numeric_limits<long double>::max_exponent10
         << '\n'
         ;

    return 0;
}
