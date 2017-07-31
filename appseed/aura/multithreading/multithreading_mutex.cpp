#include "framework.h"


#if defined(LINUX) || defined(SOLARIS) || defined(APPLEOS)
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <pthread.h>
#elif defined(ANDROID)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#endif

static int g_iMutex = 0;


mutex::mutex(::aura::application * papp, bool bInitiallyOwn, const char * pstrName, LPSECURITY_ATTRIBUTES lpsaAttribute) :
   sync_object(pstrName)
{

   m_bOwner = true;

   m_bAlreadyExists = false;

#ifdef WINDOWS

   m_object = ::CreateMutexExW(lpsaAttribute,pstrName == NULL ? NULL : (const unichar *) ::str::international::utf8_to_unicode(pstrName),bInitiallyOwn ?  CREATE_MUTEX_INITIAL_OWNER : 0,MUTEX_ALL_ACCESS);

   DWORD dwLastError = ::GetLastError();

   m_bAlreadyExists = dwLastError == ERROR_ALREADY_EXISTS;

   if(m_object == NULL)
   {

      DWORD dwError1 = ::GetLastError();

      if(pstrName != NULL)
      {

         m_object = ::OpenMutexW(SYNCHRONIZE,FALSE,::str::international::utf8_to_unicode(pstrName));

      }

      if(m_object == NULL)
      {

         DWORD dwError2 = ::GetLastError();

         throw resource_exception(papp);

      }

   }

#else

#if defined(MUTEX_NAMED_POSIX)

   m_psem = SEM_FAILED;

   if(pstrName != NULL && *pstrName != '\0')
   {

      m_pmutex = NULL;

      m_psem = NULL;

      string strName(pstrName);

      strName.replace("/", "_");
      strName.replace(":", "_");
      strName.replace("/", "_");

      if(str::begins_ci(pstrName, "Global"))
      {

#ifdef ANDROID

         strName = ::file::path(::aura::system::g_p->m_pandroidinitdata->m_pszCacheDir) / "var/tmp" / strName;

#else

         strName = "/var/tmp" / strName;

#endif

      }
      else
      {

         strName = ::file::path(getenv("HOME")) / strName;

      }

      ::dir::mk(::file::path(strName).folder());

      ::file_put_contents_dup(strName, strName);

      string strTest = file_as_string_dup(strName);

      //int isCreator = 0;

      if ((m_psem = sem_open(strName, O_CREAT|O_EXCL, 0644, 1)) != SEM_FAILED)
      {

         // We got here first

         //isCreator = 1;

      }
      else
      {

         int err = errno;

         if (err != EEXIST)
         {

            throw resource_exception(get_app());

         }

         // We're not first.  Try again

         m_psem = sem_open(strName, 0);

         if (m_psem == SEM_FAILED)
         {

            throw resource_exception(get_app());;

         }

         m_pszName = strdup(strName);

      }

   }
   else

#elif defined(MUTEX_NAMED_FD)

   //m_count = 0;
   m_count = 0;

   if(pstrName != NULL && *pstrName != '\0')
   {

      m_pmutex = NULL;

      SetLastError(0);

      ::file::path path;

      if(str::begins_ci(pstrName, "Global"))
      {

#ifdef ANDROID

         path = ::file::path(::aura::system::g_p->m_pandroidinitdata->m_pszCacheDir) / "var/tmp/ca2/lock/mutex" / string(pstrName);

#else

         path = ::file::path("/var/tmp/ca2/lock/mutex") / pstrName;

#endif

      }
      else
      {

#ifdef ANDROID

         path = ::file::path(::aura::system::g_p->m_pandroidinitdata->m_pszCacheDir) / "home/user/ca2/lock/mutex" / string(pstrName);

#else

         path = getenv("HOME");

         path /= "Library/ca2/lock/mutex/named";

#endif


      }

      path /= pstrName;

      dir::mk(path.folder());

      m_iFd = open(path, O_RDWR | O_CREAT, S_IRWXU);

      if(m_iFd < 0)
      {

         int iErr = errno;

         const char * pszError = strerror(iErr);

         throw resource_exception(get_app());

      }

      m_pszName = strdup(path);

      pthread_mutexattr_t attr;

      pthread_mutexattr_init(&attr);

      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

      pthread_mutex_init(&m_mutex, &attr);

   }
   else

#elif defined(MUTEX_NAMED_POSIX)

   m_key = 0;

   m_semid = -1;

   if(pstrName != NULL && *pstrName != '\0')
   {

      m_pmutex = NULL;

      ::file::path path;

      if(str::begins_ci(pstrName, "Global"))
      {

         path = "/var/lock/ca2/mutex/named";

      }
      else
      {

         path = getenv("HOME");

         path /= ".config/ca2/lock/mutex/named";

      }

      path /= pstrName;

      ::dir::mk(path.folder());

      ::file_put_contents_dup(path, m_pszName);

      m_key = ftok(path, 1); //Generate a unique key or supply a value

      bool bAlreadyExists;

      get_existing:

      SetLastError(0);

      m_semid = semget(
                  m_key, // a unique identifier to identify semaphore set
                  1,  // number of semaphore in the semaphore set
                  0// permissions (rwxrwxrwx) on the new
                       //semaphore set and creation flag
                  );

      if(m_semid >= 0)
      {

         m_bOwner = false;

         bAlreadyExists = true;

         if(bAlreadyExists)
         {
                  SetLastError(ERROR_ALREADY_EXISTS);

         }

      }
      else
      {

         bAlreadyExists = false;

         m_semid = semget(
                     m_key, // a unique identifier to identify semaphore set
                     1,  // number of semaphore in the semaphore set
                     0777 | IPC_CREAT | IPC_EXCL// permissions (rwxrwxrwx) on the new
                          //semaphore set and creation flag
                     );

         if(m_semid == -1 && errno == EEXIST)
         {

            goto get_existing;

         }

      }

      if(m_semid < 0)
      {

         throw resource_exception(get_app());

      }

      m_pszName = strdup(path);

      semun semctl_arg;

      //set Initial value for the resource
      semctl_arg.val = 1; //Setting semval to 1

      semctl(m_semid, 0, SETVAL, semctl_arg);

   }
   else

#endif

   {

      pthread_mutexattr_t attr;

      pthread_mutexattr_init(&attr);

      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

      pthread_mutex_init(&m_mutex, &attr);

#ifdef MUTEX_COND_TIMED

      pthread_cond_init(&m_cond, NULL);

      m_count = 0;

      m_thread = 0;

#endif

   }

#endif

}


