#pragma once

class pred_holder_base :
   virtual public object
{
public:

   sp(object) m_pholdref;

   pred_holder_base(::aura::application * papp, sp(object) pholdref = NULL) :
      object(papp),
      m_pholdref(pholdref)
   {

   }

   virtual void run() {}

};

template < typename PRED >
class pred_holder :
   virtual public pred_holder_base
{
public:

   PRED m_pred;

   pred_holder(::aura::application * papp, PRED pred) :
      object(papp),
      pred_holder_base(papp),
      m_pred(pred)
   {

   }

   pred_holder(::aura::application * papp, sp(object) pholdref, PRED pred) :
      object(papp),
      pred_holder_base(papp, pholdref),
      m_pred(pred)
   {

   }

   virtual void run() { m_pred(); }

};
//class replace_thread;

class user_interaction_ptr_array;

class thread_startup;

namespace primitive { class command;  }

///
/// \author Camilo Sasuke Tsumanuma
///
class CLASS_DECL_AURA thread :
   virtual public command_target
#ifdef WINDOWS
   ,virtual public ::exception::translator
#endif
{
private:
   
   bool                                   m_bRunThisThread;

public:

   class CLASS_DECL_AURA file_info
   {
   public:


      file_info();
      ~file_info();

      ::duration                             m_durationFileSharingViolationTimeout;

   };

   //::duration                             m_durationRunLock;

   user_interaction_ptr_array *           m_puiptra;

   single_lock *                          m_pslUser;
   static bool                            s_bAllocReady;
   //mutex *                                m_pmutex;

   //thread_impl_sp                         m_pthreadimpl;

   bool                                   m_bAutoDelete;       // enables 'delete this' after thread termination
   uint_ptr                               m_dwAlive;
   bool                                   m_bReady;
   int32_t                                m_iReturnCode;
   ::user::primitive *                  m_puiMain;           // main interaction_impl (usually same System.m_puiMain)
   ::user::primitive *                  m_puiActive;         // active main interaction_impl (may not be m_puiMain)
   //property_set                           m_set;
   string                                 m_strWorkUrl;
   ref_array < thread >                   m_threadptraDependent;
   ref_array < thread >                   m_threadptraRequired;


   bool                                   m_bZipIsDir;

//   replace_thread *                       m_preplacethread;

   //manual_reset_event *                   m_peventEvent;

   file_info *                            m_pfileinfo;


   #ifndef WINDOWSEX

   mq *                                      m_mq;

#endif

   //thread *                                  m_pthread;

   //sp(ref_array < ::user::primitive >)     m_spuiptra;

   bool                                      m_bDupHandle;
   HTHREAD                                   m_hthread;
   IDTHREAD                                  m_uiThread;

   list < ::user::frame * >           m_frameList;

   LPVOID                                    m_pThreadParams;
   __THREADPROC                              m_pfnThreadProc;

   manual_reset_event *                      m_pevReady;
   UINT                                      m_nDisablePumpCount;
   mutex                                     m_mutexUiPtra;

   UINT                                      m_dwFinishTimeout;

   string                                    m_strDebug;
   string                                    m_strFile;
   int                                       m_iLine;

   sp(::command_thread)                      m_pcommandthread;



   thread();
   thread(::aura::application * papp);
   thread(::aura::application * papp, __THREADPROC pfnThreadProc, LPVOID pParam);
   virtual ~thread();


   // file related stuff
   file_info * get_file_info();
   DWORD get_file_sharing_violation_timeout_total_milliseconds();
   ::duration set_file_sharing_violation_timeout(::duration duration);


   ///  \brief		starts thread on first call
   virtual void start();

   virtual void * get_os_data() const;
   virtual IDTHREAD get_os_int() const;


   virtual HTHREAD get_os_handle() const;


   virtual void set_os_data(void * pvoidOsData);
   virtual void set_os_int(IDTHREAD iData);


   friend bool __internal_pre_translate_message(MESSAGE * pMsg);


   void CommonConstruct();


   virtual bool pre_run();


   virtual bool finalize();

   virtual int32_t exit();


   virtual void on_keep_alive();
   virtual bool is_alive();


   virtual bool has_message();

   virtual int get_x_window_count() const;

   virtual wait_result wait(const duration & duration);

   void set_priority(int32_t priority);

   int32_t priority();


   virtual bool is_auto_delete();

   virtual bool begin(int32_t epriority = ::multithreading::priority_normal, uint_ptr nStackSize = 0, uint32_t dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

   virtual bool create_thread(int32_t epriority = ::multithreading::priority_normal,uint_ptr nStackSize = 0,uint32_t dwCreateFlags = 0,LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

   virtual bool begin_synch(int32_t *piStartupError = NULL, int32_t epriority = ::multithreading::priority_normal,uint_ptr nStackSize = 0,uint32_t dwCreateFlags = 0,LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

   virtual bool create_thread_synch(int32_t *piStartupError,int32_t epriority = ::multithreading::priority_normal,uint_ptr nStackSize = 0,uint32_t dwCreateFlags = 0,LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);


   virtual int32_t get_thread_priority();
   virtual bool set_thread_priority(int32_t epriority);

   virtual uint32_t ResumeThread();
   virtual bool post_message(UINT message, WPARAM wParam = 0, lparam lParam = 0);
   virtual bool send_message(UINT message,WPARAM wParam = 0,lparam lParam = 0, ::duration durWaitStep = millis(1));
   virtual bool post_object(UINT message, WPARAM wParam, lparam lParam);
   virtual bool send_object(UINT message, WPARAM wParam, lparam lParam, ::duration durWaitStep = millis(1));
   virtual bool post_message(::user::primitive * pui, UINT message, WPARAM wParam = 0, lparam lParam = 0);

   template < typename PRED >
   bool post_pred(sp(object) phold, PRED pred)
   {
      return post_thread_message(message_system, system_message_pred, dynamic_cast < pred_holder_base *>(canew(pred_holder < PRED >(get_app(), phold, pred))));
   }

   template < typename PRED >
   bool post_pred(PRED pred)
   {
      return post_thread_message(message_system, system_message_pred, dynamic_cast < pred_holder_base *>(canew(pred_holder < PRED >(get_app(), pred))));
   }

   virtual bool pre_init_instance();

   virtual bool on_run_exception(::exception::exception &);

   virtual bool initialize_instance();

   virtual message::e_prototype GetMessagePrototype(UINT uiMessage, UINT uiCode);

   // running and idle processing
   virtual int32_t run();
   virtual void pre_translate_message(signal_details * pobj);
   virtual bool pump_message();     // low level message pump
   virtual bool defer_pump_message();     // deferred message pump
   virtual bool process_message(LPMESSAGE lpmessage);     // route message
   // virtual bool on_idle(LONG lCount); // return TRUE if more idle processing
   virtual bool on_thread_on_idle(::thread * pthread, LONG lCount);
   virtual bool is_idle_message(signal_details * pobj);  // checks for special messages
   virtual bool is_idle_message(LPMESSAGE lpmessage);  // checks for special messages

   virtual int32_t exit_instance();

   virtual void process_window_procedure_exception(::exception::base*,signal_details * pobj);

   virtual void process_message_filter(int32_t code, signal_details * pobj);

   // virtual void add(::user::primitive * pui);
   //virtual void remove(::user::primitive * pui);
   //virtual ::count get_ui_count();
   //virtual ::user::primitive * get_ui(index iIndex);
   //virtual void set_timer(::user::primitive * pui, uint_ptr nIDEvent, UINT nEllapse);
   //virtual void unset_timer(::user::primitive * pui, uint_ptr nIDEvent);
   virtual void set_auto_delete(bool bAutoDelete = true);
   virtual ::user::primitive * get_active_ui();
   virtual ::user::primitive * set_active_ui(::user::primitive * pui);
   //virtual void step_timer();
   //virtual bool on_run_step();


   virtual void assert_valid() const;
   virtual void dump(dump_context & dumpcontext) const;
   virtual void Delete();
   // 'delete this' only if m_bAutoDelete == TRUE

   virtual void dispatch_thread_message(signal_details * pobj);  // helper

   virtual int32_t main();





   virtual void wait();


   virtual int_ptr item() const;



   virtual bool verb();


   virtual void post_to_all_threads(UINT message,WPARAM wparam,LPARAM lparam);


   virtual void register_dependent_thread(::thread * pthread);
   virtual void unregister_dependent_thread(::thread * pthread);
   virtual void on_register_dependent_thread(::thread * pthread);
   virtual void on_unregister_dependent_thread(::thread * pthread);
   virtual void signal_close_dependent_threads();
   virtual void wait_close_dependent_threads(const duration & duration);
   virtual void register_at_required_threads();
   virtual void unregister_from_required_threads();

   virtual void do_events();
   virtual void do_events(const duration & duration);

   virtual bool get_run_thread();
   virtual bool should_enable_thread();
   virtual bool post_quit();

   virtual bool kick_thread();

   //virtual void defer_add_thread_run_wait(sync_object_ptra & soa);

   virtual void message_queue_message_handler(::signal_details * pobj);

   //DECL_GEN_SIGNAL(_001OnSendThreadMessage);
   DECL_GEN_SIGNAL(_001OnThreadMessage);


   virtual void shutdown(bool bPrompt = true);

   virtual bool on_before_shutdown();

   virtual bool is_application();
   virtual bool is_session();
   virtual bool is_system();

   virtual void delete_this();

   /// thread implementation
   virtual int32_t thread_startup(::thread_startup * pstartup);
   virtual bool thread_entry();
   virtual int32_t thread_term();
   virtual void thread_delete();
   operator HTHREAD() const;
   void construct();
   void construct(__THREADPROC pfnthread_implProc, LPVOID pParam);
   virtual bool begin_thread(bool bSynch = false,int32_t * piStartupError = NULL,int32_t epriority= ::multithreading::priority_normal,uint_ptr nStackSize = 0,uint32_t dwCreateFlagsParam = 0,LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
   //virtual int_ptr item() const;
   virtual bool initialize_message_queue();
   virtual void message_handler(signal_details * pobj);

   // Wall-eeeeee aliases
   ::command_thread * command_central();
   ::command_thread * command_thread();
   ::command_thread * guideline();
   ::command_thread * command();
   ::command_thread * directrix();
   ::command_thread * axiom();
   ::command_thread * creation();

   virtual void on_command(::primitive::command * pcommand);

   virtual void on_create(::create * pcreate);

};


//
//class CLASS_DECL_AURA replace_thread
//{
//public:
//
//   ::thread *                    m_pthread;
//   ::thread *                    m_pthreadNew;
//   mutex &                       m_mutex;
//
//   replace_thread(mutex & m): m_mutex(m) {  }
//
//   void replace(::thread * pcanew);
//
//   bool do_replace(::thread * pthread);
//
//};



namespace aura
{

   
   CLASS_DECL_AURA bool post_quit_thread();
   CLASS_DECL_AURA bool post_quit_thread(::thread * pthread);


} // namespace aura