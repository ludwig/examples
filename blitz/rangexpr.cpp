#include <blitz/vector-et.h>
#include <blitz/vector.h>

using namespace blitz;

int main()
{
    Vector<float> x = cos(Range(0,7) * (2.0 * M_PI / 8));

    cout << x << endl;

    return 0;
}
