#ifndef __BASE_H_DF48B466F7D87EDB327C3D4C73E6E4A3__
#define __BASE_H_DF48B466F7D87EDB327C3D4C73E6E4A3__
/******************************************************************************/
#if defined(__BOOT__) || defined(__APP__)
#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(_condition)    ((void)BUILD_BUG_ON_ZERO(_condition))
#endif

#ifndef BUILD_BUG_ON_ZERO
#define BUILD_BUG_ON_ZERO(_e)       (sizeof(struct { int:-!!(_e); }))
#endif

#ifndef BUILD_BUG_ON_NULL
#define BUILD_BUG_ON_NULL(_e)       ((void *)sizeof(struct { int:-!!(_e); }))
#endif

#ifndef offsetof
#define offsetof(_TYPE, _MEMBER)    __builtin_offsetof(_TYPE, _MEMBER)
#endif

#ifndef container_of
#define container_of(_ptr, _type, _member) \
	    ((_type *)((char *)(_ptr) - offsetof(_type, _member)))
#endif
#endif /* defined(__BOOT__) || defined(__APP__) */

#ifndef os_count_of
#define os_count_of(_x)         (sizeof(_x)/sizeof((_x)[0]))
#endif

#ifndef os_pointer_offset
#define os_pointer_offset(_base, _pointer)  ((char *)(_pointer) - (char *)(_base))
#endif

#ifndef os_constructor
#   ifdef __BUSYBOX__
#       define os_constructor   inline
#   else
#       define os_constructor   __attribute__((constructor))
#   endif
#endif

#ifndef os_destructor
#   ifdef __BUSYBOX__
#       define os_destructor    inline
#   else
#       define os_destructor    __attribute__((destructor))
#   endif
#endif

#ifndef os_do_nothing 
#define os_do_nothing()     do{}while(0)
#endif

#ifndef os_use_fake_value
#define os_use_fake_value   extern int __os_value_not_used_forever
#endif

#ifndef os_wait_forever
#define os_wait_forever()   do{sleep(60);}while(1)
#endif

#define os_swap_value(_a, _b)  do {    \
    typeof(_a) _tmp = (_a);     \
    (_a) = (_b);                \
    (_b) = _tmp;                \
}while(0)

#define INVALID_VALUE       (-1)
#define INVALID_COMMON_ID   INVALID_VALUE
#define INVALID_SEM_ID      INVALID_COMMON_ID
#define INVALID_FD          INVALID_COMMON_ID

#define OS_PROTECTED        0x2012dead

static inline bool is_good_common_id(int id)
{
    return id >= 0;
}

#define is_good_shmid(_shmid)   is_good_common_id(_shmid)
#define is_good_semid(_semid)   is_good_common_id(_semid)
#define is_good_fd(_fd)         is_good_common_id(_fd)

#define __is_good_fd(_fd)       ((_fd) > 0)

#define __os_close(_fd) ({  \
    int ___fd = (_fd);      \
    int __os_close_err = 0; \
                            \
    if (is_good_fd(___fd)) {\
        __os_close_err = close(___fd); \
    }                       \
                            \
    __os_close_err;         \
})  /* end */

#define os_close(_fd)   ({  \
    int os_close_err = __os_close(_fd); \
    _fd = INVALID_FD;       \
    os_close_err;           \
})  /* end */

#define is_good_value(_value, _begin, _end) ({  \
    typeof(_value) __v = (_value);              \
    (__v >= (_begin) && __v < (_end));          \
})  /* end */

#define is_good_enum(_id, _end)     is_good_value(_id, 0, _end)

#define os_safe_value(_value, _min, _max) ({\
    typeof(_value)  __value   = _value;     \
    typeof(_value)  __min     = _min;       \
    typeof(_value)  __max     = _max;       \
                                            \
    if (__value < __min) {          \
        __value = __min;            \
    } else if (__value > __max) {   \
        __value = __max;            \
    }                               \
                                    \
    __value;                        \
})  /* end */

#if 1
#define os_min(_x,_y)   ({  \
    typeof(_x) x = (_x);    \
    typeof(_y) y = (_y);    \
    (void) (&x == &y);      \
    x < y ? x : y;          \
})  /* end */
    
