#ifndef __THIS_APP
#define __THIS_APP      stimerd
#endif

#define __DEAMON__
#define USE_INLINE_TIMER
#define USE_INLINE_SIMPILE_RES

#include "utils.h"
#include "stimer/stimer.h"

OS_INITER;
DECLARE_TIMER;
DECLARE_SIMPILE_RES;

#ifdef __BUSYBOX__
DECLARE_REAL_TIMER;
DECLARE_REAL_SIMPILE_RES;
#endif

static struct {    
    struct {
        int timeout;
        
        int fd;
    } timer;

    struct {
        int fd;

        struct sockaddr_un addr;
    } server;

    struct {
        struct mlist_table mlist;
    } head;
}
stimerd = {
    .timer = {
        .timeout= STIMER_TIMEOUT,
        .fd     = INVALID_FD,
    },

    .server = {
        .fd     = INVALID_FD,
        
        .addr   = {
            .sun_family = AF_UNIX,
        },
    },
};

struct stimer {
    char name[1+STIMER_NAMESIZE];
    char command[1+STIMER_CMDSIZE];
    
    int delay;
    int interval;
    int limit;
    
    int triggers;

    struct {
        struct mlist_node   mlist;
        tm_node_t timer;
    } node;
};

static inline struct stimer *
__tm_entry(tm_node_t *timer)
{
    return container_of(timer, struct stimer, node.timer);
}

static inline struct stimer *
__mlist_entry(struct mlist_node *node)
{
    return container_of(node, struct stimer, node.mlist);
}

static inline uint32_t 
__hash(char *name)
{
    return __string_bkdr(name) & (stimerd.head.mlist.size - 1);
}

static int
__insert(struct stimer *entry)
{
    if (NULL==entry) {
        return -EKEYNULL;
    }

    struct mlist_node *node = 
        __mlist_insert(&stimerd.head.mlist, &entry->node.mlist, lanmbda(int, (struct mlist_node *node) {
            return __hash(entry->name);
        }));

    return node?0:-EINLIST;
}

static int
__remove(struct stimer *entry)
{
    if (NULL==entry) {
        return -EKEYNULL;
    }

    os_tm_remove(&entry->node.timer);

    return mlist_remove(&stimerd.head.mlist, &entry->node.mlist);
}

static struct stimer *
__get(char *name)
{
    if (NULL==name) {
        return NULL;
    }

    struct mlist_node *node = mlist_find(&stimerd.head.mlist, 
        lanmbda(int, (void) {
            return __hash(name);
        }), 
        lanmbda(bool, (struct mlist_node *node) {
            struct stimer *entry = __mlist_entry(node);
            
            return 0==os_stracmp(entry->name, name);
        }));
    if (NULL==node) {
        return NULL;
    }

    return __mlist_entry(node);
}

static int
__foreach(multi_value_t (*cb)(struct stimer *entry))
{
    return mlist_foreach(&stimerd.head.mlist, lanmbda(multi_value_t, (struct mlist_node *node){
        struct stimer *entry = __mlist_entry(node);

        return (*cb)(entry);
    }));
}

static struct stimer *
__create(char *name, char *command, int delay, int interval, int limit)
{
    struct stimer *entry = __get(name);

    if (NULL==entry) {
        entry = (struct stimer *)os_zalloc(sizeof(*entry));
        if (NULL==entry) {
            return NULL;
        }
    
        os_strdcpy(entry->name, name);
    } else if (false==os_tm_is_pending(&entry->node.timer)) {
        os_tm_remove(&entry->node.timer);
    } else {
        return entry;
    }
    
    os_strdcpy(entry->command, command);
    
    entry->delay    = delay;
    entry->interval = interval;
    entry->limit    = limit;
    
    entry->triggers = 0;
    
    __insert(entry);

    return entry;
}

static void
__destroy(struct stimer *entry)
{
    if (entry) {
        __remove(entry);
        os_tm_remove(&entry->node.timer);
        os_free(entry);
    }
}

static bool
__is_cycle(struct stimer *entry)
{
    if (entry->interval) {
        return entry->limit?false:true;
    } else {
        return false;
    }
}

static multi_value_t 
stimer_cb(tm_node_t *timer)
{
    struct stimer *entry = __tm_entry(timer);
    
    entry->triggers++;
    os_system("%s &", entry->command);

    debug_trace("trigger timer:%s"
                ", command:%s"
                ", delay:%d"
                ", interval:%d"
                ", limit:%d"
                ", triggers:%d",
                entry->name,
                entry->command,
                entry->delay,
                entry->interval,
                entry->limit,
                entry->triggers);

    if (__is_cycle(entry) || entry->triggers < entry->limit) {
        debug_trace("re-insert timer:%s", entry->name);
        
        os_tm_insert(&entry->node.timer, 
            time_ms2tick(1000*entry->interval, STIMER_TICKS),
            stimer_cb, 
            false);
    }

    return tm_SAFE(0);
}

