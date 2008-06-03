// X.cpp

class X_impl : public X
{
private:
    X_impl(X_impl const &);
    X_impl &operator=(const X_impl &);

public:
    virtual void f()
    {
        // ...
    }
    virtual void g()
    {
        // ...
    }
};

shared_ptr<X> createX()
{
    shared_ptr<X> px(new X_impl);
    return px;
}

