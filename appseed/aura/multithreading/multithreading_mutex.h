#pragma once



class CLASS_DECL_AURA mutex :
   virtual public sync_object
{
public:


#if !defined(WINDOWS)


   pthread_mutex_t         m_mutex;
   
#ifdef MUTEX_COND_TIMED
   
   pthread_t               m_thread;
   pthread_cond_t          m_cond;
   int                     m_count;
   
#endif

#if defined(MUTEX_NAMED_POSIX)

   sem_t *                 m_psem; // as of 2016-11-26
                                   // not implemented (err=38) on android-19
#elif defined(MUTEX_NAMED_FD)

   int                     m_iFd;

#elif defined(MUTEX_NAMED_VSEM)

   key_t                   m_key;
   int32_t                 m_semid;

#endif


#endif


   bool                    m_bAlreadyExists;


   mutex(const mutex & m);
#ifdef WINDOWS
   mutex(::aura::application * papp,const char * pstrName,void * posdata, bool bOwner = true);
#elif defined(MUTEX_NAMED_POSIX)
   mutex(::aura::application * papp,const char * pstrName,sem_t * psem,bool bOwner = true);
#elif defined(MUTEX_NAMED_FD)
   mutex(::aura::application * papp, const char * pstrName, int iFd, bool bOwner = true);
#elif defined(MUTEX_NAMED_VSEM)
   mutex(::aura::application * papp,const char * pstrName,key_t key, int32_t semid, bool bOwner = true);
#endif
   mutex(::aura::application * papp = NULL,bool bInitiallyOwn = FALSE,const char * lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);



   virtual ~mutex();


#ifndef WINDOWS

   virtual bool lock();

   virtual bool lock(const duration & durationTimeout);

   virtual wait_result wait();

   virtual wait_result wait(const duration & durationTimeout);

#endif

   using sync_object::unlock;
   virtual bool unlock();


   bool already_exists();


   static mutex * open_mutex(::aura::application * papp, const char * pstrName);


};


CLASS_DECL_AURA void wait_until_mutex_does_not_exist(const char * pszName);



CLASS_DECL_AURA mutex * get_ui_destroyed_mutex();




#ifdef WINDOWSEX

class null_dacl_security_attributes
{
public:

   SECURITY_ATTRIBUTES m_securityattributes;

   SECURITY_DESCRIPTOR m_securitydescriptor;

   null_dacl_security_attributes();

};

#endif


#if defined(INSTALL_SUBSYSTEM)


namespace install
{



   class CLASS_DECL_AURA mutex :
#ifdef WINDOWSEX
      public null_dacl_security_attributes,
#endif
      public ::mutex
   {
   public:

      mutex(string strPlatform, string strSuffix = "");

   };


   class CLASS_DECL_AURA admin_mutex :
#ifdef WINDOWSEX
      public null_dacl_security_attributes,
#endif
      public ::mutex
   {
   public:

      admin_mutex(string strPlatform, string strSuffix = "");

   };




} // namespace install


#endif