#ifdef WINDOWS

mutex::mutex(::aura::application * papp, const char * pstrName, void * h, bool bOwner) :
   object(papp),
   sync_object(pstrName)
{
   m_bOwner = bOwner;
   m_object = h;

}

mutex::mutex(const mutex & m):
object(m.get_app()),
sync_object(m.m_pszName)
{
   m_bOwner = false;
   m_object = m.m_object;

}

#else

#if defined(MUTEX_NAMED_POSIX)

mutex::mutex(::aura::application * papp, const char * pstrName, sem_t * psem, bool bOwner) :
   object(papp),
   sync_object(pstrName)
{

   m_bOwner       = bOwner;
   m_pszName      = strdup(pstrName);

#ifdef ANDROID

   m_psem         = psem;

#endif

}

mutex::mutex(const mutex & m):
   object(m.get_app()),
   sync_object(m.m_pszName)
{

   m_bOwner       = false;

#ifdef ANDROID

   m_psem         = m.m_psem;

#endif

   m_mutex        = m.m_mutex;

}

#elif defined(MUTEX_NAMED_FD)


mutex::mutex(::aura::application * papp, const char * pstrName, int iFd, bool bOwner) :
   object(papp),
   sync_object(pstrName)
{

   m_bOwner = bOwner;
   m_pszName = strdup(pstrName);
   m_iFd = iFd;

}

