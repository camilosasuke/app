#pragma once


class CLASS_DECL_ca2 simple_form_list_view :
   virtual public ::user::form_list
{
public:


   simple_list_header_control   m_headerctrl;


   simple_form_list_view(sp(base_application) papp);
   virtual ~simple_form_list_view();


   virtual bool pre_create_window(CREATESTRUCT& cs);


   virtual void install_message_handling(message::dispatch * pinterface);


#ifdef DEBUG
   virtual void assert_valid() const;
   virtual void dump(dump_context & dumpcontext) const;
#endif


};


