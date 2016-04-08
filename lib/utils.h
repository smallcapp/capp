#ifndef __LIB_UTILS_H__
#define __LIB_UTILS_H__
/******************************************************************************/
#ifdef __FASTBOOT__
#define __BOOT__
#endif

#ifdef __BOOT__
#   include <malloc.h>
#   include <command.h>
#   include <common.h>
#   include <linux/ctype.h>
#   include <linux/string.h>
#   ifndef __AKID_DEBUG
#       define __AKID_DEBUG    __boot_debuger
#   endif
#elif defined(__KERNEL__)
#   include <linux/moduleparam.h>
#   include <linux/module.h>
#   include <linux/skbuff.h>
#   include <linux/gfp.h>
#   include <linux/init.h>
#   include <linux/in.h>
#   include <linux/inet.h>
#   include <linux/netlink.h>
#   include <linux/rtc.h>
#   include <linux/ctype.h>
#   include <linux/string.h>
#   include <linux/time.h>
#   include <linux/errno.h>
#   include <linux/list.h>
#   include <net/sock.h>
#else /* __APP__ */
#   define __APP__
#   ifdef __BUSYBOX__
#       include "libbb.h"
#   endif
#   include <stdint.h>
#   include <stdarg.h>
#   include <stdlib.h>
#   include <stdio.h>
#   include <signal.h>
#   include <string.h>
#   include <malloc.h>
#   include <assert.h>
#   include <unistd.h>
#   include <fcntl.h>
#   include <ctype.h>
#   include <errno.h>
#   include <time.h>
#   include <utime.h>
#   include <dirent.h>
#   include <syslog.h>
#   include <ucontext.h>
#   include <byteswap.h>
#   include <libgen.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <sys/file.h>
#   include <sys/epoll.h>
#   include <sys/wait.h>
#   include <sys/ioctl.h>
#   include <sys/ipc.h>
#   include <sys/msg.h>
#   include <sys/sem.h>
#   include <sys/shm.h>
#   include <sys/socket.h>
#   include <sys/un.h>
#   include <sys/mman.h>
#   include <sys/time.h>
#   include <sys/timerfd.h>
#   include <netpacket/packet.h>
#   include <netinet/ether.h>
#   include <netinet/in.h>
#   include <netinet/ip.h>
#   include <netinet/tcp.h>
#   include <netinet/udp.h>
#   include <arpa/inet.h>
#   include <net/if.h>
#   include <linux/netlink.h>
#endif /* !__APP__ */

#ifndef __THIS_APP
#error "must defined __THIS_APP before all #include"
#endif

#define __SYMBOL_TO_STRING2(x)  #x
#define __SYMBOL_TO_STRING(x)   __SYMBOL_TO_STRING2(x)

#define __SYMBOL_TO_VAR2(_prefix, _name)    __##_prefix##_name
#ifdef __BUSYBOX__
#define __SYMBOL_TO_VAR(_prefix, _name)     __this##_name
#else
#define __SYMBOL_TO_VAR(_prefix, _name)     __SYMBOL_TO_VAR2(_prefix, _name)
#endif

#ifndef __THIS_NAME
#define __THIS_NAME             __SYMBOL_TO_STRING(__THIS_APP)
#endif

#ifndef __THIS_FILE
#define __THIS_FILE             __THIS_NAME
#endif

#ifndef __THIS_LOCKFD
#define __THIS_LOCKFD           __SYMBOL_TO_VAR(__THIS_APP, _lock_fd)
#endif

#ifndef __THIS_LOCKFILE
#define __THIS_LOCKFILE         "/tmp/." __THIS_NAME ".lock"
#endif

#ifndef __THIS_DEAMON_RUNNING
#define __THIS_DEAMON_RUNNING   __SYMBOL_TO_VAR(__THIS_APP, _deamon_running)
#endif

#ifndef __THIS_OEM
#define __THIS_OEM              __SYMBOL_TO_VAR(__THIS_APP, _oem)
#endif

#ifndef __THIS_PIDFILE
#define __THIS_PIDFILE          "/tmp/." __THIS_NAME ".pid"
#endif

#ifndef __AKID_DEBUG
#define __AKID_DEBUG            __SYMBOL_TO_VAR(__THIS_APP, _debugger)
#endif

#ifndef USE_USAGE
#define USE_USAGE               1
#endif

#include "utils/type.h"
#include "utils/base.h"
#include "utils/bits.h"
#include "utils/assert.h"
#include "utils/errno.h"
#include "utils/number.h"
#include "utils/memory.h"
#include "utils/list.h"
#include "utils/string.h"
#include "utils/printf.h"
#include "utils/env.h"
#include "utils/appkey.h"
#include "utils/jsonapi.h"
#include "utils/socket.h"
#include "utils/jlog.h"
#include "utils/debug.h"
#include "utils/dump.h"
#include "utils/time.h"
#include "utils/sem.h"
#include "utils/shm.h"
#include "utils/sfile.h"
#include "utils/mac.h"
#include "utils/pid.h"
#include "utils/ip.h"
#include "utils/if.h"
#include "utils/io.h"
#include "utils/cmd.h"
#include "utils/timer.h"
#include "oem/oem.h"
/******************************************************************************/
#define OS_INITER               \
    DECLARE_DEAMON_RUNNING;     \
    DECLARE_FLOCK;              \
    DECLARE_JLOG;               \
    DECLARE_OEM;                \
    DECLARE_DEBUGGER /* last */ \
    /* end */

static inline int
os_init(void)
{
    int err;
    
    err = appkey_init();
    if (err) {
        return err;
    }
    
    err = jlog_init();
    if (err) {
        return err;
    }

    err = os_deamon_check();
    if (err) {
        return err;
    }
    
    return 0;
}

static inline int
os_fini(void)
{
    os_deamon_exit();
    jlog_fini();
    appkey_fini();

    return 0;
}

static inline int
os_main(int (*func)(int, char **), int argc, char **argv)
{
    return os_call(os_init, os_fini, func, argc, argv);
}
/******************************************************************************/
#endif /* __LIB_UTILS_H__ */
