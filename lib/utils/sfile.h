#ifndef __SFILE_H_CAF3737A24BFD00978F62CEB481C30B0__
#define __SFILE_H_CAF3737A24BFD00978F62CEB481C30B0__
#ifdef __APP__
/******************************************************************************/
static inline FILE *
os_fopen(const char *file, const char *mode)
{
    FILE *f = fopen(file, mode);
    if (NULL==f) {
        debug_error("open %s error:%d", file, -errno);
    }

    return f;
}

static inline int
__os_fclose(FILE *stream)
{
    int err = 0;

    if(stream) {
        err = fclose(stream);
    }

    return err;
}

#define os_fclose(_stream)      ({  \
    int os_fclose_err = __os_fclose(_stream); \
    _stream = NULL;                 \
    os_fclose_err;                  \
})  /* end */

#define os_fclean(_file)            __os_fclose(fopen(_file, "w"))

static inline bool
os_feof(FILE *stream)
{
    return stream?!!feof(stream):true;
}

static inline int
os_fwrite(FILE *stream, const void *buf, int size)
{
    int err = fwrite(buf, 1, size, stream);
    
    fflush(stream);
    
    return (err<0)?-errno:err;
}

static inline int
os_fread(FILE *stream, void *buf, int size)
{
    int err = fread(buf, 1, size, stream);

    return (err<0)?-errno:err;
}

static inline char *
os_freadline(FILE *stream, char *line, int size)
{
    char *p = NULL;

    if (stream && line && size>0) {
        p = fgets(line, size, stream);
    }

    return p;
}

static inline int
os_fwriteline(FILE *stream, const char *line)
{
    int len = 0;

    if (stream && line) {
        len = fputs(line, stream);
        fputc('\n', stream);
    }

    return len;
}

static inline bool 
os_sfscan_match_all(char *context)
{
    return '*'==context[0] && 0==context[1];
}

static inline bool
os_sfcan_is_dir_self(char *filename)
{
    return '.'==filename[0] && 0==filename[1];
}

static inline bool
os_sfcan_is_dir_father(char *filename)
{
    return '.'==filename[0] && '.'==filename[1] && 0==filename[2];
}

typedef multi_value_t os_sfscan_line_handle_t(char *filename, char *line, void *control);
typedef multi_value_t os_sfscan_file_handle_t(char *path, char *filename, 
                            os_sfscan_line_handle_t *line_handle, void *control);
typedef bool os_sfscan_file_filter_t(char *path, char *filename, void *control);

static inline multi_value_t 
os_sfscan_file_handle
(
    char *path,
    char *filename,
    os_sfscan_line_handle_t *line_handle,
    void *control
)
{
    FILE *stream = NULL;
    char line[1+OS_LINE_LEN];
    multi_value_u mv;

    if (NULL==path) {
        return os_assertV(mv2_GO(-EINVAL1));
    }
    else if (NULL==filename) {
        return os_assertV(mv2_GO(-EINVAL2));
    }
    else if (NULL==line_handle) {
        return os_assertV(mv2_GO(-EINVAL3));
    }
    
    stream = os_v_fopen("r", "%s/%s", path, filename);
    if (NULL==stream) {
        debug_error("open %s/%s error:%d", path, filename, -errno);
        
        mv2_result(mv) = -errno;
        
        goto error;
    }
    
    while(!os_feof(stream)) {
        line[0] = 0;
        os_freadline(stream, line, OS_LINE_LEN);
        
        __string_strim_both(line, NULL);
        __string_replace(line, NULL, ' ');
        __string_reduce(line, NULL);
        
        if (__is_blank_line(line)) {
            continue;
        }
        
        if (__is_notes_line_deft(line)) {
            continue;
        }
        
        mv.value = (*line_handle)(filename, line, control);
        if (mv2_is_break(mv)) {
            goto error;
        }
    }

    mv.value = mv2_OK;
error:
    os_fclose(stream);
    
    return mv.value;
}

