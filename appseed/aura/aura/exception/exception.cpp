//#include "framework.h"


namespace exception
{


   exception::exception()
   {

      debug_print("log:exception");
      //::MessageBox(NULL,"abc123","abc123",MB_OK);

      m_bHandled = false;

      m_bContinue = true;

   }

   exception::exception(e_context_switcher_failed efail)
   {
      //debug_print("log:exception");
      //::MessageBox(NULL,"abc123","abc123",MB_OK);
      m_bLog = efail != failure_no_log;

      if(m_bLog)
      {
         debug_print("log:exception");
      }

      m_bHandled = false;

      m_bContinue = true;

   }


   exception::~exception()
   {
      if(m_bLog)
      {

         if(m_bContinue)
         {
            debug_print("-continue");
         }
         else
         {
            debug_print("-should_not_continue(fatal_exception_instance_candidate)");
         }

         if(m_bHandled)
         {
            debug_print("-explicitly_handled");
         }
         else
         {
            debug_print("-not_handled_explicitly");
         }

         debug_print("\n");

         }

   }

   void exception::Delete()
   {

      delete this;

   }

   void rethrow(exception * pe)
   {

      throw pe;

   }

   const char * exception::what() const throw()
   {

      return m_strMessage;

   }


   result::result(std::initializer_list < exception * > list)
   {

      forallref(list)
      {

         add(item);

      }

   }

} // namespace exception


#ifdef APPLE_IOS



#else

#ifdef WINDOWS
__declspec(thread) string * t_pstrNote = NULL;
__declspec(thread) BYTE t_pdataNote[sizeof(string)];
#else
__thread string * t_pstrNote = NULL;
__thread BYTE t_pdataNote[sizeof(string)];
#endif

string __get_thread_note()
{
   if(t_pstrNote == NULL)
      return "";
   return *t_pstrNote;
}
void __set_thread_note(const char * pszNote)
{
   if(t_pstrNote == NULL)
   {
#undef new
      t_pstrNote = ::new(t_pdataNote) string;
#define new AURA_NEW
   }
   *t_pstrNote = pszNote;
}

#endif






errno_t c_runtime_error_check(errno_t error)
{
   switch(error)
   {
      case ENOMEM:
         throw memory_exception(get_thread_app());
         break;
      case EINVAL:
      case ERANGE:
         throw invalid_argument_exception(get_thread_app());
         break;
#if defined(WINDOWS)
      case STRUNCATE:
#endif
      case 0:
         break;
      default:
         throw invalid_argument_exception(get_thread_app());
         break;
   }
   return error;
}

void __cdecl __clearerr_s(FILE *stream)
{
#ifdef WINDOWS
   C_RUNTIME_ERROR_CHECK(::clearerr_s(stream));
#else
   clearerr(stream);
   C_RUNTIME_ERROR_CHECK(errno);
#endif
}



namespace exception
{

   CLASS_DECL_AURA void throw_interface_only(::aura::application * papp)
   {
      throw interface_only_exception(papp);
   }

   CLASS_DECL_AURA void throw_not_implemented(::aura::application * papp)
   {
      throw not_implemented(papp);
   }

} // namespace exception















