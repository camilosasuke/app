#pragma once


class CLASS_DECL_CORE form_view :
   virtual public ::user::form
{
public:

   
   string      m_strPath;


   form_view(::aura::application * papp);

   void on_update(::aura::impact * pSender, LPARAM lHint, object* phint);
   virtual bool BaseOnControlEvent(::user::control_event * pevent);
   DECL_GEN_SIGNAL(_001OnCreate);
   DECL_GEN_SIGNAL(_001OnTimer);
   DECL_GEN_SIGNAL(_001OnUser123);
   virtual void install_message_handling(::message::dispatch * pinterface);

};