static inline int 
__os_sfscan_dir
(
    char *path, 
    bool recur,
    os_sfscan_file_filter_t *file_filter,
    os_sfscan_file_handle_t *file_handle,
    os_sfscan_line_handle_t *line_handle,
    void *control
)
{
    DIR *dir = NULL;
    struct dirent *file = NULL;
    struct stat st;
    multi_value_u mv;
    int err = 0;

    if (NULL==path) {
        return os_assertV(-EINVAL1);
    }
    else if (NULL==file_handle) {
        return os_assertV(-EINVAL2);
    }

    debug_trace("begin scan %s", path);
    
    dir = opendir(path);
    if (NULL == dir) {
        err = -errno;

        debug_error("open dir %s error:%d", path, -errno);
        
        goto error;
    }
    
    while (NULL != (file=readdir(dir))) {
        char *filename = file->d_name;

        debug_trace("scan %s/%s", path, filename);

        if (os_sfcan_is_dir_self(filename) || os_sfcan_is_dir_father(filename)) {
            debug_trace("skip %s/%s", path, filename);
            
            continue;
        }
        
        if (stat(filename, &st) >= 0 && S_ISDIR(st.st_mode)) {
            if (recur) {
                char line[1+OS_LINE_LEN];

                os_sprintf(line, "%s/%s", path, filename);
                
                err = __os_sfscan_dir(line, recur, file_filter, file_handle, line_handle, control);
                if (err) {
                    goto error;
                }
            } else {
                continue;
            }
        }
        
        if (file_filter && (*file_filter)(path, filename, control)) {
            debug_trace("skip %s/%s", path, filename);
            
            continue;
        }
        
        mv.value = (*file_handle)(path, filename, line_handle, control);
        if (mv2_is_break(mv)) {
            err = mv2_result(mv);

            debug_error("handle %s/%s error:%d", path, filename, err);
            
            goto error;
        }
    }

    debug_trace("end scan %s error:%d", path, err);
error:
    if (dir) {
        closedir(dir);
    }

    return err;
}

static inline int 
os_sfscan_dir
(
    char *path, 
    bool recur,
    os_sfscan_file_filter_t *file_filter,
    os_sfscan_line_handle_t *line_handle,
    void *control
)
{
    return __os_sfscan_dir(path, recur, file_filter, os_sfscan_file_handle, line_handle, control);
}
/******************************************************************************/
#define __os_sgetx(_prefix, _stream, _vfmt, _pv) ({ \
    int __os_sgetx_err = 0;         \
                                    \
    if (NULL==(_stream)) {          \
        __os_sgetx_err = -errno;    \
    } else if (1!=fscanf(_stream, _vfmt, _pv)) {  \
        __os_sgetx_err = -EFORMAT;  \
    }                               \
                                    \
    if (_stream) {                  \
        __os_sgetx_err = __os_wait_error(_prefix##close(_stream)); \
    }                               \
                                    \
    __os_sgetx_err;                 \
})

#define __os_sgets(_prefix, _stream, _line, _space) ({ \
    int __os_sgets_err = 0;         \
                                    \
    if (NULL==(_line)) {            \
        __os_sgets_err = -EINVAL9;  \
    } else if ((_space)<=0) {       \
        __os_sgets_err = -EINVAL8;  \
    } else if (NULL==_stream) {     \
        __os_sgets_err = -EINVAL7;  \
    } else if (NULL==os_freadline(_stream, _line, _space)) { \
        __os_sgets_err = -errno;    \
    } else {                        \
        __string_strim_both(_line, NULL); \
    }                               \
                                    \
    if (_stream) {                  \
        __os_sgets_err = __os_wait_error(_prefix##close(_stream)); \
    }                               \
                                    \
    __os_sgets_err;                 \
}) /* end */

#define __os_sgetf(_prefix, _stream, _buf, _size) ({ \
    int __os_sgetf_err = 0;         \
                                    \
    if (NULL==(_buf)) {             \
        __os_sgetf_err = -EINVAL9;  \
    } else if ((_size)<=0) {        \
        __os_sgetf_err = -EINVAL8;  \
    } else if (NULL==_stream) {     \
        __os_sgetf_err = -EINVAL7;  \
    } else if (os_fread(_stream, _buf, _size)<0) { \
        __os_sgetf_err = -errno;    \
    }                               \
                                    \
    if (_stream) {                  \
        __os_sgetf_err = __os_wait_error(_prefix##close(_stream)); \
    }                               \
                                    \
    __os_sgetf_err;                 \
}) /* end */

#define os_sgetx(_prefix, _vfmt, _pv, _filename)        ({  \
    FILE *os_sgetx_stream = _prefix##open(_filename, "r");  \
    __os_sgetx(_prefix, os_sgetx_stream, _vfmt, _pv);       \
})  /* end */

#define os_sgets(_prefix, _line, _space, _filename)     ({  \
    FILE *os_sgets_stream = _prefix##open(_filename, "r");  \
    __os_sgets(_prefix, os_sgets_stream, _line, _space);    \
})  /* end */

#define os_sgetf(_prefix, _buf, _size, _filename)       ({  \
    FILE *os_sgetf_stream = _prefix##open(_filename, "r");  \
    __os_sgetf(_prefix, os_sgetf_stream, _buf, _size);      \
})  /* end */

#define os_sgeti(_prefix, _pi, _filename)   os_sgetx(_prefix, "%u", _pi, _filename)
#define os_sgetll(_prefix, _pll, _filename) os_sgetx(_prefix, "%llu", _pll, _filename)

