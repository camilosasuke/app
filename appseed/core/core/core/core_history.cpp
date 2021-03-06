//#include "framework.h"

namespace core
{

   history::history()
   {
      m_phistory = NULL;
   }

   history::history(::aura::application * papp) :
      object(papp)
   {
      m_phistory = NULL;
   }


   bool history::hist(const char * psz)
   {
      if(m_phistory != NULL)
      {
         return m_phistory->hist(psz);
      }
      return false;
   }

} // namespace core


