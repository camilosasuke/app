#pragma once


namespace user
{


   namespace shell
   {


      class CLASS_DECL_CORE metrowin :
         virtual public ::user::shell::shell
      {
      public:

         class per_fork
         {
         public:

            //            ::windows::comptr < IImageList >                            m_pilSmall;
            //            ::windows::comptr < IImageList >                            m_pilLarge;
            //            ::windows::comptr < IImageList >                            m_pilExtraLarge;
            //            ::windows::comptr < IImageList >                            m_pilJumbo;
            //            ::windows::comptr < IMalloc >                               m_pmalloc;
            //            ::windows::comptr < IShellFolder >                          m_pfolderDesktop;
            //            ::windows::comptr < IShellFolder >                          m_pfolder;
            //            LPITEMIDLIST                                                m_lpiidl;
            string                                                      m_strFolder;

            per_fork(bool bInit = true);
            ~per_fork();

            void init();
            //void _017ItemIDListFree(LPITEMIDLIST lpiidl);

         };

         bool                                                        m_bStarted;
         delay_thread *                                              m_pdelayRelease;
         ref_array < image_key >                                     m_keyptra;
         manual_reset_event                                          m_evKey;
         mutex                                                       m_mutexQueue;
         spa(::thread)                                               m_threadaGetImage;

         metrowin(::aura::application * papp);
         virtual ~metrowin();


         //virtual void initialize() override;

         //virtual void defer_start();
         //virtual void open_folder(oswindow oswindow, const string & strFolder);
         //virtual void close_folder(const string & strFolder);

         virtual int32_t get_image_foo(oswindow oswindow, const string & strExtension, e_file_attribute eattribute, e_icon eicon, COLORREF crBk = 0) override;
         virtual int32_t get_image(oswindow oswindow, const string & strPath, e_file_attribute eattribute, e_icon eicon, COLORREF crBk = 0) override;

         virtual ::user::shell::e_folder get_folder_type(::aura::application * papp, const unichar * lpcszPath) override;
         virtual ::user::shell::e_folder get_folder_type(::aura::application * papp, const char * lpcszPath) override;

         //         int add_icon_set(per_fork * pfork, SHFILEINFOW * pinfo16, SHFILEINFOW * pinfo48, COLORREF crBk, bool & bUsedImageList16, bool & bUsedImageList48);
         //
         //         int add_icon(int iSize, HICON hicon, COLORREF crBk);
         //
         //         int add_icon_path(::file::path path, COLORREF crBk);
         //
         //         int add_icon_info(per_fork * pfork, int iSize, SHFILEINFOW * pinfo16, SHFILEINFOW * pinfo48, COLORREF crBk, bool & bUsedImageList16, bool & bUsedImageList48);
         //
         //         int add_system_icon(int iSize, IImageList * plist, SHFILEINFOW * pinfo, COLORREF crBck, bool & bUsedImageList);


         //virtual bool do_call();
         //int32_t calc_image_foo(oswindow oswindow, const string & strExtension, e_file_attribute eattribute, e_icon eicon, COLORREF crBk = 0);
         //int32_t calc_get_image(oswindow oswindow, const string & strPath, e_file_attribute eattribute, e_icon eicon, COLORREF crBk = 0);




         int32_t get_image(per_fork * pfork, oswindow oswindow, image_key key, const unichar * lpcszExtra, COLORREF crBk);
         int32_t get_image_by_extension(per_fork * pfork, oswindow oswindow, image_key & key, COLORREF crBk);
         //bool get_icon(oswindow oswindow, const char * lpcsz, const unichar * lpcszExtra, e_icon eicon, HICON * phicon16, HICON * phicon48);
         //bool get_icon(oswindow oswindow, IShellFolder * lpsf, LPITEMIDLIST lpiidlAbsolute, LPITEMIDLIST lpiidlChild, const unichar * lpcszExtra, e_icon eicon, HICON * phicon16, HICON * phicon48);
         //bool get_icon(per_fork * pfork, oswindow oswindow, LPITEMIDLIST lpiidlAbsolute, const unichar * lpcszExtra, e_icon eicon, HICON * phicon16, HICON * phicon48);
         //         int32_t get_image(oswindow oswindow, IShellFolder * lpsf, const char * pszPath, LPITEMIDLIST lpiidlChild, const unichar * lpcszExtra, e_icon eicon);
         //         int32_t get_image(per_fork * pfork, oswindow oswindow, image_key key, LPITEMIDLIST lpiidlAbsolute, LPITEMIDLIST lpiidlChild, const unichar * lpcszExtra, COLORREF crBk);
         //         //int32_t get_image(per_fork * pfork, oswindow oswindow, image_key key, LPITEMIDLIST lpiidlAbsolute, const unichar * lpcszExtra, COLORREF crBk);
         int32_t get_foo_image(per_fork * pfork, oswindow oswindow, image_key key, COLORREF crBk);



         //         ::windows::comptr < IShellFolder> _017GetShellParentFolder(per_fork * pfork, LPITEMIDLIST lpiidlChild);
         //         //::windows::comptr < IShellFolder> _017GetShellFolder(const string & str, LPITEMIDLIST lpiidlChild);
         //         ::windows::comptr < IShellFolder> _017GetShellFolder(per_fork * pfork, LPITEMIDLIST lpiidlChild);
         //         void _017ItemIDListParsePath(per_fork * pfork, oswindow oswindow, LPITEMIDLIST * lpiidl, const char * lpcsz);

         int run();




         //         virtual index GetCSIDLSort(index iCsidl);
         //         virtual index GetCSIDL(per_fork * pfork, LPITEMIDLIST lpiidl);
         //         virtual void GetAscendants(per_fork * pfork, LPITEMIDLIST lpiidl, array < LPITEMIDLIST, LPITEMIDLIST > & lpiidla);
         //         virtual void Free(per_fork * pfork, array < LPITEMIDLIST, LPITEMIDLIST > & lpiidla);
         //
         //         //      string CLASS_DECL_CORE _017FilePathGetParent(const char * lpcsz);
         //
         //         HICON CalcIcon(LPITEMIDLIST lpiidl, const char * lpcszExtra, int32_t cx, int32_t cy);
         //         bool _017HasSubFolder(::aura::application * papp, LPITEMIDLIST lpiidl, const char * lpcszExtra);
         //         //      void CLASS_DECL_CORE GetChildren(stringa & stra, const char * lpcszPath);
         //         LPITEMIDLIST _017ItemIDListGetLast(per_fork * pfork, LPITEMIDLIST lpiidl);
         //         LPITEMIDLIST _017ItemIDListDup(per_fork * pfork, LPITEMIDLIST lpiidl);
         //         LPITEMIDLIST _017ItemIDListGetFolderParent(per_fork * pfork, LPITEMIDLIST lpiidl);
         //         LPITEMIDLIST _017ItemIDListGetAbsolute(per_fork * pfork, LPITEMIDLIST lpiidlParent, LPITEMIDLIST lpiidl);
         //         bool _017ItemIDListIsEqual(LPITEMIDLIST lpiidl1, LPITEMIDLIST lpiidl2);



         //int run();


      };


   } // namespace windows


} // namespace filemanager