#define os_v_sgetx(_prefix, _vfmt, _pv, _fmt, _args...)     ({  \
    FILE *os_v_sgetx_stream = os_v_##_prefix##open("r", _fmt, ##_args);  \
    __os_sgetx(_prefix, os_v_sgetx_stream, _vfmt, _pv);         \
})  /* end */

#define os_v_sgets(_prefix, _line, _space, _fmt, _args...)  ({  \
    FILE *os_v_sgets_stream = os_v_##_prefix##open("r", _fmt, ##_args);  \
    __os_sgets(_prefix, os_v_sgets_stream, _line, _space);      \
})  /* end */

#define os_v_sgetf(_prefix, _buf, _size, _fmt, _args...)    ({  \
    FILE *os_v_sgetf_stream = os_v_##_prefix##open("r", _fmt, ##_args);  \
    __os_sgetf(_prefix, os_v_sgetf_stream, _buf, _size);        \
})  /* end */

#define os_v_sgeti(_prefix, _pi, _fmt, _args...)    os_v_sgetx(_prefix, "%u", _pi, _fmt, ##_args)
#define os_v_sgetll(_prefix, _pll, _fmt, _args...)  os_v_sgetx(_prefix, "%u", _pll, _fmt, ##_args)

#define os_sfgets(_line, _space, _filename)         os_sgets(f, _line, _space, _filename)
#define os_sfgetf(_buf, _size, _filename)           os_sgetf(f, _buf, _size, _filename)
#define os_sfgeti(_pi, _filename)                   os_sgeti(f, _pi, _filename)
#define os_sfgetll(_pll, _filename)                 os_sgetll(f, _pll, _filename)

#define os_v_sfgets(_line, _space, _fmt, _args...)  os_v_sgets(f, _line, _space, _fmt, ##_args)
#define os_v_sfgetf(_buf, _size, _fmt, _args...)    os_v_sgetf(f, _buf, _size, _fmt, ##_args)
#define os_v_sfgeti(_pi, _fmt, _args...)            os_v_sgeti(f, _pi, _fmt, ##_args)
#define os_v_sfgetll(_pll, _fmt, _args...)          os_v_sgeti(f, _pll, _fmt, ##_args)

#define os_spgets(_line, _space, _filename)         os_sgets(p, _line, _space, _filename)
#define os_spgetf(_buf, _size, _filename)           os_sgetf(p, _buf, _size, _filename)
#define os_spgeti(_pi, _filename)                   os_sgeti(p, _pi, _filename)
#define os_spgetll(_pll, _filename)                 os_sgetll(p, _pll, _filename)

#define os_v_spgets(_line, _space, _fmt, _args...)  os_v_sgets(p, _line, _space, _fmt, ##_args)
#define os_v_spgetf(_buf, _size, _fmt, _args...)    os_v_sgetf(p, _buf, _size, _fmt, ##_args)
#define os_v_spgeti(_pi, _fmt, _args...)            os_v_sgeti(p, _pi, _fmt, ##_args)
#define os_v_spgetll(_pll, _fmt, _args...)          os_v_sgeti(p, _pll, _fmt, ##_args)

#define __os_ssetx(_prefix, _stream, _vfmt, v) ({ \
    int __os_ssetx_err = 0;     \
                                \
    if (NULL==(_stream)) {      \
        __os_ssetx_err = -errno;\
    } else {                    \
        __os_ssetx_err = fprintf(_stream, _vfmt, v); \
    }                           \
                                \
    if (_stream) {              \
        __os_ssetx_err = __os_wait_error(_prefix##close(_stream)); \
    }                           \
                                \
    __os_ssetx_err;             \
})  /* end */

#define os_ssetx(_prefix, _vfmt, _v, _filename)         ({  \
    FILE *os_ssetx_stream = _prefix##open(_filename, "w");  \
    __os_ssetx(_prefix, os_ssetx_stream, _vfmt, _v);        \
})  /* end */

#define os_v_ssetx(_prefix, _vfmt, _v, _fmt, _args...)  ({  \
    FILE *os_v_ssetx_stream = os_v_##_prefix##open("w", _fmt, ##_args);  \
    __os_ssetx(_prefix, os_v_ssetx_stream, _vfmt, _v);      \
})  /* end */

#define os_ssets(_prefix, _string, _filename)          os_ssetx(_prefix, "%s", _string, _filename)
#define os_sseti(_prefix, _vi, _filename)              os_ssetx(_prefix, "%u", _vi, _filename)
#define os_ssetll(_prefix, _vll, _filename)            os_ssetx(_prefix, "%llu", _vll, _filename)

