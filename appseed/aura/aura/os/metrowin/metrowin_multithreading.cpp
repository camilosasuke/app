// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

//#include "framework.h"
//#include "metrowin.h"
//#include "metrowin_metrowin.h"

#undef System

//#include <assert.h>
////#include <vector>
////#include <set>
////#include <map>

//using namespace std;
//using namespace Platform;
#pragma push_macro("System")
#undef System
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
#pragma pop_macro("System")

//BEGIN_EXTERN_C
//mutex * g_pmutexPendingThreadsLock = NULL;
mutex * g_pmutexThreadIdHandleLock = NULL;
mutex * g_pmutexThreadIdLock = NULL;
//mutex * g_pmutexTlsData = NULL;
//END_EXTERN_C


thread_data::thread_data()
{

   m_dwIndex = thread_alloc();

}


thread_data::~thread_data()
{

   thread_free(m_dwIndex);

}


void * thread_data::get()
{

   return tls_get_value(m_dwIndex);

}


void thread_data::set(void * p)
{

   thread_set_data(m_dwIndex,(LPVOID)p);

}





//map<HTHREAD, HTHREAD, PendingThreadInfo, PendingThreadInfo> & pendingThreads()
//{
//
//   static map<HTHREAD, HTHREAD, PendingThreadInfo,PendingThreadInfo> * pts = new map<HTHREAD, HTHREAD, PendingThreadInfo, PendingThreadInfo>();
//
//   return *pts;
//
//}
//map < DWORD, DWORD, HTHREAD, HTHREAD > & thread_id_handle_map()
//{
//
//   static map < DWORD,DWORD, HTHREAD,  HTHREAD > * s_pmap = new map < DWORD, DWORD, HTHREAD, HTHREAD >();
//
//   return *s_pmap;
//
//}
//
//
//map < HTHREAD,HTHREAD, DWORD, DWORD > & thread_id_map()
//{
//
//   static map < HTHREAD, HTHREAD, DWORD, DWORD > * s_pmap = new map < HTHREAD, HTHREAD, DWORD, DWORD >();
//
//   return *s_pmap;
//
//}
//
//DWORD DwThreadId()
//{
//   static DWORD g_dw_thread_id = 0;
//
//   if(g_dw_thread_id  <= 0)
//      g_dw_thread_id = 1;
//
//   return g_dw_thread_id++;
//}


//typedef void_ptra ThreadLocalData;


//#ifdef APPLE_IOS
//thread_pointer < ThreadLocalData > currentThreadData;
//#else
//CLASS_DECL_THREAD ThreadLocalData * currentThreadData;
//#endif
////thread_int_ptr < DWORD > currentThreadId;
////thread_pointer < HTHREAD > currentThread;
////thread_pointer < hthread > t_phthread;
//
//
//raw_array<DWORD> * freeTlsIndices = NULL;
//

//map < HTHREAD,HTHREAD,HTHREAD,HTHREAD > * s_pmapHthreadHthread = NULL;
//map < DWORD,DWORD,HTHREAD,HTHREAD > * s_pmapDwordHthread = NULL;
//map < HTHREAD,HTHREAD,DWORD,DWORD > * s_pmapHthreadDword = NULL;
//map < IDTHREAD,IDTHREAD,ThreadLocalData *,ThreadLocalData * > * allthreaddata = NULL;
//
//
//DWORD nextTlsIndex = 0;



// Converts a Win32 thread priority to WinRT format.
WorkItemPriority GetWorkItemPriority(int nPriority)
{
   if (nPriority < 0)
      return WorkItemPriority::Low;
   else if (nPriority > 0)
      return WorkItemPriority::High;
   else
      return WorkItemPriority::Normal;
}


// Helper shared between CreateThread and ResumeThread.
//void StartThread(LPTHREAD_START_ROUTINE pfn,void * pv,HTHREAD hthread,int nPriority)
//{
//
//   auto workItemHandler = ref new WorkItemHandler([=](IAsyncAction^)
//   {
//
//      t_phthread = hthread;
//
//      synch_lock mlThreadId(g_pmutexThreadIdLock);
//
//      currentThreadId =  thread_id_map()[t_phthread];
//
//      mlThreadId.unlock();
//
//      // Run the user callback.
//      try
//      {
//         pfn(pv);
//      }
//      catch (...) { }
//
//      // Clean up any TLS allocations made by this thread.
//      tls_shutdown();
//
//      // Signal that the thread has completed.
//      hthread->m_pevent->set_event();
//      delete hthread;
//
//   }, CallbackContext::Any);
//
//   ThreadPool::RunAsync(workItemHandler, GetWorkItemPriority(nPriority), WorkItemOptions::TimeSliced);
//}


