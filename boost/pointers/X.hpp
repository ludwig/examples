// X.hpp

class X
{
public:
    virtual void f() = 0;
    virtual void g() = 0;

protected:
    ~X() {}
};

shared_ptr<X> createX();

