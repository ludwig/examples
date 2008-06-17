#include "hello.h"

void Hello::HelloTest()
{
    //Helloworld:
    std::string strTemp = "Hello World\n";
    std::cout << strTemp;
    
    //Print out however many asterixes CFoo tells us to:
    Foo fooTest;
    int iTest = fooTest.GetTheNumber();
    
    for(int i = 0; i < iTest; i++)
    {
        std::cout << "*";
    }
    
    std::cout << "\n";
}