//HTHREAD WINAPI CreateThread(LPSECURITY_ATTRIBUTES unusedThreadAttributes,uint_ptr unusedStackSize,LPTHREAD_START_ROUTINE pfn,void * pv,uint32_t uiCreationFlags,uint32_t * puiId)
//{
//   // Validate parameters.
//   assert(unusedThreadAttributes == nullptr);
//   assert(unusedStackSize == 0);
//   assert((uiCreationFlags & ~CREATE_SUSPENDED) == 0);
//   //assert(unusedThreadId == nullptr);
//
//   // Create a handle that will be signalled when the thread has completed.
//   HTHREAD threadHandle = new hthread;
//
//   //threadHandle->mCreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
//
//   if (!threadHandle)
//      return nullptr;
//
//   synch_lock mlThreadId(g_pmutexThreadIdLock);
//
//   thread_id_map().set_at(threadHandle, DwThreadId());
//
//   if(puiId != NULL)
//   {
//      *puiId = thread_id_map()[threadHandle];
//   }
//
//   synch_lock mlThreadIdHandle(g_pmutexThreadIdHandleLock);
//
//   thread_id_handle_map().set_at(thread_id_map()[threadHandle], threadHandle);
//
//   mlThreadIdHandle.unlock();
//
//   mlThreadId.unlock();
//
//   // Make a copy of the handle for internal use. This is necessary because
//   // the caller is responsible for closing the handle returned by CreateThread,
//   // and they may do that before or after the thread has finished running.
//   //HANDLE completionEvent;
//
//   //if (!DuplicateHandle(GetCurrentProcess(), threadHandle->m_pevent->m_hEvent, GetCurrentProcess(), &completionEvent, 0, false, DUPLICATE_SAME_ACCESS))
//   //{
//     // CloseHandle(threadHandle);
//      //return nullptr;
//   //}
//
//   try
//   {
//
//      if (uiCreationFlags & CREATE_SUSPENDED)
//      {
//
//         // Store info about a suspended thread.
//         //PendingThreadInfo info;
//
//         //info.m_pfn                 = pfn;
//         //info.m_pv                  = pv;
//         //info.m_hthread             = threadHandle;
//         //info.m_peventCompletion    = threadHandle->m_pevent;
//         //info.m_peventSuspension    = new manual_reset_event(get_thread_app());
//         //info.m_iPriority           = 0;
//
//         synch_lock lock(g_pmutexPendingThreadsLock);
//
//         pendingThreads()[threadHandle] = info;
//
//         //::WaitForSingleObjectEx(info.suspensionEvent, INFINITE, FALSE);
//      }
//      else
//      {
//         // Start the thread immediately.
//         StartThread(pfn, pv, threadHandle, 0);
//      }
//      if(puiId != NULL)
//      {
//         *puiId =thread_id_map()[threadHandle];
//      }
//
//      return threadHandle;
//   }
//   catch (...)
//   {
//      // Clean up if thread creation fails.
//      delete threadHandle;
//
//      return nullptr;
//   }
//}


//DWORD WINAPI __ResumeThread(HTHREAD hThread)
//{
//   synch_lock lock(g_pmutexPendingThreadsLock);
//
//   // Look up the requested thread.
//   auto threadInfo = pendingThreads().PLookup(hThread);
//
//   if (threadInfo == NULL)
//   {
//      // Can only resume threads while they are in CREATE_SUSPENDED state.
//      //assert(false);
//      return (DWORD)-1;
//   }
//
//   // Start the thread.
//   try
//   {
//
//      PendingThreadInfo& info = threadInfo->m_element2;
//
//      StartThread(info.m_pfn, info.m_pv, info.m_hthread, info.m_iPriority);
//
//   }
//   catch (...)
//   {
//      return (DWORD)-1;
//   }
//
//   // Remove this thread from the pending list.
//   pendingThreads().remove_key(hThread);
//
//   return 0;
//}
//
//
//BOOL WINAPI __SetThreadPriority(HTHREAD hThread, int iPriority)
//{
//   synch_lock lock(g_pmutexPendingThreadsLock);
//
//   // Look up the requested thread.
//   auto threadInfo = pendingThreads().PLookup(hThread);
//
//   if (threadInfo == NULL)
//   {
//      // Can only set priority on threads while they are in CREATE_SUSPENDED state.
//      //assert(false);
//      return false;
//   }
//
//   // Store the new priority.
//   threadInfo->m_element2.m_iPriority = iPriority;
//
//   return true;
//}
//

