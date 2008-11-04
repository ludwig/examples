#ifndef __HELLO_H__
#define __HELLO_H__

class Hello
{
public:
    Hello(char *msg);
    ~Hello();

    void greet() const;

private:
    char *msg;
};

#endif
