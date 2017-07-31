//#include "framework.h"


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   CLASS_DECL_AURA stringa get_app_app_install_module_list()
   {

      stringa straFile;

      straFile.add("app_app_admin.exe");

      return straFile;

   }


   //CLASS_DECL_AURA stringa get_app_app_install_module_list(const string & strPlatform, const string & strVersion)
   //{

   //   stringa straFile;

   //   straFile.add("app_app_admin.exe");
   //   straFile.add("ace.dll");
   //   straFile.add("aqua.dll");
   //   straFile.add("aura.dll");
   //   straFile.add("sqlite.dll");
   //   straFile.add("axis.dll");
   //   straFile.add("bzip2.dll");

   //   if (strPlatform == "x64")
   //   {

   //      straFile.add("libcrypto-1_1-x64.dll");
   //      straFile.add("libssl-1_1-x64.dll");

   //   }
   //   else
   //   {

   //      straFile.add("libcrypto-1_1.dll");
   //      straFile.add("libssl-1_1.dll");

   //   }

   //   straFile.add("axisfreeimage.dll");
   //   //straFile.add("axisfreetype.dll");
   //   //straFile.add("idn.dll");
   //   straFile.add("pcre.dll");
   //   straFile.add("zlib.dll");
   //   //straFile.add("axis.dll");
   //   straFile.add("base.dll");
   //   straFile.add("draw2d_gdiplus.dll");

   //   //if(strVersion == "stage")
   //   //{

   //   //   //straFile.add("ucrtbase.dll");
   //   //   straFile.add("vcruntime140.dll");
   //   //   straFile.add("vcomp140.dll");

   //   //}
   //   //else
   //   //{

   //   //   //straFile.add("ucrtbased.dll");
   //   //   straFile.add("vcruntime140d.dll");
   //   //   straFile.add("vcomp140d.dll");

   //   //}

   //   return straFile;

   //}


} // namespace install



#endif




