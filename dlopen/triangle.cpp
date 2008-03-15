#include "polygon.hpp"
#include <cmath>

class triangle : public polygon
{
public:
    virtual double area() const
    {
        return _side_length * _side_length * sqrt(3) / 2;
    }
};

// the class factories

extern "C" polygon *create()
{
    return new triangle;
}

extern "C" void destroy(polygon *p)
{
    delete p;
}
