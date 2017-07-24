#pragma once


#include "core/core/core.h"
#include "core/user/user/user.h"       // thank you (mummy)
#include "core/user/wndfrm/wndfrm.h"    // thank you (mummy)


#ifdef _CA2_CORE_WNDFRM_CORE_LIBRARY
    #define CLASS_DECL_APP_CORE_WNDFRM_CORE  CLASS_DECL_EXPORT
#else
    #define CLASS_DECL_APP_CORE_WNDFRM_CORE  CLASS_DECL_IMPORT
#endif

#include "user_meta_button.h"
#include "user_meta_control_box.h"


#include "wndfrm_core_frame_schema.h"


#include "wndfrm_core_frame_schema_001.h"
#include "wndfrm_core_frame_schema_002.h"
#include "wndfrm_core_frame_schema_005.h"
#include "wndfrm_core_frame_schema_008.h"
#include "wndfrm_core_frame_schema_011.h"
#include "wndfrm_core_frame_schema_013.h"
#include "wndfrm_core_frame_schema_Minimal001.h"


#include "wndfrm_core_interaction.h"


#include "wndfrm_core_user_schema.h"


#include "wndfrm_core_library.h"




