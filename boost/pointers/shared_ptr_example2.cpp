#include "shared_ptr_example2_definition.hpp"

int main()
{
    example a;
    a.do_something();

    example b(a);
    b.do_something();

    example c;
    c = a;
    c.do_something();

    example d = c;
    d.do_something();

    return 0;
}
