#ifndef __SMONITOR_H_61C5E8485DEB03CFF49ECA4D8DC2BCE9__
#define __SMONITOR_H_61C5E8485DEB03CFF49ECA4D8DC2BCE9__
/******************************************************************************/
#include "utils.h"
/******************************************************************************/
#ifndef ENV_SMONITORD_UNIX
#define ENV_SMONITORD_UNIX          "__SMONITORD_UNIX__"
#endif

#ifndef ENV_SMONITORC_UNIX
#define ENV_SMONITORC_UNIX          "__SMONITORC_UNIX__"
#endif

#ifndef ENV_SMONITOR_TIMEOUT
#define ENV_SMONITOR_TIMEOUT        "__SMONITOR_TIMEOUT__"
#endif

#ifndef SMONITORD_UNIX
#define SMONITORD_UNIX              "/tmp/.smonitord.unix"
#endif

#ifndef SMONITORC_UNIX
#define SMONITORC_UNIX              "/tmp/.smonitorc.%d.unix"
#endif

#ifndef SMONITOR_TIMEOUT
#define SMONITOR_TIMEOUT            3000
#endif

#ifndef SMONITOR_NAMESIZE
#define SMONITOR_NAMESIZE           31
#endif

#ifndef SMONITOR_CMDSIZE
#define SMONITOR_CMDSIZE            127
#endif

#ifndef SMONITOR_PIDFILE
#define SMONITOR_PIDFILE            127
#endif

#ifndef SMONITOR_TIMER
#define SMONITOR_TIMER              1
#endif

#ifndef SCRIPT_SMONITOR_GETPID
#ifdef __PC__
#define SCRIPT_SMONITOR_GETPID      "./.smonitor.getpid"
#else
#define SCRIPT_SMONITOR_GETPID      "/usr/sbin/.smonitor.getpid"
#endif
#endif

#ifndef SCRIPT_SMONITOR_INIT
#ifdef __PC__
#define SCRIPT_SMONITOR_INIT        "./.smonitor.init"
#else
#define SCRIPT_SMONITOR_INIT        "/tmp/.smonitor.init"
#endif
#endif

enum {
    SMONITOR_STATE_INIT,
    SMONITOR_STATE_FORK,
    SMONITOR_STATE_RUN,
    SMONITOR_STATE_DIE,

    SMONITOR_STATE_END
};

static inline bool
is_good_smonitor_state(int state)
{
    return is_good_enum(state, SMONITOR_STATE_END);
}

#define SMONITOR_STATE_STRINGS      {   \
    [SMONITOR_STATE_INIT]   = "init",   \
    [SMONITOR_STATE_FORK]   = "fork",   \
    [SMONITOR_STATE_RUN]    = "run",    \
    [SMONITOR_STATE_DIE]    = "die",    \
}   /* end */

static inline char **
__smonitor_state_strings(void)
{
    static char *array[SMONITOR_STATE_END] = SMONITOR_STATE_STRINGS;

    return array;
}

static inline char *
smonitor_state_string(int state)
{
    char **array = __smonitor_state_strings();
    
    return is_good_smonitor_state(state)?array[state]:__unknow;
}

static inline int
smonitor_state_idx(char *state)
{
    char **array = __smonitor_state_strings();
    
    return os_getstringarrayidx(array, state, 0, SMONITOR_STATE_END);
}

static inline int
get_smonitord_path_env(struct sockaddr_un *addr) 
{
    return __copy_string_env(ENV_SMONITORD_UNIX, SMONITORD_UNIX, 
                get_abstract_path(addr),
                abstract_path_size);
}

static inline int
get_smonitorc_path_env(struct sockaddr_un *addr) 
{
    char path[1+OS_LINE_LEN] = {0};

    os_saprintf(path, SMONITORC_UNIX, getpid());
    
    return __copy_string_env(ENV_SMONITORC_UNIX, path, 
                get_abstract_path(addr),
                abstract_path_size);
}

static inline int
get_smonitor_timeout_env(void) 
{
    return get_int_env(ENV_SMONITOR_TIMEOUT, SMONITOR_TIMEOUT);
}
/******************************************************************************/
#endif /* __SMONITOR_H_61C5E8485DEB03CFF49ECA4D8DC2BCE9__ */