#define os_v_ssets(_prefix, _string, _fmt, _args...)    os_v_ssetx(_prefix, "%s", _string, _fmt, ##_args)
#define os_v_sseti(_prefix, _vi, _fmt, _args...)        os_v_ssetx(_prefix, "%u", _vi, _fmt, ##_args)
#define os_v_ssetll(_prefix, _vll, _fmt, _args...)      os_v_ssetx(_prefix, "%llu", _vll, _fmt, ##_args)

#define os_sfsets(_string, _filename)                 os_ssets(f, _string, _filename)
#define os_sfseti(_vi, _filename)                     os_sseti(f, _vi, _filename)
#define os_sfsetll(_vll, _filename)                   os_ssetll(f, _vll, _filename)

#define os_v_sfsets(_string, _fmt, _args...)           os_v_ssets(f, _string, _fmt, ##_args)
#define os_v_sfseti(_vi, _fmt, _args...)               os_v_sseti(f, _vi, _fmt, ##_args)
#define os_v_sfsetll(_vll, _fmt, _args...)             os_v_ssetll(f, _vll, _fmt, ##_args)

static inline int
os_sfsize(char *filename)
{
    struct stat st;
    int err;
    
    err = stat(filename, &st);
    if (err) {
        return -errno;
    } else {
        return st.st_size;
    }
}

#define os_v_sfsize(_fmt, _args...)             ({  \
    int os_v_sfsize_size;                           \
    char os_v_sfsize_buf[1+OS_LINE_LEN] = {0};      \
                                                    \
    os_saprintf(os_v_sfsize_buf, _fmt, ##_args);    \
    os_v_sfsize_size = os_sfsize(os_v_sfsize_buf);  \
                                                    \
    os_v_sfsize_size;                               \
})

#define DECLARE_FAKE_FLOCK  extern int __THIS_LOCKFD
#define DECLARE_REAL_FLOCK  int __THIS_LOCKFD = INVALID_FD

#ifdef __BUSYBOX__
#define DECLARE_FLOCK       DECLARE_FAKE_FLOCK
#else
#define DECLARE_FLOCK       DECLARE_REAL_FLOCK
#endif

DECLARE_FAKE_FLOCK;

static inline int
__os_file_lock(char *file, int open_mode, int permit, bool block)
{
    int err, fd;
    int deft_permit = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int deft_mode   = O_RDONLY | O_CREAT;
    
    fd = open(file, open_mode?open_mode:deft_mode, permit?permit:deft_permit);
        debug_trace_error((fd<0?-errno:0), "open %s", file);
    if (fd<0) {
        return -errno;
    }
    
    err = flock(fd, LOCK_EX | (block?0:LOCK_NB));
        debug_trace_error((err?-errno:0), "lock %s", file);
    if (err) {
        return -errno;
    }
    
    return fd;
}

static int
os_file_lock(bool block)
{
    __THIS_LOCKFD = __os_file_lock(__THIS_LOCKFILE, 0, (S_IRUSR | S_IRGRP), block);

    return is_good_fd(__THIS_LOCKFD)?0:__THIS_LOCKFD;
}

static int
__os_file_unlock(char *file, int fd)
{
    int err;

    if (false==is_good_fd(fd)) {
        return -ENOLOCK;
    }
    
    err = flock(fd, LOCK_UN);
        debug_trace_error((err?-errno:0), "unlock %s", file);
    if (err) {
        /* NOT return */
    }

    err = close(fd);
        debug_trace_error((err?-errno:0), "close %s", file);
    if (err) {
        return err;
    }
    
    return 0;
}

#define os_file_unlock()    ({  \
    int os_file_unlock_err = __os_file_unlock(__THIS_LOCKFILE, __THIS_LOCKFD); \
    __THIS_LOCKFD = INVALID_FD; \
    os_file_unlock_err;         \
})  /* end */

static inline bool
os_file_exist(char *file)
{
    int fd = open(file, O_RDONLY, S_IRUSR | S_IRGRP);

    return fd<0?false:(close(fd), true);
}

#ifndef SCRIPT_SHELL_SYSTEM
#define SCRIPT_SHELL_SYSTEM     "/etc/utils/system.sh"
#endif

#define os_p_system(_fmt, _args...) ({  \
    int os_p_system_err = 0;        \
    int os_p_system_code = 0;       \
                                    \
    if (os_file_exist(SCRIPT_SHELL_SYSTEM)) { \
        os_p_system_err = os_v_spgeti(&os_p_system_code, SCRIPT_SHELL_SYSTEM " " _fmt, ##_args); \
        if (0==os_p_system_err) {   \
            os_p_system_err = os_p_system_code; \
        }                           \
    } else {                        \
        os_p_system_err = os_system(_fmt, ##_args); \
    }                               \
                                    \
    os_p_system_err;                \
})  /* end */

/******************************************************************************/
#endif /* __APP__ */
#endif /* __SFILE_H_CAF3737A24BFD00978F62CEB481C30B0__ */
