#ifndef __THIS_APP
#define __THIS_APP      smonitord
#endif

#define __DEAMON__
#define USE_INLINE_SIMPILE_RES

#include "utils.h"
#include "smonitor/smonitor.h"

OS_INITER;
DECLARE_SIMPILE_RES;

#ifndef SMONITOR_NORMAL_SLEEP
#define SMONITOR_NORMAL_SLEEP       500
#endif

#ifndef SMONITOR_DEAMON_WAIT
#define SMONITOR_DEAMON_WAIT        3
#endif

static struct {
    struct {
        int fd;
        
        struct sockaddr_un addr;
    } server;

    FILE *f;
    int timeout;
    unsigned int time;
    
    struct list_head list;
}
smonitord = {
    .server = {
        .fd     = INVALID_FD,
        .addr   = OS_SOCKADDR_UNIX("\0" SMONITORD_UNIX),
    },

    .timeout = SMONITOR_TIMEOUT,

    .list = LIST_HEAD_INIT(smonitord.list),
};

enum {
    SMONITOR_F_STATIC   = 0x01,
};

struct smonitor {
    char *name;
    char *command;
    char *pidfile;
    
    int normal;
    int deamon;
    int forks;
    int state;
    int flags;
    int offset;
    
    unsigned int time[SMONITOR_STATE_END];
    
    struct list_head node;
};

#define DAEMONS(_name, _command, _pidfile) {  \
    .name   = _name,                \
    .command= _command,             \
    .pidfile= _pidfile,             \
    .flags  = SMONITOR_F_STATIC,    \
}   /* end */

static struct smonitor daemons[] = {
#if 1
    /* insert static daemon */
#endif
};

static inline bool
__is_normal(struct smonitor *entry)
{
    return NULL==entry->pidfile;
}
#define __is_deamon(_entry)     (false==__is_normal(_entry))

static inline bool
__is_timeout(struct smonitor *entry)
{
    return smonitord.time - entry->time[entry->state] > SMONITOR_DEAMON_WAIT;
}

static inline char *
__type(struct smonitor *entry)
{
    return __is_normal(entry)?"normal":"deamon";
}

static inline bool
__is_run(struct smonitor *entry)
{
    return SMONITOR_STATE_RUN==entry->state;
}

static inline bool
__have_pid(struct smonitor *entry)
{
    return SMONITOR_STATE_FORK==entry->state || SMONITOR_STATE_RUN==entry->state;
}

static int
__get_normal_pid(struct smonitor *entry)
{
    int i, pid = 0;

    for (i=0; i<3; i++) {
        os_v_spgeti(&pid, 
            "ps |"
                " grep %s |"
                " grep -v monitor |"
                " grep -v \"\\[\" |"
                " grep -v grep |"
                " head -n 1 |"
                " awk '{print $1}'",
            entry->name);
        if (pid) {
            break;
        }

        usleep(time_usec(SMONITOR_NORMAL_SLEEP));
    }

    debug_trace("get normal(%s) pid(%d)", entry->name, pid);

    return pid;
}

static inline int
__get_deamon_pid(struct smonitor *entry)
{
    int pid = __os_get_deamon(entry->pidfile);

    debug_trace("get normal(%s) pid(%d)", entry->name, pid);

    return pid;
}

static inline void
__set_time(struct smonitor *entry, char *prefix)
{
    entry->time[entry->state] = smonitord.time;

    debug_trace("%s: set entry(%s) state(%s) time(%d)",
        prefix,
        entry->name, 
        smonitor_state_string(entry->state),
        entry->time[entry->state]);
}

static void
__change(struct smonitor *entry, int state, int normal, int deamon, char *prefix)
{
    if (normal!=entry->normal && deamon!=entry->deamon) {
        debug_trace("%s: set entry(%s) state(%s==>%s), normal pid(%d==>%d), deamon pid(%d==>%d)",
            prefix,
            entry->name, 
            smonitor_state_string(entry->state),
            smonitor_state_string(state),
            entry->normal, normal,
            entry->deamon, deamon);
        
        entry->normal = normal;
        entry->deamon = deamon;
    }
    else if (normal!=entry->normal) {
        debug_trace("%s: set entry(%s) state(%s==>%s), normal pid(%d==>%d)",
            prefix,
            entry->name, 
            smonitor_state_string(entry->state),
            smonitor_state_string(state),
            entry->normal, normal);
        
        entry->normal = normal;
        entry->deamon = deamon;
    }
    else if (deamon!=entry->deamon) {
        debug_trace("%s: set entry(%s) state(%s==>%s), deamon pid(%d==>%d)",
            prefix,
            entry->name, 
            smonitor_state_string(entry->state),
            smonitor_state_string(state),
            entry->deamon, deamon);
        
        entry->deamon = deamon;
    }

    if (state!=entry->state) {
        entry->state = state;
        __set_time(entry, prefix);
    }
}