//
//
//DWORD WINAPI tls_alloc()
//{
//   synch_lock lock(g_pmutexTlsData);
//
//   // Can we reuse a previously freed TLS slot?
//   if (freeTlsIndices.get_count() > 0)
//   {
//      DWORD result = freeTlsIndices.element_at(freeTlsIndices.get_count() - 1);
//      freeTlsIndices.remove_at(freeTlsIndices.get_count() - 1);
//      return result;
//   }
//
//   // Allocate a new TLS slot.
//   return nextTlsIndex++;
//}

//
//BOOL WINAPI tls_free(DWORD dwIndex)
//{
//   synch_lock lock(g_pmutexTlsData);
//
//   assert(dwIndex < nextTlsIndex);
//   for(int i = 0; i < freeTlsIndices.get_count(); i++)
//   {
//      assert(freeTlsIndices.element_at(i) != dwIndex);
//   }
//
//   // Store this slot for reuse by TlsAlloc.
//   try
//   {
//      freeTlsIndices.add(dwIndex);
//   }
//   catch (...)
//   {
//      return false;
//   }
//
//   // Zero the value for all threads that might be using this now freed slot.
//
//   POSITION pos = all_thread_data().get_start_position();
//   while( pos != NULL)
//   {
//
//      int iThreadId;
//
//      ThreadLocalData * pdata;
//
//      allthreaddata->get_next_assoc(pos,iThreadId,pdata);
//
//      if(natural(pdata->get_count()) > dwIndex)
//      {
//         pdata->element_at(dwIndex) = nullptr;
//      }
//   }
//
//   return true;
//}

//
//LPVOID WINAPI tls_get_value(DWORD dwIndex)
//{
//   ThreadLocalData* threadData = currentThreadData;
//
//   if (threadData && natural(threadData->get_count()) > dwIndex)
//   {
//      // Return the value of an allocated TLS slot.
//      return threadData->element_at(dwIndex);
//   }
//   else
//   {
//      // Default value for unallocated slots.
//      return nullptr;
//   }
//}
//
//LPVOID WINAPI tls_get_value(IDTHREAD iThreadId,DWORD dwIndex)
//{
//   ThreadLocalData* threadData = all_thread_data()[iThreadId];
//
//   if (threadData && natural(threadData->get_count()) > dwIndex)
//   {
//      // Return the value of an allocated TLS slot.
//      return threadData->element_at(dwIndex);
//   }
//   else
//   {
//      // Default value for unallocated slots.
//      return nullptr;
//   }
//}
//
//
//BOOL WINAPI tls_set_value(DWORD dwIndex, LPVOID lpTlsValue)
//{
//   ThreadLocalData* threadData = currentThreadData;
//
//   if (!threadData)
//   {
//      // First time allocation of TLS data for this thread.
//      try
//      {
//         threadData = new ThreadLocalData;
//
//         synch_lock lock(g_pmutexTlsData);
//
//         int iThreadId = ::GetCurrentThreadId();
//
//         all_thread_data().set_at(iThreadId,threadData);
//
//         currentThreadData = threadData;
//
//      }
//      catch (...)
//      {
//         if (threadData)
//            delete threadData;
//
//         return false;
//      }
//   }
//
//   // Store the new value for this slot.
//   threadData->set_at_grow(dwIndex, lpTlsValue);
//
//   return true;
//}
//
//BOOL WINAPI tls_set_value(int iThreadId,DWORD dwIndex,LPVOID lpTlsValue)
//{
//
//   ThreadLocalData* threadData = all_thread_data()[iThreadId];
//
//   if (!threadData)
//   {
//      // First time allocation of TLS data for this thread.
//      try
//      {
//         threadData = new ThreadLocalData;
//
//         synch_lock lock(g_pmutexTlsData);
//
//         all_thread_data().set_at(iThreadId,threadData);
//
//         //currentThreadData = threadData;
//      }
//      catch (...)
//      {
//         if (threadData)
//            delete threadData;
//
//         return false;
//      }
//   }
//
//   // Store the new value for this slot.
//   threadData->set_at_grow(dwIndex, lpTlsValue);
//
//   return true;
//}
//
//
//// Called at thread exit to clean up TLS allocations.
//void WINAPI tls_shutdown()
//{
//
//   ThreadLocalData * threadData = currentThreadData;
//
//   if (threadData)
//   {
//
//      //try
//      //{
//
//      //   IDWriteFactory * pfactory = TlsGetWriteFactory();
//
//      //   if(pfactory != NULL)
//      //   {
//
//      //      pfactory->Release();
//
//      //   }
//
//      //}
//      //catch(...)
//      //{
//      //}
//
//      //try
//      //{
//
//      //   ID2D1Factory1 * pfactory = GetD2D1Factory1();
//
//      //   if(pfactory != NULL)
//      //   {
//
//      //      //pfactory->Release();
//
//      //   }
//
//      //}
//      //catch(...)
//      //{
//      //}
//
//
//
//      synch_lock ml(g_pmutexTlsData);
//
//      int iThreadId = ::GetCurrentThreadId();
//
//      all_thread_data().remove_key(iThreadId);
//
//      currentThreadData = nullptr;
//
//      delete threadData;
//   }
//}
//


