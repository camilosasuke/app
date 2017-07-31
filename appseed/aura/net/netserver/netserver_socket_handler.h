#pragma once


namespace netserver
{


   class CLASS_DECL_AURA socket_handler :
      virtual public ::sockets::socket_handler
   {
   public:


      socket_handler(::aura::application * papp);


   };


} // namespace netserver


