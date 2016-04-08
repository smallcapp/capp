#ifndef __LIB_UTILS_PID_H__
#define __LIB_UTILS_PID_H__
/******************************************************************************/
#ifdef __APP__

#define DECLARE_REAL_DEAMON_RUNNING     bool __THIS_DEAMON_RUNNING = false
#define DECLARE_FAKE_DEAMON_RUNNING     extern bool __THIS_DEAMON_RUNNING

#ifdef __BUSYBOX__
#define DECLARE_DEAMON_RUNNING          DECLARE_FAKE_DEAMON_RUNNING
#else
#define DECLARE_DEAMON_RUNNING          DECLARE_REAL_DEAMON_RUNNING
#endif

DECLARE_FAKE_DEAMON_RUNNING;

static inline int
__os_get_deamon(char *pidfile)
{
    int pid = 0;

    os_v_sfgeti(&pid, "%s", pidfile);
    
    return pid;
}

static inline int
os_get_deamon(void)
{
    int pid = 0;

    os_sfgeti(&pid, __THIS_PIDFILE);
    
    return pid;
}

static inline void
__os_set_deamon(char *pidfile, int pid)
{
    os_v_sfseti(pid, "%s", pidfile);
}

static inline void
os_set_deamon(void)
{
    os_sfseti(getpid(), __THIS_PIDFILE);
}

static inline bool
os_pid_exist(int pid)
{
    return pid > 0 && 0==kill(pid, 0);
}

static inline bool
__os_deamon_exist(void)
{
    return os_pid_exist(os_get_deamon());
}

#ifdef __DEAMON__
static inline int
os_deamon_check(void)
{    
    if (__os_deamon_exist()) {
        return -EDEAMON;
    } else {
        __THIS_DEAMON_RUNNING = true;
        
        os_set_deamon();
        
        return 0;
    }
}

static inline void
os_deamon_exit(void)
{
    if (__THIS_DEAMON_RUNNING) {
        unlink(__THIS_PIDFILE);
    }
}
#else
#define os_deamon_check()   0
#define os_deamon_exit()    os_do_nothing()
#endif

#endif
/******************************************************************************/
#endif /* __LIB_UTILS_PID_H__ */
