// Define the SIP wrapper to the Hello library.

%Module hello 0

class Hello
{

%TypeHeaderCode
#include "hello.h"
%End

public:
    Hello(char* msg);
    void greet() const;

};
