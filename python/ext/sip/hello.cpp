#include <iostream>
#include <cstring>
#include "hello.h"

using namespace std;

Hello::Hello(char *msg)
{
    this->msg = new char[100];
    strncpy(this->msg, msg, 100);
}

Hello::~Hello()
{
    if (msg != 0)
    {
        delete [] msg;
        msg = 0;
    }
}

void Hello::greet() const
{
    cout << msg << endl;
}