mutex::mutex(const mutex & m) :
   object(m.get_app()),
   sync_object(m.m_pszName)
{

   m_bOwner = false;
   m_iFd = m.m_iFd;
   m_mutex = m.m_mutex;

}



#elif defined(MUTEX_NAMED_VSEM)

mutex::mutex(::aura::application * papp,const char * pstrName,key_t key,int32_t semid,bool bOwner):
   object(papp),
   sync_object(pstrName)
{

   m_bOwner       = bOwner;
   m_key          = key;
   m_semid        = semid;

}


mutex::mutex(const mutex & m):
   object(m.get_app()),
   sync_object(m.m_pszName)
{

   m_bOwner       = false;
   m_semid        = m.m_semid;
   m_mutex        = m.m_mutex;

}

#endif

#endif



mutex::~mutex()
{

#ifndef WINDOWS

#if defined(MUTEX_NAMED_POSIX)

   if (m_psem != SEM_FAILED)
   {

      //if(m_bOwner)
      {
         sem_close(m_psem);
         sem_unlink(m_pszName);

      }

   }

#elif defined(MUTEX_NAMED_FD)

   if(m_pszName != NULL)
   {

      if(m_iFd >= 0)
      {

         close(m_iFd);

      }

      pthread_mutex_destroy(&m_mutex);

   }

#elif defined(MUTEX_NAMED_VSEM)

   if(m_semid >= 0)
   {

      if(m_bOwner)
      {

         semun ignored_argument;

         semctl(m_semid, 1, IPC_RMID , ignored_argument);

      }

   }

#endif

   else
   {

      pthread_mutex_destroy(&m_mutex);

#ifdef MUTEX_COND_TIMED

      pthread_cond_destroy(&m_cond);

#endif

   }

#endif

}


bool mutex::already_exists()
{

   return m_bAlreadyExists;

}

//      ANDROID
//      int irc = pthread_mutex_lock_timeout_np(&m_mutex, duration.get_total_milliseconds());
//
//      if (!irc)
//      {
//
//         return wait_result(wait_result::Event0);
//
//      }
//      else if (irc != EBUSY)
//      {
//
//         return wait_result(wait_result::Failure);
//
//      }
//


#if !defined(WINDOWS)

