#ifndef __THIS_APP
#define __THIS_APP      atcheck
#endif

#include "utils.h"
#include "atenv/atenv.h"
#include "atdog/atdog.h"
#if 1==AT_LTEFI_VERSION
#include "../bootm/bootm.h"
#endif
#if !AT_LTEFI_MD
#error "just support ltefi md board"
#endif

OS_INITER;

static int fd_boot      = INVALID_FD;
static int fd_bootenv   = INVALID_FD;
static int fd_cmdline   = INVALID_FD;

#define AT_CHECK_DEBUG  0
#if AT_CHECK_DEBUG
#define dprintln(_fmt, _args...)  os_println(_fmt, ##_args)
#else
#define dprintln(_fmt, _args...)  os_do_nothing()
#endif

static int
__fini(void)
{
    os_close(fd_cmdline);
    os_close(fd_bootenv);
    os_close(fd_boot);

    os_fini();

    return 0;
}

static int
__init(void)
{
    int fd, err;
    int permit = S_IRUSR | S_IRGRP;
        
    err = os_init();
    if (err) {
        return err;
    }

    do {
        fd = open(DEV_BOOT, O_RDONLY, permit);
        if(false==is_good_fd(fd)) {
            trace_error(-errno, "open " DEV_BOOT);
            
            sleep(1);
        }
    }while(false==is_good_fd(fd));
    fd_boot = fd;

    fd = open(DEV_BOOTENV, O_RDONLY, permit);
    if (false==is_good_fd(fd)) {
        trace_error(-errno, "open " AT_DEV_BOOTENV);
        
        return -errno;
    }
    fd_bootenv = fd;

    fd = open(AT_PROC_CMDLINE, O_RDONLY, permit);
    if (false==is_good_fd(fd)) {
        trace_error(-errno, "open " AT_PROC_CMDLINE);
        
        return -errno;
    }
    fd_cmdline = fd;

    return 0;
}

static inline int
hacked(int obj, int reason)
{
    os_println("system is hacked(%d.%d), reboot...", obj, reason);
    
    __os_system("reboot &");
    
    return -EHACKED;
}

static int
check_boot(void)
{
#if 2==AT_LTEFI_VERSION
    static at_reg_t dog_enable[] = AT_REG_DOG_ENABLE;
#endif
    static struct {
        char *addr;
        int size;
    } cookie[] = {
#if 2==AT_LTEFI_VERSION
        {
            .addr = (char *)dog_enable,
            .size = sizeof(dog_enable),
        },
#endif
        {
            .addr = CONFIG_BOOTARGS,
            .size = sizeof(CONFIG_BOOTARGS),
        },
    };

    dprintln("before check boot");
    
    static char boot[AT_BOOT_SIZE];
    if (AT_BOOT_SIZE != read(fd_boot, boot, AT_BOOT_SIZE)) {
        trace_error(-errno, "read boot");

        return -errno;
    }
    
    char *p     = boot;
    char *end   = boot + AT_BOOT_SIZE;
    int i;
    
    for (i=0; i<os_count_of(cookie); i++) {
        p = os_memmem(p, end - p, cookie[i].addr, cookie[i].size);
        if (NULL==p) {
            return hacked(1, i);
        }
    }
    dprintln("after  check boot");
    
    return 0;
}

static int
check_bootenv(void)
{
#if 1==AT_LTEFI_VERSION
    struct {
        char init[sizeof(AT_DEFT_INIT)-1];
    } cookie = {
        {0},
    }, deft = {
        AT_DEFT_INIT,
    };
#elif 2==AT_LTEFI_VERSION
    at_cookie_t deft = AT_DEFT_COOKIE;
    at_cookie_t cookie;
#endif

    dprintln("before check bootenv");
    
    if (sizeof(cookie) != read(fd_bootenv, &cookie, sizeof(cookie))) {
        trace_error(-errno, "read atenv cookie");

        return -errno;
    }

    if (false==os_objeq(&cookie, &deft)) {
        return hacked(2, 0);
    }

    dprintln("after  check bootenv");

    return 0;
}


static int
check_partition(void)
{
    char line[1+OS_LINE_LEN] = {0};
    
    dprintln("before check partition");
    
    read(fd_cmdline, line, OS_LINE_LEN);
    
    if (NULL==os_strstr(line, CONFIG_BOOTARGS_BODY)) {
        return hacked(3, 0);
    }

    dprintln("after  check partition");

    return 0;
}

#ifndef __BUSYBOX__
#define atcheck_main  main
#endif

static int
__main(int argc, char *argv[])
{
    static int (*check[])(void) = {
        check_boot,
        check_bootenv,
        check_partition,
    };
    
    int i, err;

    for (i=0; i<os_count_of(check); i++) {
        err = (*check[i])();
        if (err) {
            return err;
        }
    }
    
    return 0;
}

int atcheck_main(int argc, char *argv[])
{
    os_setup_signal_exit(NULL);
    os_setup_signal_callstack(NULL);
    
    return os_call(__init, __fini, __main, argc, argv);
}
/******************************************************************************/