static void
__dump_all(char *name)
{
    struct smonitor *entry;

    if (__is_debug_level_trace) {
        list_for_each_entry(entry, &smonitord.list, node) {
            debug_trace("entry:%s pid:%d/%d forks:%d command:%s ", 
                entry->name,
                entry->normal, entry->deamon,
                entry->forks,
                entry->command);
        }
    }
}

static struct smonitor *
__get_byname(char *name)
{
    struct smonitor *entry;
    
    if (NULL==name) {
        return NULL;
    }

    list_for_each_entry(entry, &smonitord.list, node) {
        if (0==os_strcmp(name, entry->name)) {
            return entry;
        }
    }
    
    return NULL;
}

static struct smonitor *
__get_bynormal(int pid)
{
    struct smonitor *entry;

    list_for_each_entry(entry, &smonitord.list, node) {
        if (pid==entry->normal) {
            return entry;
        }
    }

    return NULL;
}

static int
__run(struct smonitor *entry, char *prefix)
{
    int err, pid;
    
    pid = vfork();
    if (pid<0) {
        debug_trace("fork error:%d", -errno);
        
        return -errno;
    }
    else if (pid>0) {
        entry->forks++;

        __change(entry, SMONITOR_STATE_FORK, pid, 0, prefix);

        jinfo("%o", 
            "exec", jobj_oprintf("%s%d%d%s",
                        "name",     entry->name,
                        "normal",   entry->normal,
                        "forks",    entry->forks,
                        "command",  entry->command));

        return 0;
    }
    else {
        simpile_cmd_t *cmd = simpile_cmd_argv(entry->command);
        
        err = execvp(cmd->argv[0], cmd->argv);
        
        debug_error("exec %s error:%d", cmd->argv[0], -errno);
        
        exit(err);
    }
}

static inline void
__die(struct smonitor *entry, char *prefix)
{
    __change(entry, SMONITOR_STATE_DIE, 0, 0, prefix);
}

static inline void
__killall(struct smonitor *entry)
{
    os_system("killall '%s'", entry->name);
}

static inline void
__kill_deamon(struct smonitor *entry)
{
    if (__is_deamon(entry)) {
        __killall(entry);
    }
}

static int
__kill(struct smonitor *entry)
{
    if (false==__have_pid(entry)) {
        return 0;
    }
    
    int pid = __is_normal(entry)?entry->normal:entry->deamon;
    if (os_pid_exist(pid)) {
        kill(pid, SIGKILL);
    }
    
    __kill_deamon(entry);
    
    jinfo("%o", 
        "kill", jobj_oprintf("%s%d%d%d%s",
                    "name",     entry->name,
                    "normal",   entry->normal,
                    "deamon",   entry->deamon,
                    "forks",    entry->forks,
                    "state",    smonitor_state_string(entry->state)));
                    
    __die(entry, "in kill");

    return 0;
}

static void
__wait_error(struct smonitor *entry)
{
    char prefix[1+OS_SHORTLINE_LEN] = {0};
    int pid;

    if (__is_normal(entry)) {
        pid = entry->normal;
    } else {
        pid = entry->deamon;
    }
    
    os_snprintf(prefix, OS_SHORTLINE_LEN, "int wait %s(%s)",
        __type(entry),
        smonitor_state_string(entry->state));
    
    jerror("%o", 
        "wait-error", jobj_oprintf("%s%d%d%s",
                            "name",     entry->name,
                            __type(entry),  pid,
                            "forks",    entry->forks,
                            "state",    smonitor_state_string(entry->state)));

    __kill_deamon(entry);
    __run(entry, prefix);
}

static void
__wait_ok(struct smonitor *entry)
{
    char prefix[1+OS_SHORTLINE_LEN] = {0};
    int pid = 0, normal = 0, deamon = 0;
    
    if (__is_normal(entry)) {
        pid = normal = entry->normal;
    }
    else {
        pid = deamon = __is_run(entry)?entry->deamon:__get_deamon_pid(entry);
    }
    
    os_snprintf(prefix, OS_SHORTLINE_LEN, "int wait %s(%s)",
        __type(entry),
        smonitor_state_string(entry->state));

    if (os_pid_exist(pid)) {
        __change(entry, SMONITOR_STATE_RUN, normal, deamon, prefix);
    } else {
        jinfo("%o", 
            "wait-ok", jobj_oprintf("%s%d%d%s",
                                    "name",     entry->name,
                                    __type(entry),  pid,
                                    "forks",    entry->forks,
                                    "state",    smonitor_state_string(entry->state)));

        __kill_deamon(entry);
        __die(entry, prefix);
        __run(entry, prefix);
    }
}

