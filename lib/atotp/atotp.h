#ifndef __AT_OTP_H_739039F132EB8833CE84495D183364C7__
#define __AT_OTP_H_739039F132EB8833CE84495D183364C7__
#include "atenv/atenv.h"

#ifdef __APP__
/******************************************************************************/
#if AT_LTEFI_MD
#include "hi_unf_otp.h"
#include "hi_unf_advca.h"
#endif

#define AT_OTP_SIZE         16

#ifndef AT_OTP_ZERO
#define AT_OTP_ZERO         "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
#endif

#ifndef AT_OTP_PRIVATE
#define AT_OTP_PRIVATE      "\x73\x90\x39\xF1\x32\xEB\x88\x33\xCE\x84\x49\x5D\x18\x33\x64\xC7"
#endif

#ifndef __AT_OTP_CUSTOM
#define __AT_OTP_CUSTOM     "www.autelan.com"
#endif

#ifndef AT_OTP_CUSTOM
#define AT_OTP_CUSTOM       "538050839"
#endif

enum {
    AT_OTP_ERR_HACKED   = 1,
    AT_OTP_ERR_READ     = 2,
    AT_OTP_ERR_WRITE    = 3,
};

#define AT_OTP_DEBUG    0

#if AT_OTP_DEBUG
#define dprintln(_fmt, _args...)  os_println(_fmt, ##_args)
#else
#define dprintln(_fmt, _args...)  os_do_nothing()
#endif

#define __otp_eq(_a, _b)    os_memeq(_a, _b, AT_OTP_SIZE)

static inline char *
__otp_string(unsigned char otp[AT_OTP_SIZE])
{
    static char string[2*AT_OTP_SIZE+1];

    os_sprintf(string,
        "%.2X%.2X%.2X%.2X"
        "%.2X%.2X%.2X%.2X"
        "%.2X%.2X%.2X%.2X"
        "%.2X%.2X%.2X%.2X"
        ,
        otp[0],  otp[1],  otp[2],  otp[3],
        otp[4],  otp[5],  otp[6],  otp[7],
        otp[8],  otp[9],  otp[10], otp[11],
        otp[12], otp[13], otp[14], otp[15]);

    return string;
}

static inline unsigned char *
__otp_get_bystring(unsigned char otp[AT_OTP_SIZE], char *string)
{
    int i, len;

    len = os_strlen(string);
    if (2*AT_OTP_SIZE != len) {
        return NULL;
    }

    for (i=0; i<AT_OTP_SIZE; i++) {
        otp[i] = os_digitstring2number(string + 2*i, 2, 16, int);
    }
    
    return otp;
}

static inline void
__otp_dump(unsigned char otp[AT_OTP_SIZE])
{
    os_println("%s", __otp_string(otp));
}

#if AT_LTEFI_AP
#define __otp_init()        0
#define __otp_fini()        0

#ifndef AT_OTP_FILE
#define AT_OTP_FILE         "/dev/mtdblock1"
#endif

#ifndef AT_OTP_FILE_SIZE
#define AT_OTP_FILE_SIZE    (512*1024)
#endif

#ifndef AT_OTP_COOKIE_HEAD
#define AT_OTP_COOKIE_HEAD  "\x01\x03\x05\x07\x09\x0b\x0d\x0f\x11\x13\x15\x17\x19\x1b\x1d\x1f"
#endif

#ifndef AT_OTP_COOKIE_BODY
#define AT_OTP_COOKIE_BODY  AT_OTP_ZERO AT_OTP_ZERO
#endif

#ifndef AT_OTP_COOKIE_TAIL
#define AT_OTP_COOKIE_TAIL  "\x1e\x1c\x1a\x18\x16\x14\x12\x10\x0e\x0c\x0a\x08\x06\x04\x02\x00"
#endif

#ifndef AT_OTP_COOKIE
#define AT_OTP_COOKIE       AT_OTP_COOKIE_HEAD AT_OTP_COOKIE_BODY AT_OTP_COOKIE_TAIL
#endif

enum {
    OTP_ID_CUSTOM,
    OTP_ID_PRIVATE,