#define os_max(_x,_y)   ({  \
    typeof(_x) x = (_x);    \
    typeof(_y) y = (_y);    \
    (void) (&x == &y);      \
    x > y ? x : y;          \
})  /* end */
#endif

static inline int
os_cmp_char(int ret)
{
    int ch;
    
    if (0==ret) {
        ch = '=';
    }
    else if (ret<0) {
        ch = '<';
    }
    else {
        ch = '>';
    }
    
    return ch;
}

#define OS_IOVEC_INITER(_base, _len) { \
    .iov_base   = _base,    \
    .iov_len    = _len,     \
}   /* end */

#define __OS_MSGHDR_INITER( \
    _iov,                   \
    _iovlen,                \
    _name,                  \
    _namelen,               \
    _control,               \
    _controllen             \
)                           \
{                           \
    .msg_iov        = _iov,         \
    .msg_iovlen     = _iovlen,      \
    .msg_name       = _name,        \
    .msg_namelen    = _namelen,     \
    .msg_control    = _control,     \
    .msg_controllen = _controllen,  \
    .msg_flags      = 0,            \
}   /* end */

#define OS_MSGHDR_INITER(_iov, _iovlen, _name, _namelen)    \
    __OS_MSGHDR_INITER(_iov, _iovlen, _name, _namelen, NULL, 0)

#define OS_SOCKADDR_UNSPEC()    {   \
    .sa_family = AF_UNSPEC,         \
}   /* end */

#define OS_SOCKADDR_UNIX(_path) {   \
    .sun_family = AF_UNIX,          \
    .sun_path   = _path,            \
}   /* end */

#define OS_SOCKADDR_INET(_ip, _port) { \
    .sin_family = AF_INET,          \
    .sin_port   = _port,            \
    .sin_addr   = {                 \
        .s_addr = _ip,              \
    }                               \
}   /* end */

#define OS_SOCKADDR_NETLINK(_pid, _groups) { \
    .nl_family  = AF_NETLINK,       \
    .nl_pid     = _pid,             \
    .nl_groups  = _groups           \                               \
}   /* end */

#define OS_TIMEVAL_INITER(_sec, _usec) { \
    .tv_sec = _sec,     \
    .tv_usec= _usec,    \
}   /* end */

#define OS_ITIMEVAL_INITER(_sec, _usec) { \
    .it_interval= OS_TIMEVAL_INITER(_sec, _usec),   \
    .it_value   = OS_TIMEVAL_INITER(_sec, _usec),   \
}   /* end */

#define OS_TIMESPEC_INITER(_sec, _nsec) { \
    .tv_sec = _sec,     \
    .tv_nsec= _nsec,    \
}   /* end */

#define OS_ITIMESPEC_INITER(_sec, _nsec) { \
    .it_interval= OS_TIMESPEC_INITER(_sec, _nsec),  \
    .it_value   = OS_TIMESPEC_INITER(_sec, _nsec),  \
}   /* end */

static inline bool os_seq_before(unsigned int seq1, unsigned int seq2)
{
    return ((int)(seq1-seq2)) < 0;
}

#define os_seq_after(_seq1, _seq2)  os_seq_before(_seq2, _seq1)

enum {
    __APP_NAMELEN       = (31 - sizeof(unsigned int)),
    __APP_KEY_NAMELEN   = 31,
    
    OS_IFNAMELEN        = 15,
};

#ifndef OS_SHORTLINE_LEN
#define OS_SHORTLINE_LEN        (127)
#endif

#ifndef OS_LINE_LEN
#define OS_LINE_LEN             (1*1024-1)
#endif

#ifndef OS_PAGE_LEN
#define OS_PAGE_LEN             (4*1024-1)
#endif

#ifndef OS_BLOCK_LEN
#define OS_BLOCK_LEN            (16*1024-1)
#endif

#ifndef OS_FILE_LEN
#define OS_FILE_LEN             (64*1024-1)
#endif

#ifndef OS_HUGE_LEN
#define OS_HUGE_LEN             (256*1024-1)
#endif

