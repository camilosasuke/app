#pragma once


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   class CLASS_DECL_AURA trace :
      virtual public object,
      virtual public progress_listener
   {
   public:



      stringa              m_stra;
      HANDLE               m_hfile;
      string               m_strLastStatus;
      string               m_strLastGlsStatus;
      int32_t              m_iLastStatus;
      int32_t              m_iLastGlsStatus;
      
      
      trace(::aura::application * papp);
      virtual ~trace();

      
      bool initialize();

      void finalize();

      void ensure_trace_file();

      virtual void on_trace(string & str, string & str2);

      virtual void rich_trace(const char * psz);

      virtual void trace_add(const char * psz);

      virtual void trace_progress(double dRate);

      virtual void on_progress(double dRate);

      virtual bool isspace_dup(char ch);


   };



      class CLASS_DECL_AURA trace_file :
         virtual public ::object
      {
      public:


         string            m_strLabel;
         ::mutex           m_mutex;
         synch_lock        m_sl;

         ::file::file_sp   m_pfile;


         trace_file(::aura::application * papp, const string & strLabel);
         ~trace_file();


         void print(const string & str);
         void print(double dRate);


      };



} // namespace install


#endif // INSTALL_SUBSYSTEM




