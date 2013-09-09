#include "framework.h"


void * base_memory_alloc(size_t size);
void * base_memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);
void * base_memory_realloc_dbg(void * pvoid, size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);
void   base_memory_free_dbg(void * pvoid, int32_t iBlockType);
size_t base_memory_size_dbg(void * pvoid, int32_t iBlockType);

/*
void use_base_ca2_allocator()
{

   g_pfnca2_alloc       = &base_memory_alloc;
   g_pfnca2_alloc_dbg   = &base_memory_alloc_dbg;
   g_pfnca2_realloc     = &base_memory_realloc;
   g_pfnca2_free        = &base_memory_free;
   g_pfnca2_msize       = &base_memory_size;

}
*/

void * base_memory_alloc(size_t size)
{
   byte * p = (byte *) ca2_heap_alloc(size + 4 + 32);
   if(p == NULL)
   {
      throw memory_exception(get_thread_app());
   }
   p[0] = 0;
   *((size_t *) &p[1]) = size;
   return p + 4 + 16;
}

void * base_memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{
   UNREFERENCED_PARAMETER(nBlockUse);
   UNREFERENCED_PARAMETER(szFileName);
   UNREFERENCED_PARAMETER(nLine);
   //TODO: to do the dbg version
   //byte * p = (byte *) _malloc_dbg(nSize + 4 + 32, nBlockUse, szFileName, nLine);
   byte * p = (byte *) ca2_heap_alloc_dbg(nSize + 4 + 32, nBlockUse, szFileName, nLine);
   if(p == NULL)
   {
      throw memory_exception(get_thread_app());
   }
   p[0] = 1;
   *((size_t *) &p[1]) = nSize;
   return p + 4 + 16;
}

