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

   BOOL graphics_object::is_set()
   {
      throw interface_only_exception();   
   }

   BOOL graphics_object::DeleteObject()
   {
      // SIOOT - Should implemennt one of them
      // OASOWO - otherwise a stack overflow will occur
      // BTAIOM - because these are interface only methods
      return delete_object(); // the first call the last or implementation // comment of comment

   }

   BOOL graphics_object::delete_object()
   {
      // SIOOT - Should implemennt one of them
      // OASOWO - otherwise a stack overflow will occur
      // BTAIOM - because these are interface only methods
      return DeleteObject(); // the others call the previous or implementation // comment of comment
   }

   // graphics_object
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
      // interface only
      return NULL;
   }

   void * graphics_object::detach_os_data()
   {
      // interface only
      return NULL;
   }

} // namespace ca