//
//  multithreading_set_event_on_exit.h
//  base
//
//  Created by Carlos Gustavo Cecyn Lundgren on 18/10/13.
//
//


class CLASS_DECL_c set_event_on_exit
{
public:
   
   
   event * m_pevent;
   
   
   set_event_on_exit(event & ev) { m_pevent = &ev; }
   ~set_event_on_exit() { if(m_pevent != NULL) m_pevent->set_event(); }
   
   
};






