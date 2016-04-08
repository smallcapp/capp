#ifndef __LIB_UTILS_PRINTF_H__
#define __LIB_UTILS_PRINTF_H__
/******************************************************************************/
#if defined(__BOOT__) || defined(__APP__)
#define os_printf(_fmt, _args...)               printf(_fmt, ##_args)
#define os_vprintf(_fmt, _args)                 vprintf(_fmt, _args)
#elif defined(__KERNEL__)
#define os_printf(_fmt, _args...)               printk(KERN_ALERT _fmt, ##_args)
#define os_vprintf(_fmt, _args)                 vprintk(KERN_ALERT _fmt, _args)
#else
#error  "need printf and vprintf"
#endif

#ifdef __APP__
#define os_fprintf(_stream, _fmt, _args...)     fprintf(_stream, _fmt, ##_args)
#define os_vfprintf(_stream, _fmt, _args)       vfprintf(_stream, _fmt, _args)

#define os_eprintf(_fmt, _args...)              os_fprintf(stderr, _fmt, ##_args)
#define os_evprintf(_fmt, _args)                os_vfprintf(stderr, _fmt, _args)
#else
#define os_fprintf(_stream, _fmt, _args...)     os_printf(_fmt, ##_args)
#define os_vfprintf(_stream, _fmt, _args)       os_vprintf(_fmt, _args)

#define os_eprintf(_fmt, _args...)              os_printf(_fmt, ##_args)
#define os_evprintf(_fmt, _args)                os_vprintf(_fmt, _args)
#endif

#define os_sscanf(_buf, _fmt, _args...)         sscanf(_buf, _fmt, ##_args)
#define os_vsscanf(_buf, _fmt, _args)           vsscanf(_buf, _fmt, _args)

#define os_println(_fmt, _args...)              os_printf(_fmt __crlf, ##_args)
#define os_vprintln(_fmt, _args)                os_vprintf(_fmt __crlf, _args)

#define os_printab(_count, _fmt, _args...) do{ \
    int i;                      \
                                \
    for (i=0; i<_count; i++) {  \
        os_printf(__tab);       \
    }                           \
    os_println(_fmt, ##_args);  \
}while(0)

#define os_vprintab(count, _fmt, _args) do{ \
    int i;                      \
                                \
    for (i=0; i<count; i++) {   \
        os_printf(__tab);       \
    }                           \
    os_vprintln(_fmt, _args);   \
}while(0)

#define os_fprintln(_stream, _fmt, _args...)    os_fprintf(_stream, _fmt __crlf, ##_args)
#define os_vfprintln(_stream, _fmt, _args)      os_vfprintf(_stream, _fmt __crlf, _args)

#define os_eprintln(_fmt, _args...)             os_eprintf(_fmt __crlf, ##_args)
#define os_evprintln(_fmt, _args)               os_evprintf(_fmt __crlf, _args)

#define os_sprintf(_buf, _fmt, _args...)        sprintf(_buf, _fmt, ##_args)
#define os_vsprintf(_buf, _fmt, _args)          vsprintf(_buf, _fmt, _args)

#define os_sprintln(_buf, _fmt, _args...)       os_sprintf(_buf, _fmt __crlf, ##_args)
#define os_vsprintln(_buf, _fmt, _args)         os_vsprintf(_buf, _fmt __crlf, _args)

#define os_snprintf_is_full(_buffer_space, _snprintf_return_value) \
        ((_snprintf_return_value) >= (_buffer_space))

#ifdef __BOOT__
#ifndef BOOT_SNPRINTF_BUFSIZE
#define BOOT_SNPRINTF_BUFSIZE   4096
#endif

static inline int
os_vsnprintf(char *buf, int size, const char *fmt, va_list args)
{
    int bsize = size?size-1:0;
    int len = 0;
    
    char *p = (char *)os_zalloc(BOOT_SNPRINTF_BUFSIZE);
    if (NULL==p) {
        return -ENOMEM;
    }

    len = os_vsprintf(p, fmt, args);

    if (len <= bsize) {
        os_strcpy(buf, p);
    } else {
        len = bsize;
        os_memcpy(buf, p, len);
    }

error:
    os_free(p);

    return len;
}

static inline int
os_snprintf(char *buf, int size, const char *fmt, ...)
{
    va_list args;

    va_start(args, (char *)fmt);
    int len = os_vsnprintf(buf, size, fmt, args);
    va_end(args);

    return len;
}