    OTP_ID_END
};

static inline int
__otp_file_offset(FILE *f)
{
    int fd, err;
    char *buf = (char *)os_malloc(AT_OTP_FILE_SIZE);
    if (NULL==buf) {
        return -ENOMEM;
    }

    err = os_fread(f, buf, AT_OTP_FILE_SIZE);
    if (err<0) {
        goto error;
    }
    else if (AT_OTP_FILE_SIZE!=err) {
        err = -E2BIG; goto error;
    }

    char *begin = buf;
    while(begin < buf + AT_OTP_FILE_SIZE - (sizeof(AT_OTP_COOKIE) - 1)) {
        char *head = (char *)os_memmem(begin, AT_OTP_FILE_SIZE, AT_OTP_COOKIE_HEAD, sizeof(AT_OTP_COOKIE_HEAD)-1);
        if (NULL==head) {
            err = -ENOEXIST; goto error;
        }
        
        char *tail = head + sizeof(AT_OTP_COOKIE_HEAD AT_OTP_COOKIE_BODY) - 1;
        if (0==os_memcmp(tail, AT_OTP_COOKIE_TAIL)) {
            char *body = head + sizeof(AT_OTP_COOKIE_HEAD) - 1;
            err = body - buf;

            goto error;
        }
        
        begin = head + sizeof(AT_OTP_COOKIE_HEAD) - 1;
    }
    
error:
    os_free(buf);
    
    return err;
}

static inline int
__otp_file_op(int id, char *mode, unsigned char otp[AT_OTP_SIZE])
{
    static int otp_offset = 0;
    int offset, err = 0;

    FILE *f = os_fopen(AT_OTP_FILE, mode);
    if (NULL==f) {
        err = -EACCES; goto error;
    }

    if (0==otp_offset) {
        otp_offset = __otp_file_offset(f);
        if (otp_offset<0) {
            err = otp_offset; goto error;
        }
    }

    offset = otp_offset 
                + sizeof(AT_OTP_COOKIE_HEAD) - 1
                + id * (sizeof(AT_OTP_ZERO) - 1);

    err = fseek(f, offset, 0);
    if (err<0) {
        goto error;
    }

    if (0==os_strcmp(mode, "r")) {
        err = os_fread(f, otp, AT_OTP_SIZE);
    }
    else if (0==os_strcmp(mode, "r+")) {
        err = os_fwrite(f, otp, AT_OTP_SIZE);
    }
    else {
        err = -EINVAL9;
    }
    
    if (err<0) {
        goto error;
    }
    else if (AT_OTP_SIZE!=err) {
        err = -EFORMAT; goto error;
    }
    else {
        err = 0;
    }
    
error:
    os_fclose(f);

    return err;
}

static inline int
__otp_custom_read(unsigned char otp[AT_OTP_SIZE])
{
    return __otp_file_op(OTP_ID_CUSTOM, "r", otp);
}

static inline int
__otp_custom_write(unsigned char otp[AT_OTP_SIZE])
{
    return __otp_file_op(OTP_ID_CUSTOM, "r+", otp);
}

static inline int
__otp_private_read(unsigned char otp[AT_OTP_SIZE])
{
    return __otp_file_op(OTP_ID_PRIVATE, "r", otp);
}

static inline int
__otp_private_write(unsigned char otp[AT_OTP_SIZE])
{
    return __otp_file_op(OTP_ID_PRIVATE, "r+", otp);
}
#else
static inline int
__otp_init(void)
{
    int err;

    do{
        err = HI_UNF_OTP_Init();
            debug_trace_error(err, "otp init");
        if (err) {
            
            sleep(1);
        }
    } while(err);
    
    return 0;
}

static inline int
__otp_fini(void)
{
    int err;
    
    err = HI_UNF_OTP_DeInit();
        debug_trace_error(err, "otp fini");
    if (err) {
    }

    return err;
}

static inline int
__otp_custom_read(unsigned char otp[AT_OTP_SIZE])
{
    int err;

    err = HI_UNF_OTP_GetCustomerKey(otp, AT_OTP_SIZE);
        debug_trace_error(err, "read custom");
    if (err) {
        
        err = AT_OTP_ERR_READ;
    }
    
    return err;
}

