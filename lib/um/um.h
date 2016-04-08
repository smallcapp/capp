#ifndef __LIB_UM_UM_H__
#define __LIB_UM_UM_H__
/******************************************************************************/
#include "utils.h"
/******************************************************************************/
#ifndef UM_USE_MONITOR
#define UM_USE_MONITOR          0
#endif

#ifndef UM_USE_GC
#define UM_USE_GC               0
#endif

#ifndef ENV_UMD_UNIX
#define ENV_UMD_UNIX            "__UMD_UNIX__"
#endif

#ifndef ENV_UMD_GC
#define ENV_UMD_GC              "__UMD_GC__"
#endif

#ifndef ENV_UMC_UNIX
#define ENV_UMC_UNIX            "__UMC_UNIX__"
#endif

#ifndef ENV_UM_TIMEOUT
#define ENV_UM_TIMEOUT          "__UM_TIMEOUT__"
#endif

#ifndef UMD_UNIX
#define UMD_UNIX                "/tmp/.umd.unix"
#endif

#ifndef UMC_UNIX
#define UMC_UNIX                "/tmp/.umc.%d.unix"
#endif

#ifndef UM_TIMEOUT
#define UM_TIMEOUT              3000
#endif

#ifndef UMD_GC
#ifdef __PC__
#define UMD_GC                  30
#else
#define UMD_GC                  600
#endif
#endif

static inline int
get_umd_path_env(struct sockaddr_un *addr) 
{
    return __copy_string_env(ENV_UMD_UNIX, UMD_UNIX, 
                get_abstract_path(addr),
                abstract_path_size);
}

static inline int
get_umc_path_env(struct sockaddr_un *addr) 
{
    char path[1+OS_LINE_LEN] = {0};

    os_saprintf(path, UMC_UNIX, getpid());
    
    return __copy_string_env(ENV_UMC_UNIX, path, 
                get_abstract_path(addr),
                abstract_path_size);
}

static inline int
get_um_timeout_env(void) 
{
    return get_int_env(ENV_UM_TIMEOUT, UM_TIMEOUT);
}

static inline int
get_umd_gc_env(void) 
{
    return get_int_env(ENV_UMD_GC, UMD_GC);
}
/******************************************************************************/
#endif /* __LIB_UM_UM_H__ */
