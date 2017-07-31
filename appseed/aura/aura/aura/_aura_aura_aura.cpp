#include "framework.h"

#if defined(LINUX) || defined(ANDROID)

#include <fcntl.h>
#include <unistd.h>


#include <link.h>

#include <dlfcn.h>
#include <ctype.h>
#include <sys/stat.h>
#endif


//O volume na unidade Z ‚ eco
// O N£mero de S‚rie do Volume ‚ 4A91-94E4
//
// Pasta de Z:\ca2\app\appseed\aura\aura\aura
//
//2014-12-19  18:28             1.225 aaa_want_to_remove_user_check_interface.cpp"
#include "aura.cpp"
#include "aura_action_context.cpp"
#include "aura_allocate_mid_big.cpp"
#include "aura_allocate_system_heap.cpp"

// THANK YOU ("jai") = Jonathan Blow - Data-Oriented intercourse
#if defined(_M_IX86)
#include "aura_allocate_x86_cache_oriented.cpp"
#endif
#include "aura_allocate_processor_cache_oriented.cpp"



#include "aura_application.cpp"
#include "aura_argcargv.cpp"
#include "aura_assert.cpp"
#include "aura_assert_running_task.cpp"
#include "aura_checked.cpp"
#include "aura_command.cpp"
#include "aura_command_msg.cpp"
#include "aura_command_target.cpp"
#include "aura_command_target_interface.cpp"
#include "aura_command_ui.cpp"
#include "aura_command_ui_test.cpp"
#include "aura_conv.cpp"
#include "aura_os.cpp"
//#include "aura_cpu_architecture.cpp"
#include "aura_create_context.cpp"
#include "aura_cregexp.cpp"
#include "aura_cregexp_util.cpp"
#include "aura_debug.cpp"
#include "aura_department.cpp"
#include "aura_department_container.cpp"
#include "aura_draw_context.cpp"
#include "aura_draw_interface.cpp"
#include "aura_element.cpp"
#include "aura_file_watcher.cpp"
#include "aura_file_watcher_impl.cpp"
#include "aura_file_watcher_listener_thread.cpp"
#include "aura_file_watcher_thread.cpp"
#include "aura_fixed_alloc.cpp"
#include "aura_flags.cpp"
#include "aura_gudo.cpp"
#include "aura_heap.cpp"
#include "aura_id_pool.cpp"
#include "aura_international.cpp"
#include "aura_library.cpp"
#include "aura_live_object.cpp"
#include "aura_live_signal.cpp"
#include "aura_local_memory_map.cpp"
#include "aura_log.cpp"
#include "aura_machine_event.cpp"
#include "aura_machine_event_central.cpp"
#include "aura_machine_event_data.cpp"
#include "aura_math.cpp"
//#include "aura_message_queue.cpp"
#include "aura_chronometer.cpp"
#include "aura_mktime.cpp"
#include "aura_muldiv64.cpp"
#include "aura_net.cpp"
#include "aura_new.cpp"
//#include "aura_platform_interface.cpp"
#include "aura_plex.cpp"
#include "aura_plex_heap.cpp"
#include "aura_printer.cpp"
#include "aura_printf.cpp"
#include "aura_progress.cpp"
#include "aura_savings.cpp"
#include "aura_session.cpp"
//#include "aura_session_interface.cpp"
#include "aura_shell_launcher.cpp"
#include "aura_ipc.cpp"
#include "aura_sprintf.cpp"
#include "aura_static_start.cpp"
#include "aura_str_context.cpp"
#include "aura_system.cpp"
#include "aura_template.cpp"
#include "aura_timer_callback.cpp"
#include "aura_timer.cpp"
#include "aura_timer_event.cpp"
#include "aura_timer_array.cpp"
#include "aura_timer_item.cpp"
#include "aura_trace.cpp"
//2015-01-05  07:52                 0 _aura.cpp"
//              77 arquivo(s)        382.228 bytes
//               0 pasta(s)   36.251.623.424 bytes dispon¡veis


#include "aura_console_window.cpp"


#include "aura_profiler.cpp"





//#include "aura_timer_callback.cpp"
//#include "aura_timer.cpp"
//#include "aura_timer_event.cpp"







#include "aura_ipi.cpp"
#include "aura_app_launcher.cpp"


#include "aura_app_core.cpp"


#include "aura_speaker.cpp"


#include "aura_md5.cpp"


#include "aura_lparam.cpp"








#include "simple_log.cpp"