static inline int
os_vasprintf(char **buf, const char *fmt, va_list args)
{
    int len = 0;
    
    char *p = (char *)os_zalloc(BOOT_SNPRINTF_BUFSIZE);
    if (NULL==p) {
        return -ENOMEM;
    }

    len = os_vsprintf(p, fmt, args);
    *buf = p;

    return len;
}

static inline int
os_asprintf(char **buf, const char *fmt, ...)
{
    va_list args;

    va_start(args, (char *)fmt);
    int len = os_vasprintf(buf, fmt, args);
    va_end(args);

    return len;
}

static inline int
os_rsprintf(char **buf, int resv, const char *fmt, ...)
{
    va_list args;

    va_start(args, (char *)fmt);
    int len = os_vrsprintf(buf, resv, fmt, args);
    va_end(args);

    return len;
}
#else
#define os_snprintf(_buf, _size, _fmt, _args...) snprintf(_buf, _size, _fmt, ##_args)
#define os_vsnprintf(_buf, _size, _fmt, _args)  vsnprintf(_buf, _size, _fmt, _args)

#define os_asprintf(_pbuf, _fmt, _args...)      asprintf(_pbuf, _fmt, ##_args)
#define os_vasprintf(_pbuf, _fmt, _args)        vasprintf(_pbuf, _fmt, _args)

static inline int
os_vasprintf_resv(char **buf, int resv, const char *fmt, va_list args)
{
    int len = 0;
    va_list args2;
    
    va_copy(args2, args);
    len = os_vsnprintf(NULL, 0, fmt, args2);

    char *p = (char *)os_zalloc(1+len+resv);
    if (NULL==p) {
        return -ENOMEM;
    }

    len = os_vsnprintf(p+resv, len, fmt, args);
    p[resv+len] = 0;
    
    *buf = p;

    return len;
}

static inline int
os_asprintf_resv(char **buf, int resv, const char *fmt, ...)
{
    va_list args;

    va_start(args, (char *)fmt);
    int len = os_vasprintf_resv(buf, resv, fmt, args);
    va_end(args);

    return len;
}

#endif

#define os_saprintf(_buf, _fmt, _args...)       os_snprintf(_buf, sizeof(_buf), _fmt, ##_args)
#define os_vsaprintf(_buf, _fmt, _args)         os_vsnprintf(_buf, sizeof(_buf), _fmt, _args)
#define os_soprintf(_buf, _offset, _fmt, _args...)  \
    os_snprintf(_buf+(_offset), sizeof(_buf)-(_offset), _fmt, ##_args)
#define os_voaprintf(_buf, _offset, _fmt, _args)    \
    os_vsnprintf(_buf+(_offset), sizeof(_buf)-(_offset), _fmt, _args)

#ifdef __APP__
#define __os_v_xopen(_type, _func, _mod, _fmt, _args...) ({  \
    char cmd[1+OS_FILE_LEN] = {0};  \
    _type tvar;                     \
                                    \
    os_saprintf(cmd, _fmt, ##_args);\
    if (__debug_level_test && __debug_level_shell) { \
        debug_trace("__os_v_xopen %s", cmd); \
    }                               \
    tvar = _func(cmd, _mod);        \
                                    \
    tvar;                           \
})

#define os_v_popen(_mod, _fmt, _args...)      \
    __os_v_xopen(FILE*, popen, _mod, _fmt, ##_args)
#define os_v_fopen(_mod, _fmt, _args...) \
    __os_v_xopen(FILE*, fopen, _mod, _fmt, ##_args)
#define os_v_open(_flag, _fmt, _args...) \
    __os_v_xopen(int, open, _flag, _fmt, ##_args)

static inline int
__os_system(char *cmd);

static inline int
os_vsystem(const char *fmt, va_list args)
{
    int len, err = 0;
    char *cmd = NULL;

    err = os_vasprintf(&cmd, fmt, args);
    if (err<0) {
        goto error;
    } else {
        err = __os_system(cmd);
    }
    
error:
    os_free(cmd);

    return err;
}

static inline int
os_system(const char *fmt, ...)
{
    int err;
    va_list args;
    
    va_start(args, (char *)fmt);
    err = os_vsystem(fmt, args);
    va_end(args);
    
    return err;
}
#endif /* __APP__ */
/******************************************************************************/
#endif /* __LIB_UTILS_PRINTF_H__ */
