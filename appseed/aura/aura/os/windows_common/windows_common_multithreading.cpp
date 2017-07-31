




int32_t get_os_thread_priority(int32_t iCa2Priority)
{

   if (iCa2Priority <= (int) ::multithreading::priority_none)
      return THREAD_PRIORITY_NORMAL;

   if (iCa2Priority <= (int) ::multithreading::priority_idle)
      return THREAD_PRIORITY_IDLE;

   if (iCa2Priority <= (int) ::multithreading::priority_lowest)
      return THREAD_PRIORITY_LOWEST;

   if (iCa2Priority <= (int) ::multithreading::priority_below_normal)
      return THREAD_PRIORITY_BELOW_NORMAL;

   if (iCa2Priority <= (int) ::multithreading::priority_normal)
      return THREAD_PRIORITY_NORMAL;

   if (iCa2Priority <= (int) ::multithreading::priority_above_normal)
      return THREAD_PRIORITY_ABOVE_NORMAL;

   if (iCa2Priority <= (int) ::multithreading::priority_highest)
      return THREAD_PRIORITY_HIGHEST;

   return THREAD_PRIORITY_TIME_CRITICAL;

}



int32_t get_os_priority_class(int32_t iCa2Priority)
{

   if (iCa2Priority <= (int) ::multithreading::priority_none)
      return 0;

   if (iCa2Priority <= (int) ::multithreading::priority_idle)
      return IDLE_PRIORITY_CLASS;

   if (iCa2Priority <= (int) ::multithreading::priority_lowest)
      return BELOW_NORMAL_PRIORITY_CLASS;

   if (iCa2Priority <= (int) ::multithreading::priority_below_normal)
      return BELOW_NORMAL_PRIORITY_CLASS;

   if (iCa2Priority <= (int) ::multithreading::priority_normal)
      return NORMAL_PRIORITY_CLASS;

   if (iCa2Priority <= (int) ::multithreading::priority_above_normal)
      return ABOVE_NORMAL_PRIORITY_CLASS;

   if (iCa2Priority <= (int) ::multithreading::priority_highest)
      return HIGH_PRIORITY_CLASS;

   return REALTIME_PRIORITY_CLASS;

}







int32_t get_os_thread_scheduling_priority(int32_t nPriority)
{

   int32_t iCa2Priority;

   if (nPriority <= THREAD_PRIORITY_IDLE)
   {
      iCa2Priority = ::multithreading::priority_idle;
   }
   else if (nPriority <= THREAD_PRIORITY_LOWEST)
   {
      iCa2Priority = ::multithreading::priority_lowest;
   }
   else if (nPriority <= THREAD_PRIORITY_BELOW_NORMAL)
   {
      iCa2Priority = ::multithreading::priority_below_normal;
   }
   else if (nPriority <= THREAD_PRIORITY_NORMAL)
   {
      iCa2Priority = ::multithreading::priority_normal;
   }
   else if (nPriority <= THREAD_PRIORITY_ABOVE_NORMAL)
   {
      iCa2Priority = ::multithreading::priority_above_normal;
   }
   else if (nPriority <= THREAD_PRIORITY_HIGHEST)
   {
      iCa2Priority = ::multithreading::priority_highest;
   }
   else
   {
      iCa2Priority = ::multithreading::priority_time_critical;
   }

   return iCa2Priority;

}


int32_t get_os_class_scheduling_priority(int32_t nPriority)
{

   int32_t iCa2Priority;

   if (nPriority <= IDLE_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_idle;
   }
   else if (nPriority <= BELOW_NORMAL_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_below_normal;
   }
   else if (nPriority <= NORMAL_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_normal;
   }
   else if (nPriority <= ABOVE_NORMAL_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_above_normal;
   }
   else if (nPriority <= HIGH_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_highest;
   }
   else
   {
      iCa2Priority = ::multithreading::priority_time_critical;
   }

   return iCa2Priority;

}




namespace multithreading
{

   bool set_priority(int32_t priority)
   {

      return (::SetThreadPriority(::GetCurrentThread(), get_os_thread_priority(priority)) != 0);
   }

   int32_t priority()
   {
      return ::GetThreadPriority(::GetCurrentThread());
   }


} // namespace core






int get_processor_count()
{

   SYSTEM_INFO sysinfo;
   GetSystemInfo(&sysinfo);

   return sysinfo.dwNumberOfProcessors;

}

