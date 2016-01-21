#ifndef __THIS_APP
#define __THIS_APP      atrt
#endif

#define __DEAMON__

#include "utils.h"
#include "atenv/atenv.h"

OS_INITER;

#if !AT_LTEFI_MD
#error "just support ltefi md board"
#endif

AT_ENV_INITER;

#define RT_SAVECYCLE    3141

static int runtime;

#if 1==AT_LTEFI_VERSION

static int
__rt_load(int idx)
{
    (void)idx;

    os_spgeti(&runtime, "bootm rt");

    return 0;
}

static int
__rt_save(int idx)
{
    (void)idx;
    
    os_system("bootm rt=%d", runtime);

    return 0;
}
#elif 2==AT_LTEFI_VERSION
static int
__rt_load(int idx)
{
    (void)idx;
    
    runtime = at_mark_get(__at_mark_runtime);
    
    return 0;
}

static int
__rt_save(int idx)
{
    (void)idx;

    at_mark_set(__at_mark_runtime,  runtime);

    return 0;
}
#endif

static int
rt_load(int noused)
{
    int err;
    
    (void)noused;
    
    err = os_call_1(__at_load, __at_nothing, __rt_load, AT_ENV_MARK);
        debug_trace_error(err, "rt load");

    return err;
}

static int
rt_save(int noused)
{
    int err;
    
    (void)noused;
    
    err = os_call_1(__at_load, __at_save, __rt_save, AT_ENV_MARK);
        debug_trace_error(err, "rt save");

    return err;
}

static void 
__user(int signo)
{
    debug_trace("recive signo:%d", signo);

    if (SIGUSR2==signo) {
        runtime = 0;
    }

    os_call(at_open, at_close, rt_save, 0);
}

static void 
__exit(int signo)
{
    int err;

    debug_trace("recive signo:%d", signo);
    
    err = os_call(at_open, at_close, rt_save, 0);

    os_fini();
    
    exit(err);
}

#ifndef __BUSYBOX__
#define atrt_main  main
#endif

static int
__main(int argc, char *argv[])
{
    int i, err;
    
    os_call(at_open, at_close, rt_load, 0);

    while(1) {
        sleep(1);

        runtime++;
        if ((RT_SAVECYCLE-1)==(runtime%RT_SAVECYCLE)) {
            os_call(at_open, at_close, rt_save, 0);
        }
    }

    return 0;
}

int atrt_main(int argc, char *argv[])
{
    os_setup_signal_user(__user);
    os_setup_signal_exit(__exit);
    os_setup_signal_callstack(NULL);
    
    return os_main(__main, argc, argv);
}
/******************************************************************************/
