//#include "framework.h"


//#include <signal.h>
//#include <unistd.h>


//extern thread_pointer < os_thread > t_posthread;
//extern thread_pointer < hthread > currentThread;


int32_t create_process(const char * _cmd_line, int32_t * pprocessId)
{
   char *   exec_path_name = NULL;
   char *	cmd_line;

   cmd_line = (char *) memory_alloc(strlen(_cmd_line ) + 1 );

   if(cmd_line == NULL)
            return 0;

   strcpy_dup(cmd_line, _cmd_line);

   if((*pprocessId = fork()) == 0)
   {
      // child
      char		*pArg, *pPtr;
      char		*argv[1024 + 1];
      int32_t		 argc;
      if( ( pArg = strrchr_dup( exec_path_name, '/' ) ) != NULL )
         pArg++;
      else
         pArg = exec_path_name;
      argv[0] = pArg;
      argc = 1;

      if( cmd_line != NULL && *cmd_line != '\0' )
      {
         pArg = strtok_r_dup(cmd_line, " ", &pPtr);
         while( pArg != NULL )
         {
            argv[argc] = pArg;
            argc++;
            if( argc >= 1024 )
               break;
            pArg = strtok_r_dup(NULL, " ", &pPtr);
         }
      }
      argv[argc] = NULL;

      execv(exec_path_name, argv);
      free(cmd_line);
      exit( -1 );
   }
   else if(*pprocessId == -1)
   {
      // in parent, but error
      *pprocessId = 0;
      free(cmd_line);
      return 0;
   }
   // in parent, success
   return 1;
}

CLASS_DECL_AURA int32_t call_async(
                            const char * pszPath,
                            const char * pszParam,
                            const char * pszDir,
                            int32_t iShow,
							bool bPrivileged,
   unsigned int * puiPid)
{
    string strCmdLine;

    strCmdLine = pszPath;
    if(strlen_dup(pszParam) > 0)
    {
        strCmdLine +=  " ";
        strCmdLine += pszParam;
    }

    int32_t processId;

    if(!create_process(strCmdLine, &processId))
        return -1;

    if (puiPid != NULL)
    {

       *puiPid = processId;

    }

    return 0;

}

CLASS_DECL_AURA DWORD call_sync(
                             const char * pszPath,
                             const char * pszParam,
                             const char * pszDir,
                             int32_t iShow,
                             int32_t iRetry,
                             int32_t iSleep,
                             int32_t (* pfnOnRetry)(int32_t iTry, dword_ptr dwParam),
                             dword_ptr dwParam,
                             unsigned int * puiId)
{
    string strCmdLine;

    strCmdLine = pszPath;
    if(strlen_dup(pszParam) > 0)
    {
        strCmdLine +=  " ";
        strCmdLine += pszParam;
    }

    int32_t processId;

    if(!create_process(strCmdLine, &processId))
        return -1;

    if (puiId != NULL)
    {

       *puiId = processId;

    }

    while(true)
    {

        if(kill(processId, 0) == -1 && errno == ESRCH) // No process can be found corresponding to processId
            break;
        sleep(1);
    }

    return 0;
}










CLASS_DECL_AURA bool main_initialize()
{

//   t_posthread = new os_thread(NULL,NULL);

   //t_posthread->m_bRun = true;

   return true;

}


CLASS_DECL_AURA bool main_finalize()
{

   //if(t_posthread != NULL)
   //{

   //   try
   //   {

   //      delete t_posthread;

   //   }
   //   catch(...)
   //   {

   //   }

   //   t_posthread = NULL;

   //}

//   os_thread::stop_all((5000) * 49);

   //finalize_primitive_trace();

   return true;

}








namespace process
{

   CLASS_DECL_AURA bool set_priority(int32_t priority)
   {

      int32_t iPolicy = SCHED_OTHER;

      sched_param schedparam;

      schedparam.sched_priority = 0;

      get_os_priority(&iPolicy, &schedparam, priority);

      sched_setscheduler(0, iPolicy, &schedparam);

      return true;

   }




} // namespace process

unichar * g_pwszCommandLine = NULL;

CLASS_DECL_AURA unichar * GetCommandLineW()
{

   return g_pwszCommandLine;

}


CLASS_DECL_AURA bool shell_execute_sync(const char * pszPath, const char * pszParam, ::duration durationTimeout)
{

   string strCmdLine;

   strCmdLine = pszPath;

   if (strlen_dup(pszParam) > 0)
   {

      strCmdLine += " ";

      strCmdLine += pszParam;

   }

   int32_t processId;

   if (!create_process(strCmdLine, &processId))
      return -1;

   ::datetime::time timeEnd = ::datetime::time::get_current_time() + durationTimeout;

   while (::datetime::time::get_current_time() < timeEnd)
   {

      if (kill(processId, 0) == -1 && errno == ESRCH) // No process can be found corresponding to processId
         return true;

      sleep(1);

   }

   return true;

}


CLASS_DECL_AURA bool is_shared_library_busy(const stringa & stra)
{

   return true;

}