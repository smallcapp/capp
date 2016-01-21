#ifndef __THIS_APP
#define __THIS_APP      umd
#endif

#ifndef __THIS_FILE
#define __THIS_FILE     "umd"
#endif

#define __DEAMON__
#include "utils.h"
#include "umd.h"

OS_INITER;

extern simpile_server_t um_cli_server;
extern simpile_server_t um_flow_server;
extern simpile_server_t um_timer_server;

struct um_control umd = {
    .head   = {
        .mac    = {HLIST_HEAD_INIT},
        .ip     = {HLIST_HEAD_INIT},
        .list   = LIST_HEAD_INIT(umd.head.list),
    },

    .server = {
        [UM_SERVER_TIMER]   = &um_timer_server,
        [UM_SERVER_CLI]     = &um_cli_server,
        [UM_SERVER_FLOW]    = &um_flow_server,
    },

    .intf = {
        [UM_INTF_FLOW] = {
            .name = UM_INTF_FLOW_DEFT,
        },
        [UM_INTF_TC] = {
            .name = UM_INTF_TC_DEFT,
        },
    },

    .gc = UMD_GC,
};

static int
um_intf_init(void)
{
    int err;
    struct um_intf *intf;

    intf = &umd.intf[UM_INTF_FLOW];
    {
        intf->index = if_nametoindex(intf->name);

        err = os_intf_get_mac(intf->name, intf->mac);
        if (err) {
            return err;
        }
        os_maccpy(umd.basemac, intf->mac);
    }

    intf = &umd.intf[UM_INTF_TC];
    {
        intf->index = if_nametoindex(intf->name);
        
        err = os_intf_get_mac(intf->name, intf->mac);
        if (err) {
            return err;
        }
        
        err = os_intf_get_ip(intf->name, &intf->ip);
        if (err) {
            return err;
        }
        
        err = os_intf_get_netmask(intf->name, &intf->mask);
        if (err) {
            return err;
        }

#if UM_USE_PROMISC
        err = os_intf_set_promisc(intf->name);
        if (err) {
            return err;
        }
#endif
    }
    
    return 0;
}

static void
__umevent_fini(void)
{
    if (false==umd.deinit) {
        umd.deinit = true;
        
        os_system(UM_SCRIPT_EVENT " fini");
    }
}

static int
__fini(void)
{
    __umevent_fini();
    os_fini();

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

    err = um_cfg_init();
    if (err) {
        return err;
    }
    
    err = um_intf_init();
    if (err) {
        return err;
    }

    err = simpile_server_init(umd.server);
    if (err) {
        return err;
    }

    os_system(UM_SCRIPT_EVENT " init");

    return 0;
}

static int
__main(int argc, char **argv)
{
    int err = 0;

    update_limit_test();

    while(1) {
        simpile_server_handle(umd.server);
    }
    
    return err;
}

#ifndef __BUSYBOX__
#define umd_main  main
#endif

int umd_main(int argc, char **argv)
{
    os_setup_signal_exit(__exit);
    os_setup_signal_callstack(NULL);
    
    return os_call(__init, __fini, __main, argc, argv);
}
/******************************************************************************/
