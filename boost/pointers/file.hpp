// file.hpp

class file
{
private:
    class impl;
    shared_ptr<impl> pimpl;

public:
    // compiler generated members are fine and useful

public:
    file(char const *name, char const *mode);
    void read(void *data, size_t size);
};

