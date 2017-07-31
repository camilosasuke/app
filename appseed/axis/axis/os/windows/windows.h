#pragma once


#ifdef cplusplus


CLASS_DECL_AXIS bool vfxResolveShortcut(string & strTarget,const char * pszSource,sp(::user::primitive) puiMessageParentOptional);



#endif


//#ifdef cplusplus
//
//namespace axis
//{
//
//
//   template < class APP >
//   static int32_t app_main(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int32_t nCmdShow)
//   {
//
//      if(!defer_axis_init())
//      {
//
//         ::OutputDebugString("Failed to defer_axis_init.");
//
//         return -2;
//
//      }
//
//      APP  * papp = new APP;
//
//      __node_init_main_data(papp, hInstance,hPrevInstance,lpCmdLine,nCmdShow);
//
//      int32_t iRet;
//
//      iRet = papp->main();
//
//      try
//      {
//
//         delete papp;
//
//         papp = NULL;
//
//      }
//      catch(...)
//      {
//      }
//
//      if(!defer_axis_term())
//      {
//
//         ::OutputDebugString("Failed to defer_axis_term.");
//
//         iRet -= 9011;
//
//      }
//
//      return iRet;
//
//   }
//
//
//} // namespace axis
//
//
//
//
//
//#endif




CLASS_DECL_AXIS HMONITOR GetUiMonitorHandle(HWND hwnd);
CLASS_DECL_AXIS HMONITOR GetPrimaryMonitorHandle();
CLASS_DECL_AXIS bool GetPrimaryMonitorRect(LPRECT lprect);
CLASS_DECL_AXIS WSADATA get_wsadata();

#ifdef HOTPLUGIN_SUBSYSTEM

#ifdef cplusplus

namespace hotplugin
{

   CLASS_DECL_AXIS void entry_hall_windows_on_paint(HDC hdc, const RECT & rect, const string & strEntryHallText);


} // namespace hotplugin

#endif

#endif