static int
handle_insert(char *args)
{
    char *name      = args; os_cmd_shift(args);
    char *delay     = args; os_cmd_shift(args);
    char *interval  = args; os_cmd_shift(args);
    char *limit     = args; os_cmd_shift(args);
    char *command   = args;
    int err;
    
    if (NULL==name      ||
        NULL==delay     ||
        NULL==interval  ||
        NULL==limit     ||
        NULL==command) {
        debug_error("NULL name|delay|interval|limit|command");
        
        return -EINVAL1;
    }

    debug_test("name:%s delay:%s interval:%s, limit:%s, command:%s", 
        name, 
        delay,
        interval,
        limit,
        command);

    int i_delay     = os_atoi(delay);
    int i_interval  = os_atoi(interval);
    int i_limit     = os_atoi(limit);
    
    if (false==is_good_stimer_args(i_delay, i_interval, i_limit)) {
        debug_error("invalid args, delay:%d, interval:%d, limit:%d",
            i_delay, i_interval, i_limit);
        
        return -EINVAL2;
    }

    struct stimer *entry = __create(name, command, i_delay, i_interval, i_limit);
    if (NULL==entry) {
        return -ENOMEM;
    }

    int after = i_delay + i_interval;
    
    debug_trace("insert timer:%s"
                ", command:%s"
                ", delay:%d"
                ", interval:%d"
                ", limit:%d"
                ", after:%d"
                ", cycle:%d"
                ", pending:%s",
                entry->name,
                entry->command,
                entry->delay,
                entry->interval,
                entry->limit,
                after,
                __is_cycle(entry),
                os_tm_is_pending(&entry->node.timer)?__true:__false);
    
    err = os_tm_insert(&entry->node.timer, 
            time_ms2tick(1000*after, STIMER_TICKS),
            stimer_cb, 
            false);
    if (err) {
        return -EEXIST;
    }

    return 0;
}

static int
handle_remove(char *args)
{
    char *name = args; os_cmd_shift(args);
    if (NULL==name) {
        debug_trace("remove timer without name");
        
        return -EINVAL3;
    }
    
    struct stimer *entry = __get(name);
    if (NULL==entry) {
        debug_trace("remove timer(%s) nofound", name);
        
        return -ENOEXIST;
    }

    debug_trace("remove timer(%s)", name);
    
    __destroy(entry);
    
    return 0;
}

static void
show(struct stimer *entry)
{
    simpile_res_sprintf("%s %d %d %d %d %d %s" __crlf,
        entry->name,
        entry->delay,
        entry->interval,
        entry->limit,
        entry->triggers,
        os_tm_left(&entry->node.timer) * STIMER_TICKS / 1000,
        entry->command);
}

static int
handle_show(char *args)
{
    char *name = args; os_cmd_shift(args);
    bool empty = true;
    
    simpile_res_sprintf("#name delay interval limit triggers left command" __crlf);
    
    __foreach(lanmbda(multi_value_t, (struct stimer *entry) {
        if (NULL==name || 0==os_stracmp(entry->name, name)) {
            show(entry);

            empty = false;
        }

        return mv2_OK;
    }));
    
    if (empty) {
        simpile_res_clear();

        debug_trace("show timer(%s) nofound", name);
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
    
    if (FD_ISSET(stimerd.timer.fd, r)) {
        err = os_timerfd_trigger(stimerd.timer.fd);
    }

    if (FD_ISSET(stimerd.server.fd, r)) {
        err = simpile_d_handle(stimerd.server.fd, table);
    }

    return err;
}

static int
server_handle(void)
{
    fd_set rset;
    struct timeval tv = {
        .tv_sec     = time_sec(stimerd.timer.timeout),
        .tv_usec    = time_usec(stimerd.timer.timeout),
    };
    int maxfd = os_max(stimerd.timer.fd, stimerd.server.fd);
    int err;

    while(1) {
        FD_ZERO(&rset);
        FD_SET(stimerd.timer.fd, &rset);
        FD_SET(stimerd.server.fd, &rset);
        
        err = select(maxfd+1, &rset, NULL, NULL, &tv);
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
}

static int
init_env(void) 
{
    int err;
    
    stimerd.timer.timeout   = get_stimer_timeout_env();
    os_tm_unit_set(STIMER_TICKS);

    err = get_stimerd_path_env(&stimerd.server.addr);
    if (err) {
        return err;
    }

    return 0;
}

static int
init_timerfd(void)
{
    int fd = os_timerfd(time_sec(STIMER_TICKS), time_nsec(STIMER_TICKS));
    if (fd<0) {
        return fd;
    } else {
        stimerd.timer.fd = fd;

        return 0;
    }
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
    
    err = bind(fd, (struct sockaddr *)&stimerd.server.addr, get_abstract_sockaddr_len(&stimerd.server.addr));
    if (err) {
        debug_error("bind error:%d", -errno);
        return -errno;
    }

    stimerd.server.fd = fd;

    return 0;
}

#ifndef STIMERD_HASHSIZE
#define STIMERD_HASHSIZE    1024
#endif

static int
__fini(void)
{
    os_tm_fini();
    os_fini();
    
    return 0;
}

static int
load(void)
{
    return 0;
}

static void
__exit(int sig)
{
    __fini();
    
    exit(sig);
}

static int
__init(void)
{
    int err;
    
    err = os_init();
    if (err) {
        return err;
    }

    os_tm_init();
    
    err = mlist_table_init(&stimerd.head.mlist, STIMERD_HASHSIZE);
        debug_trace_error(err, "__init timerfd");

    load();
    
    err = init_env();
        debug_trace_error(err, "init env");
    if (err) {
        return err;
    }

    err = init_timerfd();
        debug_trace_error(err, "init timerfd");
    if (err) {
        return err;
    }
    
    err = init_server();
        debug_trace_error(err, "init server");
    if (err) {
        return err;
    }
    
    return 0;
}

static int 
__main(int argc, char *argv[])
{
    while (1) {
        server_handle();
    }
    
    return 0;
}

#ifndef __BUSYBOX__
#define stimerd_main  main
#endif

int stimerd_main(int argc, char *argv[])
{    
    os_setup_signal_exit(NULL);
    os_setup_signal_callstack(NULL);
    
    return os_call(__init, __fini, __main, argc, argv);
}
/******************************************************************************/
