#include "StdAfx.h"

namespace ca
{

   /////////////////////////////////////////////////////////////////////////////
   // graphics_object

#ifdef _DEBUG
   void graphics_object::dump(dump_context & dumpcontext) const
   {
      UNREFERENCED_PARAMETER(dumpcontext);
      throw interface_only_exception();   
   }

   void graphics_object::assert_valid() const
   {
      throw interface_only_exception();   
   }
#endif

   graphics_object::~graphics_object()
   {
   }

   int graphics_object::_AFX_FUNCNAME(GetObject)(int nCount, LPVOID lpObject) const
   {
      UNREFERENCED_PARAMETER(nCount);
      UNREFERENCED_PARAMETER(lpObject);
      throw interface_only_exception();   
   }

#pragma push_macro("GetObject")
#undef GetObject

   int graphics_object::GetObject(int nCount, LPVOID lpObject) const
   {
      return _AFX_FUNCNAME(GetObject)(nCount, lpObject); 
   }

#pragma pop_macro("GetObject")

   BOOL graphics_object::CreateStockObject(int nIndex)
   {
      UNREFERENCED_PARAMETER(nIndex);
      throw interface_only_exception();   
   }

   BOOL graphics_object::UnrealizeObject()
   {
      throw interface_only_exception();   
   }

   UINT graphics_object::GetObjectType() const
   {
      throw interface_only_exception();   
   }

   BOOL graphics_object::operator==(const graphics_object & obj) const
   {
      UNREFERENCED_PARAMETER(obj);
      throw interface_only_exception();   
   }

   BOOL graphics_object::operator!=(const graphics_object & obj) const
   {
      UNREFERENCED_PARAMETER(obj);
      throw interface_only_exception();   
   }

   void * graphics_object::get_os_data() const
   {
      return NULL;
   }

   bool graphics_object::is_set()
   {
      return get_os_data() != NULL;
   }

   bool graphics_object::is_updated()
   {
      return is_set() && m_bUpdated;
   }


} // namespace ca