static void
__wait_son(int pid)
{
    debug_trace("wait son:%d", pid);
    
    struct smonitor *entry = __get_bynormal(pid);
    if (NULL==entry) {
        debug_trace("wait son:%d, but no-found entry, maybe have been removed", pid);

        return;
    }

    if (__is_normal(entry)) {
        jinfo("%o", 
            "wait-son", jobj_oprintf("%s%d%d%s",
                            "name",     entry->name,
                            "normal",   entry->normal,
                            "forks",    entry->forks,
                            "state",    smonitor_state_string(entry->state)));
        char *prefix = "in wait son(normal)";
        
        __die(entry, prefix);
        __run(entry, prefix);
    } else {
        __change(entry, entry->state, 0, 0, "in wait son(deamon)");
    }
}

static int
__wait(void)
{
    int pid;
    struct smonitor *entry;
    char *prefix;

    while((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        __wait_son(pid);
    }
    
    list_for_each_entry(entry, &smonitord.list, node) {
        switch(entry->state) {
            case SMONITOR_STATE_DIE:
            case SMONITOR_STATE_INIT:
                __wait_error(entry);
                
                break;
            case SMONITOR_STATE_FORK:
                if (__is_timeout(entry)) {
                    __wait_ok(entry);
                }
                
                break;
            case SMONITOR_STATE_RUN:
                __wait_ok(entry);
                
                break;
            default:
                break;
        }
    }
    
    return 0;
}

static int
__insert(struct smonitor *entry)
{
    int err, pid = 0;
    
    if (is_in_list(&entry->node)) {
        return -EINLIST;
    }
    
    list_add(&entry->node, &smonitord.list);

    if (__is_normal(entry)) {
        pid = __get_normal_pid(entry);
    } else {
        os_v_sfgeti(&pid, "%s", entry->pidfile);
        
        if (!os_pid_exist(pid)) {
            pid = 0;
        }
    }
    
    if (0==pid) {
        err = __run(entry, "in insert(not exist)");
        if (err) {
            return err;
        }
    } else {
        if (__is_normal(entry)) {
            __change(entry, SMONITOR_STATE_RUN, pid, 0, "in insert(normal exist)");
        } else {
            __change(entry, SMONITOR_STATE_RUN, 0, pid, "in insert(deamon exist)");
        }
    }
    
    return 0;
}

static void
__destroy(struct smonitor *entry)
{
    if (entry) {
        os_free(entry->name);
        os_free(entry->command);
        os_free(entry->pidfile);

        os_free(entry);
    }
}

static int
__remove(struct smonitor *entry)
{
    if (os_hasflag(entry->flags, SMONITOR_F_STATIC)) {
        return 0;
    }
    else if (false==is_in_list(&entry->node)) {
        return -ENOINLIST;
    }

    debug_trace("remove %s:%s", entry->name, entry->command);
    
    list_del(&entry->node);
    __kill(entry);
    __destroy(entry);
    
    return 0;
}

static struct smonitor *
__create(char *name, char *command, char *pidfile)
{
    struct smonitor *entry = (struct smonitor *)os_zalloc(sizeof(*entry));
    if (NULL==entry) {            
        return NULL;
    }

    entry->name     = strdup(name);
    entry->command  = strdup(command);
    if (pidfile) {
        entry->pidfile  = strdup(pidfile);
    }

    __set_time(entry, "in create");
    
    return entry;
}

static int
handle_insert(char *args)
{
    char *type      = args; os_cmd_shift(args);
    char *name      = args; os_cmd_shift(args);
    char *pidfile   = NULL;
    char *command   = NULL;
    int err = 0;

    if (NULL==type) {
        debug_error("NULL type");
        
        return -EINVAL1;
    }
    else if (NULL==name) {
        debug_error("NULL name");
        
        return -EINVAL2;
    }

    if (0==os_strcmp(type, "normal")) {
        command = args;
    }
    else if (0==os_strcmp(type, "deamon")) {
        pidfile = args; os_cmd_shift(args);
        command = args;
    }
    else {
        return -EINVAL3;
    }

    if (NULL==command) {
        debug_error("NULL command");
        
        return -EINVAL4;
    }
    
    struct smonitor *entry = __get_byname(name);
    if (entry) {
        debug_trace("%s exist", name);
        
        return 0;
    }
    
    entry = __create(name, command, pidfile);
    if (NULL==entry) {
        return -ENOMEM;
    }

    err = __insert(entry);
    if (err) {
        __destroy(entry);

        return err;
    }
    
    return 0;
}

static int
handle_remove(char *args)
{
    char *name = args; os_cmd_shift(args);
    if (NULL==name) {
        debug_trace("remove monitor without name");
        
        return -EINVAL5;
    }
    
    struct smonitor *entry = __get_byname(name);
    if (NULL==entry) {
        return -ENOEXIST;
    }

    return __remove(entry);
}

static void
show(struct smonitor *entry)
{
    simpile_res_sprintf("%s %c%d/%d %d %s '%s'" __crlf,
        entry->name,
        os_hasflag(entry->flags, SMONITOR_F_STATIC)?'*':' ',
        entry->normal,
        entry->deamon,
        entry->forks,
        smonitor_state_string(entry->state),
        entry->command);
}

static int
handle_show(char *args)
{
    char *name = args; os_cmd_shift(args);
    struct smonitor *entry;
    bool empty = true;
    
    simpile_res_sprintf("#name pid/dpid forks state command" __crlf);

    list_for_each_entry(entry, &smonitord.list, node) {
        show(entry);

        empty = false;
    }

    if (empty) {
        simpile_res_clear();
        
        debug_trace("show monitor(%s) nofound", name);
    }
    
    return 0;
}

static int
__server_handle(fd_set *r)
{
    static cmd_table_t table[] = {
        CMD_ENTRY("insert",  handle_insert),
        CMD_ENTRY("remove",  handle_remove),
        CMD_ENTRY("show",    handle_show),
    };
    int err = 0;

    if (FD_ISSET(smonitord.server.fd, r)) {
        err = simpile_d_handle(smonitord.server.fd, table);
    }
    
    return err;
}

static int
server_handle(void)
{
    fd_set rset;
    struct timeval tv = {
        .tv_sec     = time_sec(smonitord.timeout),
        .tv_usec    = time_usec(smonitord.timeout),
    };
    int err;

    while(1) {
        FD_ZERO(&rset);
        FD_SET(smonitord.server.fd, &rset);
        
        err = select(smonitord.server.fd+1, &rset, NULL, NULL, &tv);
        switch(err) {
            case -1:
                if (EINTR==errno) {
                    debug_event("select breaked");
                    continue;
                } else {
                    debug_trace("select error:%d", -errno);
                    return -errno;
                }
            case 0:
                debug_timeout("select timeout");
                return -ETIMEOUT;
            default:
                return __server_handle(&rset);
        }
    }

    return 0;
}


static int
init_env(void) 
{
    int err;
    
    smonitord.timeout = get_smonitor_timeout_env();

    err = get_smonitord_path_env(&smonitord.server.addr);
    if (err) {
        return err;
    }

    return 0;
}

static int
init_server(void)
{
    int fd;
    int err;
    
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd<0) {
    	debug_error("socket error:%d", -errno);
        return -errno;
    }
    os_closexec(fd);
    
    err = bind(fd, (struct sockaddr *)&smonitord.server.addr, get_abstract_sockaddr_len(&smonitord.server.addr));
    if (err) {
        debug_error("bind error:%d", -errno);
        return -errno;
    }
    
    smonitord.server.fd = fd;
    
    debug_ok("init server ok");
    
    return 0;
}