static inline int
__otp_custom_write(unsigned char otp[AT_OTP_SIZE])
{
    int err;

    err = HI_UNF_OTP_SetCustomerKey(otp, AT_OTP_SIZE);
        debug_trace_error(err, "write custom");
    if (err) {
        err = AT_OTP_ERR_WRITE;
    } else {
        os_println("custom inited.");
    }

    return err;
}

static inline int
__otp_private_read(unsigned char otp[AT_OTP_SIZE])
{
    int i, err, errs = 0;

    for (i=0; i<AT_OTP_SIZE; i++) {
        err = HI_UNF_OTP_GetStbPrivData(i, &otp[i]);
        if (err) {
            errs = err;
        }
    }
    
    debug_trace_error(errs, "read private");
    if (errs) {
        
        errs = AT_OTP_ERR_READ;
    }

    return errs;
}

static inline int
__otp_private_write(unsigned char otp[AT_OTP_SIZE])
{
    int i, err, errs = 0;

    for (i=0; i<AT_OTP_SIZE; i++) {
        err = HI_UNF_OTP_SetStbPrivData(i, otp[i]);
        if (err) {
            errs = err;
        }
    }
    
    debug_trace_error(errs, "write private");
    if (errs) {
        errs = AT_OTP_ERR_WRITE;
    } else {
        os_println("private inited.");
    }

    return errs;
}
#endif

static inline int
__otp_check(
    unsigned char val[AT_OTP_SIZE], 
    int (*read)(unsigned char otp[]), 
    int (*write)(unsigned char otp[])
)
{
    unsigned char otp[AT_OTP_SIZE] = {0};
    int err;

    err = (*read)(otp);
    if (err) {
        return err;
    }

    if (__otp_eq(otp, AT_OTP_ZERO)) {
        dprintln("otp is zero");

        if (__otp_eq(val, AT_OTP_ZERO)) {
            dprintln("try set otp zero");

            return 0;
        }

        err = (*write)(val);
        if (err) {
            return err;
        }

        return 0;
    }
    else if (__otp_eq(otp, __AT_OTP_CUSTOM)) {
        dprintln("otp set==get");

        return 0;
    }
    else if (__otp_eq(otp, val)) {
        dprintln("otp set==get");

        return 0;
    }
    else {
        dprintln("otp is hacked");

        return AT_OTP_ERR_HACKED;
    }
}

static inline int
__otp_show(unsigned char otp[AT_OTP_SIZE], int (*read)(unsigned char otp[]), bool string)
{
    int err = (*read)(otp);
    if (0==err) {
        os_println("%s", string?(char *)otp:__otp_string(otp));
    }

    return err;
}

#define __otp_call(_call, _otp, _args...)   os_call(__otp_init, __otp_fini, _call, _otp, ##_args)

static inline int
at_otp_custom_show(void)
{
    unsigned char otp[AT_OTP_SIZE] = {0};
    
    return __otp_call(__otp_show, otp, __otp_custom_read, true);
}

static inline int
at_otp_custom_check(void)
{
    return __otp_call(__otp_check, AT_OTP_CUSTOM, __otp_custom_read, __otp_custom_write);
}

static inline int
at_otp_custom_write(unsigned char val[AT_OTP_SIZE])
{
    return __otp_call(__otp_custom_write, val);
}

static inline int
at_otp_private_show(void)
{
    unsigned char otp[AT_OTP_SIZE] = {0};
 
    return __otp_call(__otp_show, otp, __otp_private_read, false);
}

static inline int
at_otp_private_check(unsigned char val[AT_OTP_SIZE])
{
    return __otp_call(__otp_check, val, __otp_private_read, __otp_private_write);
}

static inline int
at_otp_private_write(unsigned char val[AT_OTP_SIZE])
{
    return __otp_call(__otp_private_write, val);
}
/******************************************************************************/
#endif /* __APP__ */
#endif /* __AT_OTP_H_739039F132EB8833CE84495D183364C7__ */
