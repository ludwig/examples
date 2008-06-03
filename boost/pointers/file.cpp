// file.cpp

#include "file.hpp"

class file::impl
{
private:
    impl(impl const &);
    impl &operator=(impl const &);

public:
    impl(char const *name, char const *mode)
    {
        // ...
    }

    ~impl()
    {
        // ...
    }

    void read(void *data, size_t size)
    {
        // ...
    }
};

file::file(char const *name, char const *mode) : pimpl(new imple(name, mode))
{
}

void file::read(void *data, size_t size)
{
    pimpl->read(data, size);
}