wait_result mutex::wait(const duration & duration)
{

   if(duration.is_pos_infinity())
   {

      return wait();

   }

#if defined(MUTEX_NAMED_POSIX)

   if (m_psem != SEM_FAILED)
   {

      timespec delay;

      delay.tv_sec = duration.m_iSeconds;

      delay.tv_nsec = duration.m_iNanoseconds;

      int32_t ret = sem_timedwait(m_psem, &delay);

      if(ret == 0)
      {

         return wait_result(wait_result::Event0);

      }
      else
      {

         if (errno == ETIMEDOUT)
         {

            return wait_result(wait_result::Timeout);

         }

      }

      return wait_result(wait_result::Failure);


   }
   else

#elif defined(MUTEX_NAMED_FD)

   if (m_pszName != NULL)
   {

      int rc = pthread_mutex_lock(&m_mutex);

      if (rc < 0)
      {

         return wait_result(wait_result::Failure);

      }

      uint32_t uiTimeout = (uint32_t)MIN(0xffffffffULL, duration.get_total_milliseconds());

      uint32_t uiStart = get_tick_count();

      while (true)
      {

         if (m_count > 0)
         {

            if (pthread_equal(m_thread, pthread_self()))
            {

               m_count++;

               int iError = pthread_mutex_unlock(&m_mutex);

               ASSERT(iError == 0);

               return wait_result(wait_result::Event0);

            }

         }
         else
         {

            int rc = lockf(m_iFd, F_LOCK, 0);

            if (rc == 0)
            {

               m_count++;

               m_thread = pthread_self();

               int iError = pthread_mutex_unlock(&m_mutex);

               ASSERT(iError == 0);

               return wait_result(wait_result::Event0);

            }
            else
            {

               rc = errno;

               if (rc != EAGAIN && rc != EACCES)
               {

                  int iError = pthread_mutex_unlock(&m_mutex);

                  ASSERT(iError == 0);

                  return wait_result(wait_result::Failure);

               }

            }

            int iError = pthread_mutex_unlock(&m_mutex);

            if (iError < 0)
            {

               ASSERT(FALSE);

               return wait_result(wait_result::Failure);

            }

            uint32_t uiElapsed = get_tick_count() - uiStart;

            if (uiElapsed >= uiTimeout)
            {

               return wait_result(wait_result::Timeout);

            }

            Sleep(MAX(1, MIN(1000, (uiTimeout - uiElapsed) / 50)));

            rc = pthread_mutex_lock(&m_mutex);

            if (rc < 0)
            {

               return wait_result(wait_result::Failure);

            }

         }

      }

      int iError = pthread_mutex_unlock(&m_mutex);

      ASSERT(iError == 0);

      return wait_result(wait_result::Failure);

   }
   else

#elif defined(MUTEX_NAMED_VSEM)

   if(m_semid >= 0)
   {

      timespec timeout;

      timeout.tv_sec = duration.m_iSeconds;

      timeout.tv_nsec = duration.m_iNanoseconds;

      struct sembuf operation[1] ;

      operation[0].sem_op = -1; // Wait
      operation[0].sem_num = 0;
      operation[0].sem_flg = 0;

      int32_t ret = semtimedop(m_semid, operation, 1, &timeout);

      if(ret == 0)
      {

         return wait_result(wait_result::Event0);

      }
      else if(ret == -1)
      {

         if(errno == EAGAIN)
         {

            return wait_result(wait_result::Timeout);

         }

      }

      return wait_result(wait_result::Failure);

   }
   else

#endif

#if defined(MUTEX_COND_TIMED)
   {

      int rc = pthread_mutex_lock(&m_mutex);

      if(rc < 0)
      {

         return wait_result(wait_result::Failure);

      }

      bool bFirst = true;

      timespec abs_time;

      while((m_thread != 0) && !pthread_equal(m_thread, pthread_self()))
      {

         if (bFirst)
         {

#ifdef ANDROID

            clock_gettime(CLOCK_MONOTONIC, &abs_time);

#else

            clock_gettime(CLOCK_REALTIME, &abs_time);

#endif

            ::duration d;

            d.m_iSeconds = abs_time.tv_sec + duration.m_iSeconds;

            d.m_iNanoseconds = abs_time.tv_nsec + duration.m_iNanoseconds;

            d.normalize();

            abs_time.tv_sec = d.m_iSeconds;

            abs_time.tv_nsec = d.m_iNanoseconds;

            bFirst = false;

         }

#ifdef ANDROID

         rc = pthread_cond_timedwait_monotonic_np(&m_cond, &m_mutex, &abs_time);

#else

         rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abs_time);

#endif

         if(rc == ETIMEDOUT)
         {

            int iError = pthread_mutex_unlock(&m_mutex);

            ASSERT(iError == 0);

            return wait_result(wait_result::Timeout);

         }
         else if(rc != 0)
         {

            int iError = pthread_mutex_unlock(&m_mutex);

            ASSERT(iError == 0);

            return wait_result(wait_result::Failure);

         }

      }

      if (m_count == 0)
      {

         m_thread = pthread_self();

      }

      m_count++;

      int iError = pthread_mutex_unlock(&m_mutex);

      ASSERT(iError == 0);

      return wait_result(wait_result::Event0);

   }

