#pragma once


#undef new



namespace message
{


   class CLASS_DECL_AXIS base:
      public signal_details
   {
   public:


      ::user::interaction *      m_pwnd;
      bool                       m_bConditional;
      bool                       m_bReflect;
      bool                       m_bDestroyed;

      base(::aura::application * papp,class ::signal * psignal = NULL);
      base(::aura::application * papp,::user::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult);
      base(const base & base);
      virtual ~base();



      virtual void set_lresult(LRESULT lresult);
      virtual LRESULT & get_lresult();
      virtual void set(::user::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult);
      virtual void set(::user::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam);
   protected:
      LRESULT * m_plresult;
      LRESULT  m_lresult;
   };


} // namespace message


#define new AURA_NEW
