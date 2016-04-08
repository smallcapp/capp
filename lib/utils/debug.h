#ifndef __LIB_UTILS_DEBUG_H__
#define __LIB_UTILS_DEBUG_H__
/******************************************************************************/
#ifdef __APP__

#define BACKTRACE_by_none               0
#define BACKTRACE_by_libc_backtrace     1
#define BACKTRACE_by_libunwind          2

#ifndef BACKTRACE_TYPE
#   define BACKTRACE_TYPE      BACKTRACE_by_libc_backtrace
#endif

#define BACKTRACE_DESTROY_STACK_FOREVER(_up) do{   \
    int var;            \
    int *p = &var;      \
                        \
    while(1) {          \
        os_println("write %p to zero", p); \
        if (_up) {      \
            *p++ = 0;   \
        } else {        \
            *p-- = 0;   \
        }               \
    }                   \
}while(0)

#define BACKTRACE_DESTROY_STACK_BYSIZE(_up, _size)  do{   \
    int var;                        \
    char *p = (char *)&var;         \
                                    \
    os_println("write stack %s %d's byte to zero", _up?"up":"down", _size); \
                                    \
    if (_up) {                      \
        os_memzero(p, _size);          \
    } else {                        \
        os_memzero(p - _size, _size);  \
    }                               \
}while(0)

#define BACKTRACE_WRITE_POINTER(_v)    do {   \
    int i;                  \
    int *p = _v;            \
                            \
    for (i=0; i<10; i++) {  \
        os_println("write pointer(%p) in function %s", p, __func__); \
    }                       \
                            \
    *p = 0;                 \
}while(0)

#define BACKTRACE_READ_POINTER(_v)    do {   \
    int i;                  \
    int *p = _v;            \
                            \
    for (i=0; i<10; i++) {  \
        os_println("read pointer(%p) in function %s", p, __func__); \
    }                       \
                            \
    i = *p;                 \
}while(0)

#ifndef BACKTRACE_PATH
#ifdef __PC__
#define BACKTRACE_PATH          "."
#else
#define BACKTRACE_PATH          "/tmp/.backtrace"
#endif
#endif

#define BACKTRACE_NAME          BACKTRACE_PATH "/" __THIS_NAME
#define BACKTRACE_FILE_TRACE    BACKTRACE_NAME ".trace"
#define BACKTRACE_FILE_MAPS     BACKTRACE_NAME ".maps"
#define BACKTRACE_FILE_LOCATE   BACKTRACE_NAME ".locate"

#ifndef BACKTRACE_ASIZE
#define BACKTRACE_ASIZE         256
#endif

#if BACKTRACE_TYPE==BACKTRACE_by_libunwind
#include <libunwind.h>

static inline void 
__os_sighandle_callstack(int signo)
{
    unw_cursor_t    cursor;
    unw_context_t   context;

    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    while (unw_step(&cursor) > 0) {
        unw_word_t  offset, pc;
        char        fname[128];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);

        fname[0] = '\0';
        (void) unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);

        printf ("%p : (%s+0x%x) [%p]\n", (void *)pc, fname, offset, (void *)pc);
    }

    exit(signo);
}

#elif BACKTRACE_TYPE==BACKTRACE_by_libc_backtrace
#include <execinfo.h>

static inline void 
__os_sighandle_callstack(int signo)
{
    void *array[BACKTRACE_ASIZE] = {NULL};
    int i, count, fd;
    int pid = getpid();
    unsigned int now = (unsigned int)time(NULL);
    
    fd = os_v_open(O_WRONLY | O_CREAT | O_TRUNC, BACKTRACE_FILE_TRACE);
    count = backtrace(array, os_count_of(array));    
    backtrace_symbols_fd(array, count, fd);
    close(fd);
    
    os_println("backtrace array count(%d)", count);
    for (i=0; i<count; i++) {
        os_println(__tab "backtrace array[%d]=%p", i, array[i]);
    }
    os_println("backtrace array end");
    
    os_system("cp /proc/%d/maps " BACKTRACE_FILE_MAPS, pid);
    
{
    char lib[256];
    unsigned long base_address = 0;
    unsigned long dead_address = (unsigned long)array[3];
    
    FILE *stream = os_v_popen("r", 
        "awk -F\"[- ]\" '{if($1<=\"%lx\" && $2>=\"%lx\") print$1, $NF}' "
        BACKTRACE_FILE_MAPS,
        dead_address,
        dead_address);
    fscanf(stream, "%lx %s", &base_address, lib);
    
    if (NULL==strstr(lib, ".so.")) {
        os_system("addr2line -e ./%s 0x%lx > " BACKTRACE_FILE_LOCATE, __THIS_NAME, dead_address);
    } else {
        os_system("addr2line -e %s 0x%lx > " BACKTRACE_FILE_LOCATE, lib, dead_address - base_address);
    }
}

    exit(signo);
}
#else
static inline void
__os_sighandle_callstack(int signo)
{
    os_do_nothing();
}
#endif /* BACKTRACE_TYPE */

static inline int os_fini(void);

static inline void 
__os_sighandle_donothing(int signo)
{
    os_do_nothing();
}

static inline void 
__os_sighandle_exit(int signo)
{
    os_fini();
    
    exit(signo);
}

static inline void
__os_setup_signal(void (*handle)(int signo), int sigs[], int count)
{
    struct sigaction s = {
        .sa_flags   = 0,
        .sa_handler = handle,
    };
    int i;
    
	for (i=0; i<count; i++) {
        sigaction(sigs[i], &s, NULL);
	}
}

#define os_setup_signal(_handle, _sigs) \
        __os_setup_signal(_handle, _sigs, os_count_of(_sigs))

#define os_signal_user_list     SIGUSR1, SIGUSR2
static inline void
os_setup_signal_user(void (*handle)(int signo))
{
    int sigs[] = {os_signal_user_list};

    os_setup_signal(handle?handle:__os_sighandle_exit, sigs);
}

#define os_signal_exit_list     SIGINT, SIGHUP, SIGTERM, SIGABRT
static inline void
os_setup_signal_exit(void (*handle)(int signo))
{
    int sigs[] = {os_signal_exit_list};

    os_setup_signal(handle?handle:__os_sighandle_exit, sigs);
}

#define os_signal_timer_list    SIGALRM
static inline void
os_setup_signal_timer(void (*handle)(int signo))
{
    int sigs[] = {os_signal_timer_list};

    os_setup_signal(handle?handle:__os_sighandle_donothing, sigs);
}

#define os_signal_callstack_list    SIGBUS, SIGILL, SIGFPE, SIGSEGV
static inline void
os_setup_signal_callstack(void (*handle)(int signo))
{
    int sigs[] = {os_signal_callstack_list};

    os_setup_signal(handle?handle:__os_sighandle_callstack, sigs);
}
#else
#define os_setup_signal_user(_handle)       os_do_nothing()
#define os_setup_signal_exit(_handle)       os_do_nothing()
#define os_setup_signal_timer(_handle)      os_do_nothing()
#define os_setup_signal_callstack(_handle)  os_do_nothing()
#endif /* __APP__ */
/******************************************************************************/
#endif /* __LIB_UTILS_DEBUG_H__ */