#else

   {

      timespec abs_time;

      clock_gettime(CLOCK_REALTIME, &abs_time);

      ::duration d;

      d.m_iSeconds = abs_time.tv_sec + duration.m_iSeconds;

      d.m_iNanoseconds = abs_time.tv_nsec + duration.m_iNanoseconds;

      d.normalize();

      abs_time.tv_sec = d.m_iSeconds;

      abs_time.tv_nsec = d.m_iNanoseconds;

      int rc = pthread_mutex_timedlock (&m_mutex, &abs_time);

      if (!rc)
      {

         return wait_result(wait_result::Event0);

      }
      else if(rc == ETIMEDOUT)
      {

         return wait_result(wait_result::Timeout);

      }
      else
      {

         return wait_result(wait_result::Failure);

      }

   }

#endif

}


bool mutex::lock()
{

#if defined(MUTEX_NAMED_POSIX)

   if (m_psem != SEM_FAILED)
   {

      timespec delay;

      int32_t ret = sem_wait(m_psem);

      if (ret == 0)
      {

         return true;

      }
      else
      {

         if (errno == ETIMEDOUT)
         {

            return false;

         }

      }

      return false;

   }
   else

#elif defined(MUTEX_NAMED_FD)

   if (m_pszName != NULL)
   {

      int rc = pthread_mutex_lock(&m_mutex);

      if (rc < 0)
      {

         return false;

      }

      if (m_count > 0 && pthread_equal(m_thread, pthread_self()))
      {

         m_count++;

         int iError = pthread_mutex_unlock(&m_mutex);

         ASSERT(iError == 0);

         return true;

      }

      while (true)
      {

         if (m_count > 0)
         {

            if (pthread_equal(m_thread, pthread_self()))
            {

               m_count++;

               int iError = pthread_mutex_unlock(&m_mutex);

               ASSERT(iError == 0);

               return true;

            }

         }
         else
         {

            rc = lockf(m_iFd, F_LOCK, 0);

            if (rc == 0)
            {

               m_count++;

               m_thread = pthread_self();

               pthread_mutex_unlock(&m_mutex);

               return true;

            }
            else
            {

               rc = errno;

               if (rc != EAGAIN && rc != EACCES)
               {

                  int iError = pthread_mutex_unlock(&m_mutex);

                  ASSERT(iError == 0);

                  return false;

               }

            }

         }

         int iError = pthread_mutex_unlock(&m_mutex);

         if (iError < 0)
         {

            ASSERT(FALSE);

            return false;

         }

         Sleep(100);

         rc = pthread_mutex_lock(&m_mutex);

         if (rc < 0)
         {

            return false;

         }

      }

      int iError = pthread_mutex_unlock(&m_mutex);

      ASSERT(iError == 0);

      return false;

   }
   else

#elif defined(MUTEX_NAMED_VSEM)

   if(m_semid >= 0)
   {

      struct sembuf operation[1] ;

      operation[0].sem_op = -1; // Wait
      operation[0].sem_num = 0;
      operation[0].sem_flg = 0;

      int32_t ret = semop(m_semid, operation, 1);

      if(ret < 0)
      {

         return false;

      }

   }
   else

#endif

#ifdef MUTEX_COND_TIMED
   {

      int rc = pthread_mutex_lock(&m_mutex);

      if(rc < 0)
      {

         return false;

      }

      while ((m_thread != 0) && !pthread_equal(m_thread, pthread_self()))
      {

         rc = pthread_cond_wait(&m_cond, &m_mutex);

         if(rc < 0)
         {

            int iError = pthread_mutex_unlock(&m_mutex);

            ASSERT(iError == 0);

            return false;

         }

      }

      if(m_count == 0)
      {

         m_thread = pthread_self();

      }

      m_count++;

      int iError = pthread_mutex_unlock(&m_mutex);

      ASSERT(iError == 0);

      return true;

   }

#else

   {

      int irc = pthread_mutex_lock(&m_mutex);

      if (irc)
      {

         return false;

      }

   }

#endif

   return true;

}


bool mutex::lock(const duration & duration)
{

   wait_result result = wait(duration);

   if (!result.signaled())
   {

      return false;

   }

   return true;

}


wait_result mutex::wait()
{

   if (!lock())
   {

      return wait_result(wait_result::Failure);

   }

   return wait_result(wait_result::Event0);

}

