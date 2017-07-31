#pragma once


#ifdef APPLEOS
typedef int key_t;
#elif defined(LINUX)
typedef __key_t key_t;
#elif defined(ANDROID)
#else
//#include "ca_mutex.h"
#endif

namespace aura
{


   namespace ipc
   {


      class CLASS_DECL_AURA base :
         virtual public object
      {
      public:

#ifdef METROWIN

         //int32_t              m_iSerial;

#elif defined(WINDOWSEX)


         oswindow             m_oswindow;


#elif defined(APPLEOS)


         CFMessagePortRef     m_port;


#elif !defined(METROWIN)

         key_t                m_key;
         int                  m_iQueue;

         struct data_struct
         {


            long     mtype;
            long     request;
            int      size;
            char     data[0];


         };


#endif


         string   m_strBaseChannel;


         base(::aura::application * papp);
         virtual ~base();


      };




      class CLASS_DECL_AURA tx:
         virtual public  base
      {
      public:


         tx(::aura::application * papp);
         virtual ~tx();



#if defined(METROWIN)
         bool open(const char * pszChannel);
#else
         bool open(const char * pszChannel, launcher * plauncher = NULL);
#endif
         bool close();


         bool send(const char * pszMessage,unsigned int dwTimeout);
         bool send(int message,void * pdata,int len,unsigned int dwTimeout);


         bool is_tx_ok();

      };


      class rx_private;


      class CLASS_DECL_AURA rx:
         virtual public base
      {
      public:


         class CLASS_DECL_AURA receiver
         {
         public:

            virtual void on_receive(rx * prx,const char * pszMessage);
            virtual void on_receive(rx * prx,int message,void * pdata,memory_size_t len);
            virtual void on_post(rx * prx,long long int a,long long int b);

         };

         receiver *           m_preceiver;
         rx_private *         m_pp;

#ifndef WINDOWS

         bool                 m_bRunning;
         bool                 m_bRun;
         thread *             m_pthread;

#endif


         rx(::aura::application * papp);
         virtual ~rx();


         bool create(const char * pszChannel);
         bool destroy();


         virtual void * on_receive(rx * prx,const char * pszMessage);
         virtual void * on_receive(rx * prx,int message,void * pdata,memory_size_t len);
         virtual void * on_post(rx * prx,long long int a,long long int b);


         virtual bool on_idle();


#ifdef WINDOWSEX
         //ATOM register_class(HINSTANCE hInstance);
         //static LRESULT CALLBACK s_message_queue_proc(oswindow oswindow,UINT message,WPARAM wParam,LPARAM lParam);
         LRESULT message_queue_proc(UINT message,WPARAM wParam,LPARAM lParam);
#else

         bool start_receiving();

         void * receive();

#endif

         bool is_rx_ok();

      };




      class CLASS_DECL_AURA ipc:
         virtual public tx,
         virtual public rx::receiver
      {
      public:


         rx                      m_rx;
         string                  m_strChannel;
         unsigned int            m_dwTimeout;


         ipc(::aura::application * papp);
         virtual ~ipc();

#if defined(METROWIN)
         bool open_ab(const char * pszChannel, const char * pszModule);
         bool open_ba(const char * pszChannel, const char * pszModule);
#elif defined(WINDOWS)
         bool open_ab(const char * pszChannel,const char * pszModule,launcher * plauncher = NULL);
         bool open_ba(const char * pszChannel,const char * pszModule,launcher * plauncher = NULL);
#else
         bool open_ab(const char * pszChannel, launcher * plauncher = NULL);
         bool open_ba(const char * pszChannel, launcher * plauncher = NULL);
#endif
         bool close();

         virtual void restart_aura_ipc();

         bool ensure_tx(const char * pszMessage,unsigned int dwTimeout = INFINITE);
         bool ensure_tx(int message,void * pdata,int len,unsigned int dwTimeout = INFINITE);


         bool is_rx_tx_ok();


      };






      CLASS_DECL_AURA string app_install(string strPlatform = "");


   } // namespace ipc


} // namespace aura