//int WINAPI __GetThreadPriority(_In_ HTHREAD hThread)
//{
//
//   synch_lock lock(g_pmutexPendingThreadsLock);
//
//   // Look up the requested thread.
//   auto threadInfo = pendingThreads().PLookup(hThread);
//
//   if (threadInfo == NULL)
//   {
//
//      return 0x80000000;
//
//   }
//
//   return threadInfo->m_element2.m_iPriority;
//
//}
//
//
//
//

//hthread::hthread(uint32_t(* pfn)(void *),void * pv)
//{
//
//   m_pfn    = pfn;
//   m_pv     = pv;
//   m_bRun   = true;
//
//
//   synch_lock ml(s_pmutex);
//
//   s_pptra->add(this);
//
//}





HTHREAD start_thread(uint32_t ( * pfn)(void *), void * pv, int iPriority)
{

   UNREFERENCED_PARAMETER(iPriority);

   return create_thread(NULL, 0, pfn, pv, 0, NULL);

}




//thread_layer::~thread_layer()
//{
//
//
//}
//
//
//int thread_layer::run()
//{
//
//   throw "todo"; // message pumping
//
//   /*
//   MSG msg;
//
//   while(true)
//   {
//
//   if(!PeekMessage(&msg, NULL, 0, 0xffffffffu, TRUE))
//   {
//
//   if(!on_idle())
//   {
//
//   Sleep(m_iSleepiness);
//
//   }
//
//
//   continue;
//
//   }
//
//
//   }
//
//   return m_iResult;*/
//
//   return -1;
//
//}

//bool thread_layer::on_idle()
//{
//
//   return false;
//
//}

//void thread_layer::wait_thread(uint32_t dwMillis)
//{
//
//   ::WaitForSingleObjectEx(m_hthread, dwMillis, FALSE);
//
//}

//HTHREAD g_hMainThread = NULL;
//UINT g_uiMainThread = -1;
//
//CLASS_DECL_AURA void set_main_thread(HTHREAD hThread)
//{
//
//   //   MSG msg;
//
//   throw "todo"; // PeekMessage function used to create message queue Windows Desktop
//
//   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);
//
//   g_hMainThread = hThread;
//
//}

//void set_main_thread_id(UINT uiThread)
//{
//
//   //   MSG msg;
//
//   throw "todo"; // PeekMessage function used to create message queue Windows Desktop
//
//   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);
//
//   g_uiMainThread = uiThread;
//
//}


//HTHREAD get_main_thread()
//{
//   return g_hMainThread;
//
//}
//UINT   get_main_thread_id()
//{
//   return g_uiMainThread;
//}
//

void attach_thread_input_to_main_thread(int_bool bAttach)
{
   return;
   //   MSG msg;

   // metrowin todo
   throw "todo"; // PeekMessage function used to create message queue Windows Desktop

   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);

   //   AttachThreadInput(::GetCurrentThreadId(), get_main_thread_id(), bAttach ? TRUE : FALSE); // AttachThreadInput function used to attach thread input to main thread in Windows Desktop

}