#endif


bool mutex::unlock()
{

#ifdef WINDOWS

   return ::ReleaseMutex(m_object) != FALSE;

#else

#if defined(MUTEX_NAMED_POSIX)

   if (m_psem != SEM_FAILED)
   {

      return sem_post(m_psem) == 0;

   }
   else

#elif defined(MUTEX_NAMED_FD)

   if (m_pszName != NULL)
   {

      int rc = pthread_mutex_lock(&m_mutex);

      if (rc < 0)
      {

         return false;

      }

      if (!pthread_equal(m_thread, pthread_self()))
      {

#ifdef _DEBUG

         ASSERT(FALSE);

#endif

         int iError = pthread_mutex_unlock(&m_mutex);

         ASSERT(iError == 0);

         return false;

      }

      rc = 0;

      if (m_count > 1)
      {

         m_count--;

      }
      else if (m_count == 1)
      {

         rc = lockf(m_iFd, F_ULOCK, 0);

         if (rc == 0)
         {

            m_count = 0;

         }

      }

      int iError = pthread_mutex_unlock(&m_mutex);

      ASSERT(iError == 0);

      return rc == 0;

   }
   else

#elif defined(NAMED_MUTEX_VSEM)

   if(m_semid >= 0)
   {

      struct sembuf operation[1] ;

      operation[0].sem_op = 1; //Release the resource
      operation[0].sem_num = 0;
      operation[0].sem_flg = SEM_UNDO;

      return semop(m_semid, operation, 1) == 0;

   }
   else

#endif

#ifdef MUTEX_COND_TIMED

  {

      int rc = pthread_mutex_lock(&m_mutex);

      if(rc < 0)
      {

         return false;

      }

      if(!pthread_equal(m_thread, pthread_self()))
      {


#ifdef _DEBUG

         ASSERT(FALSE);

#endif

         int iError = pthread_mutex_unlock(&m_mutex);

         ASSERT(iError == 0);

         return false;

      }

      rc = 0;

      if(m_count > 1)
      {

         m_count--;

      }
      else if(m_count == 1)
      {

         rc = pthread_cond_signal(&m_cond);

         if(rc == 0)
         {

            m_thread = 0;

            m_count = 0;

         }

      }

      int iError = pthread_mutex_unlock(&m_mutex);

      ASSERT(iError == 0);

      return true;

   }

#else

   {

      return pthread_mutex_unlock(&m_mutex) == 0;

   }

#endif

#endif // _WIN32

}



mutex * mutex::open_mutex(::aura::application * papp,  const char * pstrName)
{

#ifdef WINDOWS

   HANDLE h = ::OpenMutexW(SYNCHRONIZE, FALSE, ::str::international::utf8_to_unicode(pstrName));

   if(h == NULL || h == INVALID_HANDLE_VALUE)
      return NULL;

   mutex * pmutex = canew(mutex(papp, pstrName, h));

   return pmutex;

#elif defined(MUTEX_NAMED_POSIX)

   string strName = pstrName;

   sem_t * psem;

   int isCreator = 0;

   if ((psem = sem_open(strName, O_CREAT | O_EXCL, 0666, 1)) != SEM_FAILED)
   {

      isCreator = 1;

   }
   else
   {

      psem = sem_open(strName, 0);

      if (psem == SEM_FAILED)
      {

         //throw resource_exception(papp,"failed to create named mutex");
         throw resource_exception(papp);

      }

   }

   mutex * pmutex = new mutex(papp, strName, psem, isCreator);

   return pmutex;

#elif defined(MUTEX_NAMED_FD)

   if (pstrName == NULL || *pstrName == '\0')
   {

      return NULL;

   }

   SetLastError(0);

   ::file::path path;

   if (str::begins_ci(pstrName, "Global"))
   {

      path = "/var/tmp/ca2/lock/mutex/named";

   }
   else
   {

      path = getenv("HOME");

      path /= "Library/ca2/lock/mutex/named";

   }

   path /= pstrName;

   dir::mk(path.folder());

   int iFd = open(path, O_RDWR, S_IRWXU);

   if (iFd < 0)
   {

      throw resource_exception(::get_thread_app());

   }

   //m_pszName = strdup(path);

   //pthread_mutexattr_t attr;

   //pthread_mutexattr_init(&attr);

   //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

   //pthread_mutex_init(&m_mutex, &attr);

   mutex * pmutex = new mutex(papp, pstrName, iFd, false);

   return pmutex;

#elif defined(POSIX_NAMED_VSEM)

   if(pstrName == NULL || *pstrName == '\0')
      return NULL;

   string strName = pstrName;

   key_t key = ftok(strName, 0); //Generate a unique key or supply a value

   int32_t semid = semget(
               key, // a unique identifier to identify semaphore set
               1,  // number of semaphore in the semaphore set
               0666 // permissions (rwxrwxrwx) on the new
                    //semaphore set and creation flag
                  );
   if(semid < 0)
   {

      return NULL;

   }

   mutex * pmutex = new mutex(papp, strName, key, semid);

   return pmutex;

#endif

}


