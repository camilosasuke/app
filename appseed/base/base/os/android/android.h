#pragma once


#include "axis/os/android/android.h"

#ifdef VSNORD
BEGIN_EXTERN_C
#include "android_vsnord_native_app_glue.h"
END_EXTERN_C
#include "android_window_buffer.h"

#else

#include "android_window_android.h"

#endif // VSNORD


#include "base/graphics/graphics_window_buffer.h"

//#include "android_window_android.h"
//#include "android_window_impl.h"
#include "android_view_bitmap.h"



#include "android_windowing.h"