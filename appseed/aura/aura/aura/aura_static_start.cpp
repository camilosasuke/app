//
//  base_static_start.cpp
//  aura
//
//
//

#include "framework.h"

extern mutex * g_pmutexSignal;
extern class ::exception::engine * g_pexceptionengine;
CLASS_DECL_AURA void init_draw2d_mutex();
CLASS_DECL_AURA void term_draw2d_mutex();
void aura_auto_debug_teste();
void teste_aura_cmp();
void init_resolve_addr_file_func_line();
extern string * g_pstrCalcModuleFolderDup;
string calc_ca2_module_folder_dup();
extern mutex * s_pmutexMessageDispatch;
CLASS_DECL_AURA array<object * > * g_paAura = NULL;
namespace str
{


   namespace international
   {
      extern ::map < ::id,const ::id &,::id,const ::id & > * g_pmapRTL;
   } // namespace international

} // namespace str


extern ::map < void *, void *,::aura::application *, ::aura::application * > * g_pmapAura;

extern string_map < sp(::aura::library) > * g_pmapLibCall;

extern string_map < INT_PTR,INT_PTR > * g_pmapLibrary;

extern plex_heap_alloc_array * g_pheap;

extern critical_section * g_pmutexSystemHeap;

extern plex_heap_alloc_array * g_pheap;

extern mutex * g_pmutexThreadOn;

extern map < IDTHREAD, IDTHREAD, IDTHREAD, IDTHREAD > * g_pmapThreadOn;

#ifdef WINDOWS

   extern LARGE_INTEGER g_freq;

#endif

extern uint64_t g_firstNano;

//extern mutex * g_pmutexTrace;


plex_heap_alloc_array * g_pplexheapallocarray = NULL;

extern int g_iMemoryCountersStartable;
//id_space * create_id_space();

//void destroy_id_space();

extern mutex * g_pmutexFactory;

extern mutex * g_pmutexUiDestroyed;

//extern string * g_pstrLastStatus;

//extern string * g_pstrLastGlsStatus;
#ifdef ANDROID

   extern mutex * g_pmutexOutputDebugStringA;

#endif


#if defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

   extern mutex * g_pmutexMq;

   extern map < HTHREAD,HTHREAD,mq *,mq * > * g_pmapMq;

#endif

#if defined(LINUX) || defined(APPLEOS) || defined(METROWIN) || defined(ANDROID)

   //extern mutex * g_pmutexThreadIdHandleLock;

   //extern mutex * g_pmutexThreadIdLock;

   #if !defined(METROWIN)

      //extern mutex * g_pmutexPendingThreadsLock;

   #endif

   extern mutex * g_pmutexTlsData;

#endif // defined(LINUX) || defined(APPLEOS) || defined(METROWIN)

#if defined(LINUX) || defined(APPLEOS)



   extern mutex * g_pmutexTz;

   //extern map < HTHREAD, HTHREAD, PendingThreadInfo, PendingThreadInfo > * g_ppendingThreads;

   extern mutex * g_pmutexThreadHandleLock;

#endif // defined(LINUX) || defined(APPLEOS)

#if defined(LINUX)

#endif


/*
#if defined(APPLEOS)

//#include "aura/os/macos/macos_window_impl.h"

extern oswindow_dataptra * g_poswindowdataptra;

#elif defined(APPLE_IOS)

//#include "aura/os/ios/ios_window_impl.h"

extern oswindow_dataptra * g_poswindowdataptra;

#endif
 */

#ifdef APPLEOS

   extern mutex * g_pmutexCvt;

#endif

#undef new



namespace aura
{


   namespace static_start
   {