void * base_memory_realloc_dbg(void * pvoid, size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{
   byte * p = (byte *) pvoid;
   if(p == NULL)
      return memory_alloc_dbg(nSize, nBlockUse, szFileName, nLine);
   p -= (4 + 16);
   try
   {
      if(p == NULL)
      {
         return memory_alloc_dbg(nSize, nBlockUse, szFileName, nLine);
      }
      else if(p[0] == 0)
      {
         p = (byte *) ca2_heap_realloc(p, nSize + 4 + 32);
      }
      else if(p[0] == 1)
      {
   //TODO: to do the dbg version
         p = (byte *) ca2_heap_realloc_dbg(p, nSize + 4 + 32, nBlockUse, szFileName, nLine);
         //p = (byte *) _realloc_dbg(p, nSize + 4 + 32, nBlockUse, szFileName, nLine);
      }
      else
      {
         return ca2_heap_realloc_dbg(pvoid, nSize, nBlockUse, szFileName, nLine);
      }
   }
   catch(...)
   {
      // todo: rethrow free exception
      {
         throw memory_exception(get_thread_app());
      }
   }
   if(p == NULL)
   {
      throw memory_exception(get_thread_app());
   }
   *((size_t *) &p[1]) = nSize;
   return p + 4 + 16;
}

void base_memory_free_dbg(void * pvoid, int32_t iBlockType)
{
   byte * p = (byte *) pvoid;
   if(p == NULL)
      return;
   p -= (4 + 16);
   try
   {
      if(p[0] == 0)
      {
         ca2_heap_free(p);
      }
      else if(p[0] == 1)
      {
            //TODO: to do the dbg version

         ca2_heap_free_dbg(p);
         //_free_dbg(p, iBlockType);
      }
      else
      {
         ca2_heap_free_dbg(pvoid);
      }
   }
   catch(...)
   {
      // todo: rethrow free exception
   }
}

size_t base_memory_size_dbg(void * pvoid, int32_t iBlockType)
{

   byte * p = (byte *) pvoid;

   if(p == NULL)
      return 0;

   p -= (4 + 16);

   try
   {
      if(p[0] == 0)
      {
         return *((size_t *) &p[1]);
      }
      else if(p[0] == 1)
      {
         //TODO: to do the dbg version

         return *((size_t *) &p[1]);
         //_free_dbg(p, iBlockType);
      }
      else
      {
         return memory_size_dbg(pvoid, iBlockType);
      }
   }
   catch(...)
   {
      // todo: rethrow free exception
   }

   return 0;

}

/*
/////////////////////////////////////////////////////////////////////////////
// Debug primitive::memory globals and implementation helpers

#ifdef DEBUG       // most of this file is for debugging

#undef new

void * __cdecl operator new(size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine);
void * __cdecl operator new[](size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine);

/////////////////////////////////////////////////////////////////////////////
// test allocation routines


#define new DEBUG_NEW

void * __alloc_memory_debug(size_t nSize, bool bIsObject,  const char * lpszFileName, int32_t nLine)
{
   return memory_alloc_dbg(nSize, bIsObject ? ___CLIENT_BLOCK : _NORMAL_BLOCK, lpszFileName, nLine);
}

void __free_memory_debug(void * pbData, bool bIsObject)
{
   memory_free(pbData, bIsObject ? ___CLIENT_BLOCK : _NORMAL_BLOCK);
}

/////////////////////////////////////////////////////////////////////////////
// allocation failure hook, tracking turn on

bool __default_alloc_hook(size_t, bool, LONG)
   { return TRUE; }

__STATIC_DATA __ALLOC_HOOK pfnAllocHook = __default_alloc_hook;

__STATIC_DATA _CRT_ALLOC_HOOK pfnCrtAllocHook = NULL;
#if _MSC_VER >= 1200
int32_t __cdecl __alloc_alloc_hook(int32_t nAllocType, void * pvData, size_t nSize,
   int32_t nBlockUse, long lRequest, const uchar * szFilename, int32_t nLine)
#else
int32_t __cdecl __alloc_alloc_hook(int32_t nAllocType, void * pvData, size_t nSize,
   int32_t nBlockUse, long lRequest, const char * szFilename, int32_t nLine)
#endif
{
#if _MSC_VER >= 1200
   if (nAllocType != _HOOK_ALLOC)
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, (const uchar*) szFilename, nLine);
   if ((pfnAllocHook)(nSize, _BLOCK_TYPE(nBlockUse) == ___CLIENT_BLOCK, lRequest))
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, (const uchar*) szFilename, nLine);
#else
   if (nAllocType != _HOOK_ALLOC)
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, szFilename, nLine);
   if ((pfnAllocHook)(nSize, _BLOCK_TYPE(nBlockUse) == ___CLIENT_BLOCK, lRequest))
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, szFilename, nLine);
#endif
   return FALSE;
}

__ALLOC_HOOK __set_alloc_hook(__ALLOC_HOOK pfnNewHook)
{
   if (pfnCrtAllocHook == NULL)
      pfnCrtAllocHook = _CrtSetAllocHook(__alloc_alloc_hook);

   __ALLOC_HOOK pfnOldHook = pfnAllocHook;
   pfnAllocHook = pfnNewHook;
   return pfnOldHook;
}

// This can be set to TRUE to override all __enable_memory_tracking calls,
// allowing all allocations, even core API internal allocations to be tracked.
bool gen_MemoryLeakOverride = FALSE;

bool __enable_memory_leak_override(bool bEnable)
{
   bool bOldState = gen_MemoryLeakOverride;
   gen_MemoryLeakOverride = bEnable;

   return bOldState;
}

bool __enable_memory_tracking(bool bTrack)
{
   if (gen_MemoryLeakOverride)
      return TRUE;

   int32_t nOldState = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
   if (bTrack)
      _CrtSetDbgFlag(nOldState | _CRTDBG_ALLOC_MEM_DF);
   else
      _CrtSetDbgFlag(nOldState & ~_CRTDBG_ALLOC_MEM_DF);
   return nOldState & _CRTDBG_ALLOC_MEM_DF;
}



/////////////////////////////////////////////////////////////////////////////
// memory_state

memory_state::memory_state()
{
   memset(this, 0, sizeof(*this));
}

void memory_state::update_data()
{
   for(int32_t i = 0; i < nBlockUseMax; i++)
   {
      m_lCounts[i] = m_memState.lCounts[i];
      m_lSizes[i] = m_memState.lSizes[i];
   }
   m_lHighWaterCount = m_memState.lHighWaterCount;
   m_lTotalCount = m_memState.lTotalCount;
}

// fills 'this' with the difference, returns TRUE if significant
bool memory_state::Difference(const memory_state& oldState,
      const memory_state& newState)
{
   int32_t nResult = _CrtMemDifference(&m_memState, &oldState.m_memState, &newState.m_memState);
   update_data();
   return nResult != 0;
}

void memory_state::dumpStatistics() const
{
   _CrtMemDumpStatistics(&m_memState);
}

// -- fill with current primitive::memory state
void memory_state::Checkpoint()
{
   _CrtMemCheckpoint(&m_memState);
   update_data();
}

// dump objects created after this primitive::memory state was checkpointed
// Will dump all objects if this primitive::memory state wasn't checkpointed
// dump all objects, report about non-objects also
// List request number in {}
void memory_state::dumpAllObjectsSince() const
{
   _CrtMemDumpAllObjectsSince(&m_memState);
}

/////////////////////////////////////////////////////////////////////////////
// Enumerate all objects allocated in the diagnostic primitive::memory heap

__STATIC void __do_for_all_objects_proxy(void * pObject, void * pContext)
{
   ___ENUM_CONTEXT* p = (___ENUM_CONTEXT*)pContext;
   (*p->m_pfn)((object*)pObject, p->m_pContext);
}

void _API
__do_for_all_objects(void (c_cdecl *pfn)(object*, void *), void * pContext)
{
   if (pfn == NULL)
   {
      throw invalid_argument_exception(get_app());
   }
   ___ENUM_CONTEXT context;
   context.m_pfn = pfn;
   context.m_pContext = pContext;
   _CrtDoForAllClientObjects(__do_for_all_objects_proxy, &context);
}

/////////////////////////////////////////////////////////////////////////////
// Automatic debug primitive::memory diagnostics

bool __dump_memory_leaks()
{
   return _CrtDumpMemoryLeaks();
}

#endif // ___NO_DEBUG_CRT

*/


