#include "framework.h"


invalid_argument_exception::invalid_argument_exception(::ca::application * papp) :
   ca(papp),
   ::call_stack(papp),
   ::base_exception(papp),
   simple_exception(papp)
{

}


invalid_argument_exception::invalid_argument_exception(::ca::application * papp, const char * pszMessage) :
   ca(papp),
   ::call_stack(papp),
   ::base_exception(papp),
   simple_exception(papp, pszMessage)
{

}


invalid_argument_exception::~invalid_argument_exception()
{

}
