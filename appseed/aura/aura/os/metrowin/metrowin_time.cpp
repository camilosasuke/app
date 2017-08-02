//#include "framework.h"
//#include "metrowin.h"

//DWORD get_tick_count()
//{
//
//   return (DWORD) (GetTickCount64() % 0x100000000ULL);
//
//}


LARGE_INTEGER g_freq;

uint64_t get_nanos()
{

   LARGE_INTEGER li = {};

   QueryPerformanceCounter(&li);

   return muldiv64(li.QuadPart, 1000 * 1000 * 1000, g_freq.QuadPart);

}



CLASS_DECL_AURA void sleep(const ::duration & dur)
{

   Sleep(dur.total_milliseconds());

}
