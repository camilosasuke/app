#pragma once


#include "netserver_socket_handler.h"


namespace netserver
{



   class CLASS_DECL_AURA socket_thread :
      virtual public ::thread
   {
   public:



      string                                       m_strIp;
      int32_t                                      m_iPort;
      int                                          m_iSsl;

      SOCKET                                       m_hsocket;
      string                                       m_strCat;
      string                                       m_strCipherList;

      sp(::netserver::socket_handler)              m_psockethandler;
      sp(::sockets::listen_socket_base)            m_plistensocket;


      socket_thread(::aura::application * papp);
      ~socket_thread();


      virtual int32_t run() override;

      virtual ::netserver::socket_handler * create_socket_handler();
      virtual ::sockets::listen_socket_base * create_listen_socket();

      virtual bool initialize_listen_socket();

      ::netserver::socket_handler * socket_handler();


   };


} // namespace netserver




