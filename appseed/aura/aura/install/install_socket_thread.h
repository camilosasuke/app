#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class service;
   class socket;


   class CLASS_DECL_AURA socket_thread :
      virtual public ::netserver::socket_thread
   {
   public:


      socket_thread(::aura::application * papp);
      ~socket_thread();


      void install_message_handling(::message::dispatch * pinterface);

      void setup_socket(::sockets::listen_socket<socket> & ll);

      virtual ::netserver::socket_handler * create_socket_handler();
      virtual ::sockets::listen_socket_base * create_listen_socket();


   };


} // namespace install



#endif