      CLASS_DECL_AURA void init()
      {

#ifdef WINDOWS

         QueryPerformanceFrequency(&g_freq);

#endif

         g_firstNano = get_nanos();

         xxdebug_box("aura.dll base_static_start (0)", "box", MB_OK);

         g_pexceptionengine = (class ::exception::engine *) malloc(sizeof(class exception::engine));

         new(g_pexceptionengine) class ::exception::engine(NULL);

//#if !defined(_ACID_LIBRARY)

         ace_set_alloc(&memory_alloc, &memory_realloc, &memory_free, &memory_size);

//#endif

         /*

          if(g_pfnca2_alloc == NULL)
          {
            g_pfnca2_alloc       = memory_alloc;
          }
          if(g_pfnca2_alloc_dbg == NULL)
          {
            g_pfnca2_alloc_dbg   = _ca_alloc_dbg;
          }
          if(g_pfnca2_realloc == NULL)
          {
            g_pfnca2_realloc     = memory_realloc_dbg;
          }
          if(g_pfnca2_free == NULL)
          {
            g_pfnca2_free        = memory_free_dbg;
          }
          if(g_pfnca2_msize == NULL)
          {
            g_pfnca2_msize       = _ca_msize;
          }

         */
#ifndef __MCRTDBG
         g_pplexheapallocarray = new plex_heap_alloc_array();
#endif

         s_pstringmanager = new string_manager();

         ::id_space::s_pidspace = new id_space();

         g_pstrCalcModuleFolderDup = new string(calc_ca2_module_folder_dup());

         g_paAura = new array<object * >;

#ifdef ANDROID

         g_pmutexOutputDebugStringA = new mutex(NULL);

#endif


#ifdef APPLEOS

         g_pmutexCvt = new mutex(NULL);

#endif
         
         ::thread::s_pmutexDependencies = new mutex();

         g_pmutexThreadOn = new mutex();

         g_pmapThreadOn = new ::map < IDTHREAD, IDTHREAD, IDTHREAD, IDTHREAD >;

         g_pmutexSignal = new mutex();

         //g_pstrLastStatus = new string();

         //g_pstrLastGlsStatus = new string();

         g_pmutexSystemHeap = new critical_section();

#if MEMDLEAK

         memdleak_init();

#endif

         g_pmapAura =new ::map < void *,void *,::aura::application *,::aura::application * >;

         //g_pmutexTrace = new mutex();

         g_pmutexUiDestroyed = new mutex();

         s_pmutexMessageDispatch = new mutex();


#if defined(WINDOWSEX)

         //os_thread::s_pmutex = new mutex();

         //os_thread::s_pptra = new ref_array <  os_thread > ();

#endif

#if defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

         g_pmutexMq = new mutex;

         g_pmapMq = new map < HTHREAD,HTHREAD,mq *,mq * >;

#endif

#if defined(LINUX) || defined(APPLEOS) || defined(METROWIN)

//         g_pmutexThreadIdHandleLock = new mutex;

         //       g_pmutexThreadIdLock = new mutex;


#ifndef METROWIN

         //     g_pmutexPendingThreadsLock = new mutex;

#endif

         g_pmutexTlsData = new mutex;

         //hthread::s_pmutex = new mutex();

         //hthread::s_pptra = new ref_array <  hthread > ();

#endif // defined(LINUX) || defined(APPLEOS) || defined(METROWIN)

#if defined(LINUX) || defined(APPLEOS)


         g_pmutexTz = new mutex();

//         g_ppendingThreads = new map < HTHREAD, HTHREAD, PendingThreadInfo, PendingThreadInfo >();

//         g_pmutexThreadHandleLock = new mutex;

#endif // defined(LINUX) || defined(APPLEOS)


//#if defined(APPLEOS)
//
         //       g_poswindowdataptra = new oswindow_dataptra;
//
//#endif // defined(APPLEOS)

         // IMPLEMENT_AXIS_FIXED_ALLOC_CONSTRUCTOR(var, 1024)
         // IMPLEMENT_AXIS_FIXED_ALLOC_CONSTRUCTOR(property, 1024)

         ::str::international::g_pmapRTL = new ::map < ::id,const ::id &,::id,const ::id & >();

         g_pmapLibrary = new string_map < INT_PTR,INT_PTR >();

         g_pmutexFactory = new mutex;
#ifndef METROWIN
         br_init(NULL);
#endif

         // Only draw2d implementations needing "big" synch should init_draw2d_mutex();
         // init_draw2d_mutex();

         aura_auto_debug_teste();

         teste_aura_cmp();

         g_pmapLibCall = new string_map < sp(::aura::library) >();

#ifndef ANDROID

         g_iMemoryCountersStartable = 1;

#endif

      }














