#ifndef OS_FILENAME_LEN
#define OS_FILENAME_LEN         (1*1024-1)
#endif

#define __errno(_err)               ({  \
    int __errno_err = (_err);           \
                                        \
    __errno_err<0?-errno:__errno_err;   \
})

#define __os_call(_new, _free, _call, _args...) ({ \
    int __os_call_err = 0;                  \
    void *__os_call_obj = (void *)_new();   \
    if (__os_call_obj) {                    \
        __os_call_err = _call(__os_call_obj, ##_args); \
    }                                       \
    _free(__os_call_obj);                   \
                                            \
    __os_call_err;                          \
})  /* end */

#define os_call(_begin, _end, _call, _arg1, _args...) ({ \
    int os_call_err = _begin();             \
    if (0==os_call_err) {                   \
        os_call_err = _call(_arg1, ##_args);\
    }                                       \
    _end();                                 \
                                            \
    shell_error(os_call_err);               \
})  /* end */

#define os_call_1(_begin, _end, _call, _arg1, _args...) ({ \
    int os_call_1_err = _begin(_arg1);          \
    if (0==os_call_1_err) {                     \
        os_call_1_err = _call(_arg1, ##_args);  \
    }                                           \
    _end(_arg1);                                \
                                                \
    shell_error(os_call_1_err);                 \
})  /* end */

#define os_call_2(_begin, _end, _call, _arg1, _arg2, _args...) ({ \
    int os_call_2_err = _begin(_arg1, _arg2);   \
    if (0==os_call_2_err) {                     \
        os_call_2_err = _call(_arg1, _arg2, ##_args); \
    }                                           \
    _end(_arg1, _arg2);                         \
                                                \
    shell_error(os_call_2_err);                 \
})  /* end */

#define os_call_3(_begin, _end, _call, _arg1, _arg2, _arg3, _args...) ({ \
    int os_call_3_err = _begin(_arg1, _arg2, _arg3);    \
    if (0==os_call_3_err) {                             \
        os_call_3_err = _call(_arg1, _arg2, _arg3, ##_args); \
    }                                                   \
    _end(_arg1, _arg2, _arg3);                          \
                                                        \
    shell_error(os_call_3_err);                         \
})  /* end */

#define os_call_4(_begin, _end, _call, _arg1, _arg2, _arg3, _arg4, _args...) ({ \
    int os_call_4_err = _begin(_arg1, _arg2, _arg3, _arg4); \
    if (0==os_call_4_err) {                                 \
        os_call_4_err = _call(_arg1, _arg2, _arg3, _arg4, ##_args); \
    }                                                       \
    _end(_arg1, _arg2, _arg3, _arg4);                       \
                                                            \
    shell_error(os_call_4_err);                             \
})  /* end */

#define os_call_5(_begin, _end, _call, _arg1, _arg2, _arg3, _arg4, _arg5, _args...) ({ \
    int os_call_5_err = _begin(_arg1, _arg2, _arg3, _arg4, _arg5);  \
    if (0==os_call_5_err) {                                         \
        os_call_5_err = _call(_arg1, _arg2, _arg3, _arg4, _arg5, ##_args); \
    }                                                               \
    _end(_arg1, _arg2, _arg3, _arg4, _arg5);                        \
                                                                    \
    shell_error(os_call_5_err);                                     \
})  /* end */

static inline void 
os_closexec(int fd)
{
#ifdef __APP__
    fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
#endif
}

static inline void 
os_noblock(int fd)
{
#ifdef __APP__
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK );
#endif
}

static inline int
__os_wait_error(int status)
{
    int err = 0;
    
#ifdef __APP__
    if (WIFEXITED(status)) {
        err = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        if (WCOREDUMP(status)) {
            err = WCOREDUMP(status);
        }
        else if (WTERMSIG(status)) {
            err = WTERMSIG(status);
        }
    }
#endif

    return err;
}

typedef struct {
    char *k;
    unsigned int v;
} os_kv_t;

#define OS_KV_INITER(_k, _v)    { .k = _k, .v = _v }
/******************************************************************************/
#endif /* __BASE_H_DF48B466F7D87EDB327C3D4C73E6E4A3__ */
