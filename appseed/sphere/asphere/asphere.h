#pragma once



#include "core/core/core.h"
#include "core/filesystem/filemanager/filemanager.h"
#include "core/user/html/html/html.h"
#include "math/math/math_math.h"




#ifdef __SPHERE__LIBRARY
   #define CLASS_DECL_SPHERE  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_SPHERE  CLASS_DECL_IMPORT
#endif


namespace sphere
{


   class application;


   //enum e_theme
   //{

   //   theme_first = 0,
   //   theme_lite = theme_first,
   //   theme_blue,
   //   theme_dark,
   //   theme_count

   //};





} // namespace sphere


#undef App
#define App(pcaapp) (pcaapp->cast_app < ::sphere::application > ())


#include "asphere_application.h"


