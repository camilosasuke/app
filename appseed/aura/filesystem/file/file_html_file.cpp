#include "framework.h"


namespace html
{


   file::file()
   {
      m_puistrcontext = NULL;
   }

   ::aura::str_context * file::str_context()
   {
      return m_puistrcontext;
   }



   void file::print(const string & str)
   {
      raw_print(str);
   }

   void file::raw_print(const string & str)
   {
      print(str);
   }

#ifndef VARIADIC_TEMPLATE_FORMAT

   void file::printf(string str,...)
   {
      UNREFERENCED_PARAMETER(str);
      ::exception::throw_interface_only(get_app());
   }

   void file::printstr(string str,...)
   {
      UNREFERENCED_PARAMETER(str);
      ::exception::throw_interface_only(get_app());
   }

#else

   void file::trace(void *,const char * psz)
   {
      print(psz);
   }

#endif


   void file::dprint(const char * lpcsz)
   {
      UNREFERENCED_PARAMETER(lpcsz);
      ::exception::throw_interface_only(get_app());
   }


} // namespace html


