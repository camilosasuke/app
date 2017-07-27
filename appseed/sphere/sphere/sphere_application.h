#pragma once


namespace sphere
{


   class CLASS_DECL_SPHERE application : 
      virtual public ::console::application
   {
   public:
      
      


      application();
      application(const char * pszId);
      virtual ~application();


      using ::asphere::application::construct;
      virtual void construct(const char * pszId);
      
      virtual bool initialize_application();
      virtual int32_t  exit_application() override;

      virtual void _001OnFileNew();
      virtual bool bergedge_start();


      virtual bool is_serviceable();
      virtual service_base * allocate_new_service();


      virtual bool on_install();
      virtual bool on_uninstall();

      virtual int32_t run();

      virtual void on_request(::create * pcreate);

      ::user::document * _001OpenDocumentFile(var varFile);

      sp(::aura::application) get_system();

   };

   
} // namespace sphere



