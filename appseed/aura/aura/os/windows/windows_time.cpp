//#include "framework.h"






CLASS_DECL_AURA int_bool get_file_time(HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime)
{

   return GetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime) != FALSE;

}

LARGE_INTEGER g_freq;

uint64_t get_nanos()
{

   LARGE_INTEGER li = {};

   QueryPerformanceCounter(&li);

   return muldiv64(li.QuadPart, 1000 * 1000 * 1000, g_freq.QuadPart);

}







//
///**********************************=> unix ************************************/
//#ifndef _WIN32
//void SleepInMs(uint32 ms) {
//   struct timespec ts;
//   ts.tv_sec = ms / 1000;
//   ts.tv_nsec = ms % 1000 * 1000000;
//
//   while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
//}
//
//void SleepInUs(uint32 us) {
//   struct timespec ts;
//   ts.tv_sec = us / 1000000;
//   ts.tv_nsec = us % 1000000 * 1000;
//
//   while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
//}
//
//#ifndef __APPLE__
//uint64 NowInUs() {
//   struct timespec now;
//   clock_gettime(CLOCK_MONOTONIC, &now);
//   return static_cast<uint64>(now.tv_sec) * 1000000 + now.tv_nsec / 1000;
//}
//
//#else // mac
//uint64 NowInUs() {
//   clock_serv_t cs;
//   mach_timespec_t ts;
//
//   host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
//   clock_get_time(cs, &ts);
//   mach_port_deallocate(mach_task_self(), cs);
//
//   return static_cast<uint64>(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
//}
//#endif // __APPLE__
//#endif // _WIN32
///************************************ unix <=**********************************/
//
///**********************************=> win *************************************/
//#ifdef _WIN32
//void SleepInMs(uint32 ms) {
//   ::Sleep(ms);
//}
//
//void SleepInUs(uint32 us) {
//   ::LARGE_INTEGER ft;
//   ft.QuadPart = -static_cast<int64>(us * 10);  // '-' using relative time
//
//   ::HANDLE timer = ::CreateWaitableTimer(NULL, TRUE, NULL);
//   ::SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
//   ::WaitForSingleObject(timer, INFINITE);
//   ::CloseHandle(timer);
//}
//
//static inline uint64 GetPerfFrequency() {
//   ::LARGE_INTEGER freq;
//   ::QueryPerformanceFrequency(&freq);
//   return freq.QuadPart;
//}
//
//static inline uint64 PerfFrequency() {
//   static uint64 xFreq = GetPerfFrequency();
//   return xFreq;
//}
//
//static inline uint64 PerfCounter() {
//   ::LARGE_INTEGER counter;
//   ::QueryPerformanceCounter(&counter);
//   return counter.QuadPart;
//}
//
//uint64 NowInUs() {
//   return static_cast<uint64>(
//      static_cast<double>(PerfCounter()) * 1000000 / PerfFrequency());
//}
//#endif // _WIN32

/* Windows sleep in 100ns units */
BOOLEAN nanosleep(LONGLONG ns) {
   /* Declarations */
   HANDLE timer;   /* Timer handle */
   LARGE_INTEGER li;   /* Time defintion */
                       /* Create timer */
   if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
   {
      Sleep(ns / (1000 * 1000))
      return TRUE;
   }
   /* Set timer properties */
   li.QuadPart = -(ns / 100);
   if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
      CloseHandle(timer);
      return FALSE;
   }
   /* Start & wait for timer */
   WaitForSingleObject(timer, INFINITE);
   /* Clean resources */
   CloseHandle(timer);
   /* Slept without problems */
   return TRUE;
}


void sleep(const ::duration & dur)
{

   nanosleep(dur.get_total_nanos());

}

