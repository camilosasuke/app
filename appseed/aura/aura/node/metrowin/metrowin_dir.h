#pragma once


namespace metrowin
{


   class CLASS_DECL_AURA dir:
      virtual public ::file::dir::system
   {
   public:


      ::file::path         m_strCa2;
      ::file::path         m_strTimeFolder;
      ::file::path         m_strNetSeedFolder;


      dir(::aura::application *  papp);


      //using ::file::dir::system::path;
      //virtual class ::file::path & path();
      //virtual string path(const char * pszFolder,strsize iLenFolder,const char * pszRelative,strsize iLenRelative,const char * psz2,strsize iLen2,bool bUrl);

      //virtual string relpath(const char * lpcszSource,const char * lpcszRelative,const char * lpcsz2 = NULL);
      //virtual bool  ls_pattern(::aura::application *  papp,const char * lpcsz,const char * lpcszPattern,stringa * pstraPath = NULL,stringa * pstraTitle = NULL,bool_array * pbaIsDir = NULL,int64_array * piaSize = NULL);
      virtual ::file::listing & ls(::aura::application *  papp,::file::listing & listing);
      //virtual bool  rls_pattern(::aura::application *  papp,const char * lpcsz,const char * lpcszPattern,stringa * pstraPath = NULL,stringa * pstraTitle = NULL,stringa * pstraRelative = NULL,bool_array * pbaIsDir = NULL,int64_array * piaSize = NULL,e_extract eextract = extract_first);
      //virtual bool  rls(::aura::application *  papp,const char * lpcsz,stringa * pstraPath = NULL,stringa * pstraTitle = NULL,stringa * pstraRelative = NULL,e_extract eextract = extract_first);
      //virtual bool  rls_dir(::aura::application *  papp,const char * lpcsz,stringa * pstraPath = NULL,stringa * pstraTitle = NULL,stringa * pstraRelative = NULL);
      //virtual bool  ls_dir(::aura::application *  papp,const char * lpcsz,stringa * pstraPath = NULL,stringa * pstraTitle = NULL);
      //virtual bool  ls_file(::aura::application *  papp,const char * lpcsz,stringa * pstraPath = NULL,stringa * pstraTitle = NULL);
      virtual bool  is(const ::file::path & path,::aura::application *  papp);
      //virtual bool  is(const string & str,::aura::application *  papp);
      virtual bool  name_is(const ::file::path & str,::aura::application *  papp);
      virtual bool  has_subdir(::aura::application *  papp, const ::file::path & path);
      virtual bool  is_inside(const ::file::path & pathDir, const ::file::path & path,::aura::application *  papp);
      virtual bool  is_inside_time(const ::file::path & path,::aura::application *  papp);
      virtual ::file::listing & root_ones(::file::listing & listing,::aura::application *  papp);
      virtual bool mk(const ::file::path & path,::aura::application *  papp);
      virtual bool rm(::aura::application *  papp, const ::file::path & path, bool bRecursive = true);


      //virtual string name(const char * lpcsz);
      //virtual string name(const string & str);

      ::file::path dir::userfolder(::aura::application *  papp);

      virtual ::file::path votagus();
      virtual ::file::path time();
      virtual ::file::path stage();
      virtual ::file::path stageapp();
      virtual ::file::path netseed();

      // stage in ccvotagus spalib
      virtual ::file::path element();

      //virtual ::file::path module();
      //virtual ::file::path ca2module();
      virtual ::file::path time_square();
      virtual ::file::path time_log();

      ::file::path time_square(::aura::application * papp, const string & strPrefix, const string & strSuffix);

      virtual ::file::path trash_that_is_not_trash(const ::file::path & path);

      virtual ::file::path appdata();

      virtual ::file::path usersystemappdata(::aura::application *  papp,const string & strPrefix);
      virtual ::file::path userappdata(::aura::application *  papp);
      virtual ::file::path userdata(::aura::application *  papp);
      //virtual ::file::path userfolder(::aura::application *  papp);
      virtual ::file::path default_os_user_path_prefix(::aura::application *  papp);
      virtual ::file::path default_userappdata(::aura::application *  papp,const string & strPrefix,const string & strLogin);
      virtual ::file::path default_userdata(::aura::application *  papp,const string & strPrefix,const string & strLogin);
      virtual ::file::path default_userfolder(::aura::application *  papp,const string & strPrefix,const string & strLogin);
      virtual ::file::path userquicklaunch(::aura::application *  papp);
      virtual ::file::path userprograms(::aura::application *  papp);

      virtual ::file::path commonprograms();

      virtual ::file::path commonappdata();

      virtual bool initialize();


      virtual bool update_module_path() override;

      virtual ::file::path get_document_folder() override;

      virtual ::file::path get_music_folder() override;
      virtual ::file::path get_video_folder() override;
      virtual ::file::path get_image_folder() override;


   };


} // namespace metrowin





