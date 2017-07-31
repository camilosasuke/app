#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class application;
   class service;


   class CLASS_DECL_AURA socket :
      virtual public ::netserver::socket
   {
   public:


      bootstrap *          m_pbootstrap;


      socket(::sockets::base_socket_handler & h);
      virtual ~socket();

      virtual void on_send_response() override;


   };


} // namespace install


#endif


