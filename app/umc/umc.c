#ifndef __THIS_APP
#define __THIS_APP      umc
#endif

#ifndef __THIS_FILE
#define __THIS_FILE     "umc"
#endif

#define USE_INLINE_SIMPILE_RES

#include "utils.h"
#include "um/um.h"

OS_INITER;
DECLARE_SIMPILE_RES;

static char *self;
static simpile_client_t umc = SIMPILE_CLIENT_INITER(UM_TIMEOUT, UMD_UNIX);

static int
usage(int error)
{
#if USE_USAGE
    os_fprintln(stderr, "%s bind   {mac} {ip}", self);
    os_fprintln(stderr, "%s unbind {mac}", self);
    os_fprintln(stderr, "%s auth   {mac} {group} {json}", self);
    os_fprintln(stderr, "%s reauth {mac}", self);
    os_fprintln(stderr, "%s deauth {mac}", self);
    os_fprintln(stderr, "%s tag    {mac} {key} [value]", self);
#if UM_USE_MONITOR
    os_fprintln(stderr, "%s enter  {mac} {json}", self);
    os_fprintln(stderr, "%s leave  {mac}", self);
#endif
#if UM_USE_GC
    os_fprintln(stderr, "%s gc", self);
#endif

    os_fprintln(stderr, "%s show [stat | json]", self);
#endif

    return error;
}

#define umc_handle(_action, _argc, _argv) \
    simpile_c_handle(_action, true, _argc, _argv, &umc.server, &umc.client, umc.timeout)

static int
cmd_online(int argc, char *argv[])
{
    if (0!=argc) {
        return -EFORMAT;
    }

    return umc_handle("online", argc, argv);
}

static int
cmd_offline(int argc, char *argv[])
{
    if (0!=argc) {
        return -EFORMAT;
    }

    return umc_handle("offline", argc, argv);
}

#if UM_USE_MONITOR
static int
cmd_enter(int argc, char *argv[])
{
    char *mac   = argv[0];
    char *json  = argv[1];
    
    if (2!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }
    else if (false==is_good_json(json)) {
        debug_trace("bad json %s", json);

        return -EFORMAT;
    }
    
    return umc_handle("enter", argc, argv);
}

static int
cmd_leave(int argc, char *argv[])
{
    char *mac= argv[0];
    
    if (1!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }

    return umc_handle("leave", argc, argv);
}
#endif

static int
cmd_bind(int argc, char *argv[])
{
    char *mac= argv[0];
    char *ip = argv[1];
    
    if (2!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }
    else if (false==is_good_ipstring(ip)) {
        debug_trace("bad ip %s", ip);

        return -EFORMAT;
    }

    return umc_handle("bind", argc, argv);
}

static int
cmd_unbind(int argc, char *argv[])
{
    char *mac= argv[0];
    
    if (1!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }

    return umc_handle("unbind", argc, argv);
}

static int
cmd_auth(int argc, char *argv[])
{
    char *mac   = argv[0];
    char *group = argv[1];
    char *json  = argv[2];
    
    if (3!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }
    else if (false==is_good_json(json)) {
        debug_trace("bad json %s", json);

        return -EFORMAT;
    }
    
    return umc_handle("auth", argc, argv);
}

static int
cmd_reauth(int argc, char *argv[])
{
    char *mac = argv[0];
    
    if (1!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }

    return umc_handle("reauth", argc, argv);
}

static int
cmd_deauth(int argc, char *argv[])
{
    char *mac = argv[0];
    
    if (1!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }

    return umc_handle("deauth", argc, argv);
}

static int
show_by(int argc, char *argv[])
{
    char *json = argv[0];

    if (is_good_json(json) || 0==os_strcmp("stat", json)) {
        return umc_handle("show", argc, argv);
    }
    else {
        debug_error("bad json %s", json);
        
        return -EFORMAT;
    }
}

static int
cmd_show(int argc, char *argv[])
{
    switch(argc) {
        case 0:
            return umc_handle("show", argc, argv);
        case 1: {
            char *json = argv[0];

            if (is_good_json(json) || 0==os_strcmp("stat", json)) {
                return umc_handle("show", argc, argv);
            }
            else {
                debug_error("bad json %s", json);
                
                return -EFORMAT;
            }
        }
        default:
            return usage(-EHELP);
    }
}

static int
cmd_tag(int argc, char *argv[])
{
    char *mac = argv[0];

    if (2!=argc && 3!=argc) {
        return -EFORMAT;
    }
    else if (false==is_good_macstring(mac)) {
        debug_trace("bad mac %s", mac);

        return -EFORMAT;
    }

    return umc_handle("tag", argc, argv);
}

static int
cmd_gc(int argc, char *argv[])
{
    if (0!=argc) {
        return -EFORMAT;
    }

    return umc_handle("gc", argc, argv);
}

static int
command(int argc, char *argv[])
{
    static cmd_table_t table[] = {
#if UM_USE_MONITOR
        CMD_ENTRY("enter",  cmd_enter),
        CMD_ENTRY("leave",  cmd_leave),
#endif
        CMD_ENTRY("bind",   cmd_bind),
        CMD_ENTRY("unbind", cmd_unbind),
        CMD_ENTRY("auth",   cmd_auth),
        CMD_ENTRY("reauth", cmd_reauth),
        CMD_ENTRY("deauth", cmd_deauth),
        CMD_ENTRY("show",   cmd_show),
        CMD_ENTRY("tag",    cmd_tag),
#if UM_USE_GC
        CMD_ENTRY("gc",     cmd_gc),
#endif
    };
    int err;

    err = cmd_argv_handle(table, os_count_of(table), argc, argv);
    if (err) {
        debug_error("%s error:%d", argv[0], err);

        return err;
    }
    
    return 0;
}

static int
init_env(void) 
{
    int err;
    
    umc.timeout = get_um_timeout_env();

    err = get_umc_path_env(&umc.client);
    if (err) {
        return err;
    }
    
    err = get_umd_path_env(&umc.server);
    if (err) {
        return err;
    }

    return 0;
}

static int 
__main(int argc, char *argv[])
{
    int err;
    
    self = os_cmd_trace_argv(argc, argv);
    
    if (1==argc) {
        return usage(-EHELP);
    }
    
    err = init_env();
        debug_trace_error(err, "init env");
    if (err) {
        return err;
    }
    
    err = command(argc-1, argv+1);
    if (err) {
        /* just log, NOT return */
    }

    return 0;
}

#ifndef __BUSYBOX__
#define umc_main  main
#endif

int umc_main(int argc, char *argv[])
{
    os_setup_signal_exit(NULL);
    os_setup_signal_callstack(NULL);
    
    return os_main(__main, argc, argv);
}

/******************************************************************************/