//uint32_t thread_layer::proc(void * pv)
//{
//
//   thread_layer * player   = (thread_layer *) pv;
//
//   player->m_hthread       = ::get_current_thread();
//
//   player->m_nId           = ::get_current_thread_id();
//
//   return player->run();
//
//}


//mq * __get_mq()
//{
//
//   mq * pmq = (mq *) tls_get_value(TLS_MESSAGE_QUEUE);
//
//   if(pmq != NULL)
//      return pmq;
//
//   pmq = new mq();
//
//   tls_set_value(TLS_MESSAGE_QUEUE, pmq);
//
//   return pmq;
//
//}
//
////bool is_thread(HTHREAD h)
////{
////   return get_thread_id(h) != 0;
////}
//
//mq * __get_mq(IDTHREAD iThreadId)
//{
//
//
//   mq * pmq = (mq *)tls_get_value(iThreadId,TLS_MESSAGE_QUEUE);
//
//   if(pmq != NULL)
//      return pmq;
//
//   pmq = new mq();
//
//   tls_set_value(iThreadId,TLS_MESSAGE_QUEUE,pmq);
//
//   return pmq;
//
//}


//WINBOOL WINAPI GetMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax)
//{
//
//   mq * pmq = __get_mq();
//
//   if(pmq == NULL)
//      return FALSE;
//
//   single_lock ml(&pmq->m_mutex, false);
//
//   if(wMsgFilterMax == 0)
//      wMsgFilterMax = (UINT) -1;
//
//   pmq->m_eventNewMessage.ResetEvent();
//
//   goto restart;
//
//}
//
//
//BOOL WINAPI PeekMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
//{
//
//   mq * pmq = __get_mq();
//
//   if(pmq == NULL)
//      return FALSE;
//
//   synch_lock ml(&pmq->m_mutex);
//
//   if(wMsgFilterMax == 0)
//      wMsgFilterMax = (UINT) -1;
//
//   for(int i = 0; i < pmq->ma.get_count(); i++)
//   {
//      MESSAGE & msg = pmq->ma[i];
//
//      if((oswindow == NULL || msg.hwnd == oswindow) && msg.message >= wMsgFilterMin && msg.message <= wMsgFilterMax)
//      {
//         *lpMsg = msg;
//         if(wRemoveMsg & PM_REMOVE)
//         {
//            pmq->ma.remove_at(i);
//         }
//         return TRUE;
//      }
//   }
//
//   return FALSE;
//}



//DWORD WINAPI get_thread_id(HTHREAD Thread)
//{
//
//   synch_lock mlThreadId(g_pmutexThreadIdLock);
//
//   auto p = thread_id_map().PLookup(Thread);
//
//   if(p == NULL)
//      return NULL;
//
//
//   return p->m_element2;
//
//}

void _on_aura_thread_attach()
{
   
   //synch_lock mlThreadId(g_pmutexThreadIdLock);

   //HTHREAD hthread = ::GetCurrentThread();

   //DWORD dwId = ::GetCurrentThreadId();
   //
   //thread_id_map()[hthread] = dwId;

}
void _on_aura_thread_detach()
{
   
   //synch_lock mlThreadId(g_pmutexThreadIdLock);
   //
   //HTHREAD hthread = ::GetCurrentThread();

   //thread_id_map().remove_key(hthread);

}

//DWORD WINAPI GetThreadId(HTHREAD Thread)
//{
//   
//   return get_thread_id(Thread);
//
//}


//HTHREAD WINAPI get_thread_handle(DWORD dw)
//{
//
//   synch_lock mlThreadIdHandle(g_pmutexThreadIdHandleLock);
//
//   auto p = thread_id_handle_map().PLookup(dw);
//
//   if(p == NULL)
//      return NULL;
//
//
//   return p->m_element2;
//
//}


BOOL WINAPI PostThreadMessageW(IDTHREAD iThreadId,UINT message,WPARAM wparam,LPARAM lparam)
{

   //HTHREAD h = ::get_thread_handle(idThread);

   //if(h == NULL)
     // return FALSE;


   mq * pmq = __get_mq(iThreadId, message != WM_QUIT);

   if(pmq == NULL)
      return FALSE;

   synch_lock ml(&pmq->m_mutex);

   MESSAGE msg;

   msg.hwnd       = NULL;
   msg.message    = message;
   msg.wParam     = wparam;
   msg.lParam     = lparam;
   msg.pt.x       = 0x80000000;
   msg.pt.y       = 0x80000000;
   msg.time       = -1;

   pmq->ma.add(msg);

   pmq->m_eventNewMessage.set_event();

   return true;

}




