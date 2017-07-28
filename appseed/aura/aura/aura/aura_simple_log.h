#pragma once



class CLASS_DECL_AURA simple_log :
   virtual public object
{
public:


   simple_log *         m_psimplelog;
   int                  m_iLogTarget;


   simple_log();
   simple_log(simple_log * psimplelog, int iLogTarget);
   virtual ~simple_log();


   virtual void log_line(string str, int iLogTarget);
   virtual void log_line(string str);


};

