#ifndef POLYGON_HPP
#define POLYGON_HPP

class polygon
{
protected:
    double _side_length;

public:

    polygon() : _side_length(0) {}

    virtual ~polygon() {}

    void set_side_length(double side_length)
    {
        _side_length = side_length;
    }

    virtual double area() const = 0;
};

// the types of the class factories
typedef polygon *create_t();
typedef void destroy_t(polygon *);

#endif