      CLASS_DECL_AURA void term()
      {

         delete g_pmapLibCall;

         g_pmapLibCall = NULL;

         term_draw2d_mutex();

         delete g_pmutexFactory;

         g_pmutexFactory = NULL;

         delete g_pmapLibrary;

         g_pmapLibrary = NULL;

         delete ::str::international::g_pmapRTL;

         ::str::international::g_pmapRTL = NULL;

         // IMPLEMENT_AXIS_FIXED_ALLOC_DESTRUCTOR(property)
         // IMPLEMENT_AXIS_FIXED_ALLOC_DESTRUCTOR(var)

#if defined(LINUX) || defined(APPLEOS)

//         delete g_pmutexThreadHandleLock;

         //       g_pmutexThreadHandleLock = NULL;

//         delete g_ppendingThreads;

         //       g_ppendingThreads = NULL;

         delete g_pmutexTz;

         g_pmutexTz = NULL;


#endif // defined(LINUX) || defined(APPLEOS)

#if defined(LINUX) || defined(APPLEOS) || defined(METROWIN) || defined(ANDROID)

         //delete hthread::s_pptra;

         //hthread::s_pptra = NULL;

         //delete hthread::s_pmutex;

         //hthread::s_pmutex = NULL;

         delete g_pmutexTlsData;

         g_pmutexTlsData = NULL;


#ifndef METROWIN

//         delete g_pmutexPendingThreadsLock;

         //       g_pmutexPendingThreadsLock = NULL;

#endif

         //     delete g_pmutexThreadIdHandleLock;

         //   g_pmutexThreadIdHandleLock = NULL;

         // delete g_pmutexThreadIdLock;

         //g_pmutexThreadIdLock = NULL;

#endif  // defined(LINUX) || defined(APPLEOS) || defined(METROWIN)

#if defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

         ::aura::del(g_pmutexMq);

         ::aura::del(g_pmapMq);


#endif // defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

//#if defined(APPLEOS)

         //       delete g_poswindowdataptra;

         //     g_poswindowdataptra = NULL;

//#endif // defined(APPLEOS)

         //delete g_pmutexTrace;

         //g_pmutexTrace = NULL;
         ::aura::del(s_pmutexMessageDispatch);

         ::aura::del(g_pmutexUiDestroyed);

         ::aura::del(g_pmapAura);

#if MEMDLEAK

         memdleak_term();

#endif

         ::aura::del(g_pmutexSystemHeap);

         ::aura::del(g_pmutexSignal);

         ::aura::del(g_pmutexThreadOn);

         ::aura::del(g_pmapThreadOn);
         
         ::aura::del(::thread::s_pmutexDependencies);

         // delete g_pstrLastGlsStatus;

         // g_pstrLastGlsStatus = NULL;

         // delete g_pstrLastStatus;

         // g_pstrLastStatus = NULL;

#ifdef APPLEOS

         delete g_pmutexCvt;

         g_pmutexCvt = NULL;

#endif

//         destroy_id_space();

#ifdef ANDROID

         ::aura::del(g_pmutexOutputDebugStringA);

#endif

         for(auto * po : *g_paAura)
         {

            try
            {

               delete po;

            }
            catch(...)
            {


            }

         }

         ::aura::del(g_paAura);

         ::aura::del(g_pstrCalcModuleFolderDup);

         ::aura::del(::id_space::s_pidspace);

         ::aura::del(s_pstringmanager);


         //::aura::del(g_pplexheapallocarray);

#if !defined(__MCRTDBG) && !MEMDLEAK
         ::aura::del(g_pheap);
#endif
#if MEMDLEAK

         ::set_thread(NULL);

#ifdef __USE_BFD

         init_resolve_addr_file_func_line();

#endif

         memdleak_dump();

#endif

      }

      ::aura::system * aura_create_system()
      {
         return new ::aura::system(NULL, NULL);
      }



   } // namespace static_start


} // namespace aura


static mutex * s_pmutexDraw2d = NULL;

mutex & draw2d_mutex()
{

   return *s_pmutexDraw2d;

}


void init_draw2d_mutex()
{

   s_pmutexDraw2d = new mutex();

}


void term_draw2d_mutex()
{

   ::aura::del(s_pmutexDraw2d);

}
