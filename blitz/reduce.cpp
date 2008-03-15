/* From http://www.oonumerics.org/blitz/examples/Blitz++/reduce.cpp
 *
 * This example illustrates the array reduction functions provided by
 * Blitz++.  These functions reduce an N dimensional array (or array
 * expression) to an N-1 dimensional array expression by summing,
 * taking the mean, etc.  These array reductions are currently provided:
 * sum, mean, min, max, minIndex, maxIndex, product, count, any, all.
 */

#include <blitz/array.h>

using namespace blitz;

int main()
{
    Array<int,2> A(4,4);

    A = 3,  1,  2,  4,
        8, -1, -5,  3,
        0,  9, -1,  4,
        1,  3,  1,  2;

    cout << "A = " << A << endl;

    /*
     * Reduce the array A to a one-dimensional array,
     * by summing/taking the mean/etc. of each row.
     */

    Array<float,1> z(4);
    secondIndex j;

    z = sum(A,j);
    cout << "sum(A,j) = " << endl << z << endl << endl;

    z = mean(A,j);
    cout << "mean(A,j) = " << endl << z << endl << endl;

    z = min(A,j);
    cout << "min(A,j) = " << endl << z << endl << endl;

    z = minIndex(A,j);
    cout << "minIndex(A,j) = " << endl << z << endl << endl;

    z = max(A,j);
    cout << "max(A,j) = " << endl << z << endl << endl;

    z = maxIndex(A,j);
    cout << "maxIndex(A,j) = " << endl << z << endl << endl;

    z = product(A,j);
    cout << "product(A,j) = " << endl << z << endl << endl;

    z = count((A > 0), j);
    cout << "count((A > 0), j) = " << endl << z << endl << endl;

    z = any((abs(A) > 4), j);
    cout << "any((abs(A) > 4), j) = " << endl << z << endl << endl;

    z = all(A > 0, j);
    cout << "all(A > 0, j) = " << endl << z << endl << endl;

    return 0;
}

/* Output:

    A = 4 x 4
    [         3         1         2         4
              8        -1        -5         3
              0         9        -1         4
              1         3         1         2 ]

    sum(A,j) =
    4
     [        10         5        12         7  ]

    mean(A,j) =
    4
     [       2.5      1.25         3      1.75  ]

    min(A,j) =
    4
     [         1        -5        -1         1  ]

    minIndex(A,j) =
    4
     [         1         2         2         0  ]

    max(A,j) =
    4
     [         4         8         9         3  ]

    maxIndex(A,j) =
    4
     [         3         0         1         1  ]

    product(A,j) =
    4
     [        24       120         0         6  ]

    count((A > 0), j) =
    4
     [         4         2         2         4  ]

    any((abs(A) > 4), j) =
    4
     [         0         1         1         0  ]

    all(A > 0, j) =
    4
     [         1         0         0         1  ]

*/
