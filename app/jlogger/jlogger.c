#ifndef __THIS_APP
#define __THIS_APP      jlogger
#endif

#ifndef ENV_JLOG_SUB
#define ENV_JLOG_SUB    "__JLOG_SUB__"
#endif

#define __JLOGGER__

#include "utils.h"

OS_INITER;

static char *self;
static char sub[1+__APP_NAMELEN];

static int
usage(int error)
{
#if USE_USAGE
    os_fprintln(stderr, "%s pri app json", self);
#endif

    return error;
}

static int 
__main(int argc, char *argv[])
{
    char *app, *obj;
    int err, pri;
    
    self = os_cmd_trace_argv(argc, argv);

    if (4 != argc) {
        return usage(-EHELP);
    }

    pri = os_atoi(argv[1]);
    app = argv[2];
    obj = argv[3];
    
    copy_string_env(ENV_JLOG_SUB, __empty, sub);
    
    err = __jclogger(app, sub, NULL, 0, 0, pri, obj);
    
    return err;
}

#ifndef __BUSYBOX__
#define jlogger_main  main
#endif

int jlogger_main(int argc, char *argv[])
{
    os_setup_signal_exit(NULL);
    os_setup_signal_callstack(NULL);
    
    return os_main(__main, argc, argv);
}
/******************************************************************************/
