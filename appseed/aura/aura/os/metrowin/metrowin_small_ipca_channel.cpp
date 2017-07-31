//#include "framework.h"
//#include "metrowin.h"
//#ifndef METROWIN

// loop question http://stackoverflow.com/users/2101860/loop
// http://stackoverflow.com/questions/23709329/launch-one-metro-app-from-other-metro-app-windows-8
// souvickcse http://stackoverflow.com/users/2246798/souvickcse

namespace aura
{


   namespace ipc
   {


      extern bool (WINAPI * g_pfnChangeWindowMessageFilter)(
         _In_ UINT message,
         _In_ DWORD dwFlag);


      base::base(::aura::application * papp) :
         object(papp)
      {

      }

      base::~base()
      {
      }

      tx::tx(::aura::application * papp) :
         object(papp),
         base(papp)
      {

      }

      tx::~tx()
      {
      }

#if defined(INSTALL_SUBSYSTEM)

      bool tx::open(const char * pszChannel, launcher * plauncher)
      {

         UNREFERENCED_PARAMETER(plauncher);

         if (m_strBaseChannel.has_char())
            close();

         // LaunchUri protocol is m_strBaseChannel

         m_strBaseChannel = pszChannel;

         m_strBaseChannel.replace("_", "-");
         m_strBaseChannel.replace("/", "-");

         return true;

      }

#else

      bool tx::open(const char * pszChannel)
      {

         if (m_strBaseChannel.has_char())
            close();

         // LaunchUri protocol is m_strBaseChannel

         m_strBaseChannel = pszChannel;

         m_strBaseChannel.replace("_", "-");
         m_strBaseChannel.replace("/", "-");

         return true;

   }


#endif

      bool tx::close()
      {

         if (m_strBaseChannel.is_empty())
            return true;

         m_strBaseChannel.Empty();

         return true;

      }


      bool tx::send(const char * pszMessage, unsigned int dwTimeout)
      {

         if (!is_tx_ok())
            return false;

         string anotherappURI = m_strBaseChannel + ":///send?message=" + ::aura::system::g_p->url_encode(pszMessage);

         Uri ^uri = ref new Uri(anotherappURI);

         ::wait(Launcher::LaunchUriAsync(uri), dwTimeout);

         return true;

      }


      bool tx::is_tx_ok()
      {

         //return ::IsWindow(m_hwnd) != FALSE;

         return m_strBaseChannel.has_char();


      }


      bool tx::send(int message, void * pdata, int len, unsigned int dwTimeout)
      {

         if (!is_tx_ok())
            return false;

         memory m;

         

         string anotherappURI = m_strBaseChannel + "://send?messagebin=" + ::str::from(message)+ "," + ::aura::system::g_p->url_encode(::aura::system::g_p->base64().encode((byte *)pdata, len));

         Uri ^uri = ref new Uri(anotherappURI);

         ::wait(Launcher::LaunchUriAsync(uri), dwTimeout);

         return true;

      }


      rx::rx(::aura::application * papp) :
         object(papp),
         base(papp)
      {

         m_preceiver = NULL;

      }


      rx::~rx()
      {

      }


      bool rx::create(const char * pszChannel)
      {

         if (m_strBaseChannel.has_char())
            destroy();

         m_strBaseChannel = pszChannel;

         m_strBaseChannel.replace("_", "-");
         m_strBaseChannel.replace("/", "-");


         return true;

      }


      bool rx::destroy()
      {

         if (m_strBaseChannel.is_empty())
         {

            return true;

         }


         m_strBaseChannel.Empty();

         return true;

      }

      void rx::receiver::on_receive(rx * prx, const char * pszMessage)
      {

      }

      void rx::receiver::on_receive(rx * prx, int message, void * pdata, memory_size_t len)
      {

      }


      void rx::receiver::on_post(rx * prx, long long int a, long long int b)
      {

      }


      void * rx::on_receive(rx * prx, const char * pszMessage)
      {

         if (m_preceiver != NULL)
         {
            m_preceiver->on_receive(prx, pszMessage);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }

      void * rx::on_receive(rx * prx, int message, void * pdata, memory_size_t len)
      {

         if (m_preceiver != NULL)
         {
            m_preceiver->on_receive(prx, message, pdata, len);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }


      void * rx::on_post(rx * prx, long long int a, long long int b)
      {

         if (m_preceiver != NULL)
         {
            m_preceiver->on_post(prx, a, b);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }



      bool rx::on_idle()
      {

         return false;

      }

      bool rx::is_rx_ok()
      {

         return m_strBaseChannel.has_char();

      }

      
      ipc::ipc(::aura::application * papp) :
         object(papp),
         base(papp),
         tx(papp),
         m_rx(papp)
      {


      }


      ipc::~ipc()
      {


      }


#ifdef INSTALL_SUBSYSTEM


      bool ipc::open_ab(const char * pszChannel, const char * pszModule, launcher * plauncher)
      {

         m_strChannel = pszChannel;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel;

         string strChannelTx = m_strChannel;

         if (!m_rx.create(strChannelRx))
         {

            return false;

         }

         if (!tx::open(strChannelTx, plauncher))
         {

            return false;

         }

         return true;

      }


      bool ipc::open_ba(const char * pszChannel, const char * pszModule, launcher * plauncher)
      {

         m_strChannel = pszChannel;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel;

         string strChannelTx = m_strChannel;

         if (!m_rx.create(strChannelRx))
         {

            return false;

         }

         if (!tx::open(strChannelTx, plauncher))
         {

            return false;

         }

         return true;

      }

#else

      bool ipc::open_ab(const char * pszChannel, const char * pszModule)
      {

         m_strChannel = pszChannel;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel;

         string strChannelTx = m_strChannel;

         if (!m_rx.create(strChannelRx))
         {

            return false;

         }

         if (!tx::open(strChannelTx))
         {

            return false;

         }

         return true;

      }


      bool ipc::open_ba(const char * pszChannel, const char * pszModule)
      {

         m_strChannel = pszChannel;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel;

         string strChannelTx = m_strChannel;

         if (!m_rx.create(strChannelRx))
         {

            return false;

         }

         if (!tx::open(strChannelTx))
         {

            return false;

         }

         return true;

      }


#endif


      bool ipc::is_rx_tx_ok()
      {

         return m_rx.is_rx_ok() && is_tx_ok();

      }


   } // namespace ipc


} // namespace aura



//#endif
