#include "examplelib/something.h"
#include "config.h"

namespace examplelib
{

Something::Something()
{
}

Something::~Something()
{
}


void Something::doSomething()
{
  m_Sub.doSomethingElse();
}

} //namespace