void wait_until_mutex_does_not_exist(const char * pszName)
{

   mutex * pmutex = new mutex(get_thread_app(), false, "Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");

   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {

      while(::GetLastError() == ERROR_ALREADY_EXISTS)
      {

         delete pmutex;

         Sleep(184);

         pmutex = new mutex(get_thread_app(), false, "Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");

      }

   }

   delete pmutex;

}

mutex * g_pmutexUiDestroyed = NULL;

CLASS_DECL_AURA mutex * get_ui_destroyed_mutex()
{

  return g_pmutexUiDestroyed;

}

#ifdef WINDOWSEX

null_dacl_security_attributes::null_dacl_security_attributes()
{


   ZERO(m_securityattributes);

   m_securityattributes.nLength = sizeof(m_securityattributes);

   m_securityattributes.bInheritHandle = FALSE; // object uninheritable

   // declare and initialize a security descriptor
   ZERO(m_securitydescriptor);

   bool bInitOk = InitializeSecurityDescriptor(&m_securitydescriptor,SECURITY_DESCRIPTOR_REVISION) != FALSE;

   if(bInitOk)
   {
      // give the security descriptor a Null Dacl
      // done using the  "TRUE, (PACL)NULL" here
      bool bSetOk = SetSecurityDescriptorDacl(&m_securitydescriptor,TRUE,(PACL)NULL,FALSE) != FALSE;

      if(bSetOk)
      {

         m_securityattributes.lpSecurityDescriptor = &m_securitydescriptor;

      }

   }

}

#endif


#if defined(INSTALL_SUBSYSTEM)

namespace install
{

   mutex::mutex(string strPlatform, string strSuffix) :
#ifdef WINDOWSEX
      ::mutex(NULL, false, "Global\\::ca2::fontopus::ccvotagus::install::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix, &m_securityattributes)
      , sync_object("Global\\::ca2::fontopus::ccvotagus::install::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix)
#else
   ::mutex(NULL, false, "Global\\::ca2::fontopus::ccvotagus::spa::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix, (LPSECURITY_ATTRIBUTES)NULL)
      , sync_object("Global\\::ca2::fontopus::ccvotagus::spa::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix)
#endif

   {
   }


   admin_mutex::admin_mutex(string strPlatform, string strSuffix) :
#ifdef WINDOWSEX
      ::mutex(NULL, false, "Global\\::ca2::fontopus::ccvotagus::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix, &m_securityattributes)
      , sync_object("Global\\::ca2::fontopus::ccvotagus::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix)
#else
      mutex(NULL, false, "Global\\::ca2::fontopus::ccvotagus::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix)
      , sync_object("Global\\::ca2::fontopus::ccvotagus::" + strPlatform + "::200010001951042219770204-11dd-ae16-0800200c7784" + strSuffix)
#endif

   {

   }


} // namespace install


#endif