static int 
init_timer(void)
{
    struct itimerval itimer = OS_ITIMEVAL_INITER(SMONITOR_TIMER, 0);
    
    int err = setitimer(ITIMER_REAL, &itimer, NULL);
    if (err) {
        debug_error("init timer error:%d", -errno);
        
        return -errno;
    }
    
    return 0;
}

static int
load(void)
{
    int i;

    for (i=0; i<os_count_of(daemons); i++) {
        debug_trace("load static %s", daemons[i].name);
        
        __insert(&daemons[i]);
    }

    if (os_file_exist(SCRIPT_SMONITOR_INIT)) {
        os_system("(sleep 3;" SCRIPT_SMONITOR_INIT ")&");

        debug_trace(".smonitor.init");
    }

    return 0;
}

static int
__fini(void)
{
    os_fini();

    return 0;
}

static int
__init(void)
{
    int err;
    
    err = os_init();
    if (err) {
        return err;
    }

    err = init_env();
    if (err) {
        return err;
    }

    err = init_timer();
    if (err) {
        return err;
    }
    
    err = init_server();
    if (err) {
        return err;
    }

    return 0;
}

static void
__timer(int sig)
{
    smonitord.time++;
}

static void
__exit(int sig)
{
    __fini();
    
    exit(sig);
}

static int 
__main(int argc, char *argv[])
{
    load();
    
    while (1) {
        server_handle();
        __wait();
    }
    
    return 0;
}

#ifndef __BUSYBOX__
#define smonitord_main  main
#endif

int smonitord_main(int argc, char *argv[])
{
    os_setup_signal_exit(__exit);
    os_setup_signal_timer(__timer);
    os_setup_signal_callstack(NULL);
    
    return os_call(__init, __fini, __main, argc, argv);
}
/******************************************************************************/