namespace core
{

///  \brief		global function to set thread priority for current thread
	///  \param		new priority
	bool set_thread_priority(int32_t priority)
	{
      return ( ::SetThreadPriority(::get_current_thread(), priority) != 0 );

	}


	///  \brief		global function to get thread priority for current thread
	///  \return	priority of current thread

	int32_t thread_priority()
	{

      return ::GetThreadPriority(::get_current_thread());

   }


} // namespace core





bool on_init_thread()
{

   if(!__os_init_thread())
   {

      return false;

   }

   return true;

}


bool on_term_thread()
{

   bool bOk1 = __os_term_thread();

   return bOk1;

}





void __node_init_multithreading()
{

   __node_init_cross_windows_threading();

}



void __node_term_multithreading()
{

   __node_term_cross_windows_threading();

}




//thread_int_ptr < HRESULT > t_hresultCoInitialize;

//CLASS_DECL_AURA void __clear_mq();

bool __os_init_thread()
{

   //__clear_mq();

   //if(!defer_co_initialize_ex())
     // return false;

   return true;

}

//mutex * g_pmutexMq = NULL;
//
//map < HTHREAD,HTHREAD,mq *,mq * > * g_pmapMq = NULL;
//
//void __clear_mq()
//{
//
//   synch_lock sl(g_pmutexMq);
//
//   HTHREAD hthread = GetCurrentThread();
//
//   auto pmq = (mq *)tls_get_value(hthread,TLS_MESSAGE_QUEUE);
//
//   if(pmq == NULL)
//      return;
//
//   ::aura::del(pmq);
//
//   tls_set_value(hthread,TLS_MESSAGE_QUEUE,NULL);
//
//}
//
//

bool __os_term_thread()
{

   __clear_mq();

   thread_shutdown();

   if(SUCCEEDED(t_hresultCoInitialize))
   {

      CoUninitialize();

   }

   return true;

}



















































//DWORD MsgWaitForMultipleObjects(DWORD dwSize,const HANDLE * lphandles,DWORD dwTimeout,DWORD dwWakeMask,DWORD dwFlags)
//{
//
//   HANDLE * ph = new HANDLE[dwSize + 1];
//
//   memcpy(ph,lphandles,sizeof(HANDLE) *dwSize);
//
//   ph[dwSize] = (HANDLE)__get_mq()->m_eventNewMessage.m_object;
//
//   DWORD r = ::WaitForMultipleObjectsEx(dwSize + 1,lphandles,dwFlags & MWMO_WAITALL,dwTimeout,dwWakeMask & MWMO_ALERTABLE);
//
//   delete ph;
//
//   return r;
//
//}






DWORD WinMsgWaitForMultipleObjects(DWORD dwSize,const HANDLE * lphandles,DWORD dwTimeout,DWORD dwWakeMask,DWORD dwFlags)
{

   HANDLE * ph = new HANDLE[dwSize + 1];

   memcpy(ph,lphandles,sizeof(HANDLE) *dwSize);

   ph[dwSize] = (HANDLE) __get_mq()->m_eventNewMessage.m_object;

   DWORD r = ::WaitForMultipleObjectsEx(dwSize + 1,ph,dwFlags & MWMO_WAITALL,dwTimeout,dwWakeMask & MWMO_ALERTABLE);

   delete ph;

   return r;

}





void _on_os_hthread_end()
{

}



IDTHREAD get_current_thread_id()
{

   return ::GetCurrentThreadId();

}


HTHREAD get_current_thread()
{

   return ::GetCurrentThread();

}


HTHREAD g_hMainThread = NULL;
UINT g_uiMainThread = -1;
//
void set_main_thread(HTHREAD hThread)
{
   //
   //   //   MSG msg;
   //
   //   throw "todo"; // PeekMessage function used to create message queue Windows Desktop
   //
   //   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);
   //
   g_hMainThread = hThread;

}
//
void set_main_thread_id(UINT uiThread)
{

   //   //   MSG msg;
   //
   //   throw "todo"; // PeekMessage function used to create message queue Windows Desktop
   //
   //   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);

   g_uiMainThread = uiThread;

}
//
//
HTHREAD get_main_thread()
{
   return g_hMainThread;

}
UINT   get_main_thread_id()
{
   return g_uiMainThread;
}


