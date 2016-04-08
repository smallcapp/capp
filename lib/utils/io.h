#ifndef __LIB_UTILS_IO_H__
#define __LIB_UTILS_IO_H__
/******************************************************************************/
#include "utils/time.h"
/******************************************************************************/
#ifdef __APP__

static inline int
__io_error(char *io, char *buf, int size, int error)
{
    int err = error;
    
    if (err>0) {
        os_dump_buffer(__is_debug_level_packet, buf, err, NULL);
    }
    else if (err<0) {
        debug_io("%s error:%d", io, -errno);

        return -errno;
    }

    return err;
}

static inline int
__io_read_error(char *io, char *buf, int size, int error)
{
    int err = __io_error(io, buf, size, error);
    if (0==err) {
        debug_io("%s nothing", io);

        return -EIO;
    }

    return err;
}

static inline int
__io_write_error(char *io, char *buf, int size, int error)
{
    int err = __io_error(io, buf, size, error);
    if (err != size) {
        debug_io("%s count(%d) < length(%d)", io, err, size);

        return -EIO;
    }

    return err;
}

#define __IO_READ(_action, _fd, _buf, _len, _timeout, _call)  ({ \
    fd_set rset;                                        \
    struct timeval tv = {                               \
        .tv_sec     = time_sec(_timeout),               \
        .tv_usec    = time_usec(_timeout),              \
    };                                                  \
    int err;                                            \
                                                        \
    while(1) {                                          \
        FD_ZERO(&rset);                                 \
        FD_SET(fd, &rset);                              \
                                                        \
        err = select(fd + 1, &rset, NULL, NULL, &tv);   \
        if (-1==err) {                                  \
            if (EINTR==errno) {                         \
                debug_signal("%s breaked", _action);    \
                                                        \
                continue;                               \
            } else {                                    \
                debug_io("%s error:%d", _action, -errno); \
                                                        \
                err = -errno;                           \
            }                                           \
        }                                               \
        else if (0==err) {                              \
            debug_timeout("%s timeout", _action);       \
                                                        \
            err = -ETIMEOUT;                            \
        }                                               \
        else {                                          \
            err = _call;                                \
        }                                               \
                                                        \
        break;                                          \
    }                                                   \
                                                        \
    err;                                                \
})

static inline int
__io_read(int fd, char *buf, int size)
{
    int err = read(fd, buf, size);

    return __io_read_error("read", buf, size, err);
}

static inline int
__io_recvfrom(int fd, char *buf, int size, int flag, struct sockaddr *addr, socklen_t *paddrlen)
{
    int err = recvfrom(fd, buf, size, flag, addr, paddrlen);

    return __io_read_error("recvfrom", buf, size, err);
}

static inline int
__io_recv(int fd, char *buf, int size, int flag)
{
    int err = recv(fd, buf, size, flag);

    return __io_read_error("recv", buf, size, err);
}

static inline int
io_read(int fd, char *buf, int size, int timeout)
{
    return __IO_READ("read", fd, buf, size, timeout, __io_read(fd, buf, size));
}

static inline void
io_drop(int fd)
{
    char buf[1+OS_LINE_LEN];

    while(read(fd, buf, sizeof(buf)) > 0) {
        os_do_nothing();
    }
}

static inline int
io_write(int fd, char *buf, int len)
{
    int err = write(fd, buf, len);

    return __io_write_error("write", buf, len, err);
}

static inline int
io_recvfrom(int fd, char *buf, int size, int timeout, struct sockaddr *addr, socklen_t *paddrlen)
{
    return __IO_READ("recvfrom", fd, buf, size, timeout, __io_recvfrom(fd, buf, size, 0, addr, paddrlen));
}

static inline int
io_sendto(int fd, char *buf, int len, struct sockaddr *addr, int addrlen)
{
    int err = sendto(fd, buf, len, 0, addr, addrlen);

    return __io_write_error("sendto", buf, len, err);
}

static inline int
io_recv(int fd, char *buf, int size, int timeout)
{
    return __IO_READ("recv", fd, buf, size, timeout, __io_recv(fd, buf, size, 0));
}

static inline int
io_send(int fd, char *buf, int len)
{
    int err = send(fd, buf, len, 0);

    return __io_write_error("send", buf, len, err);
}

static inline void
__iov_dump(struct iovec *iov, int count)
{
    int i;
    bool debug = __is_debug_level_packet;
    
    if (debug) {
        for (i=0; i<count; i++) {
            os_dump_buffer(debug, iov[i].iov_base, iov[i].iov_len, NULL);
        }
    }
}

static inline int
__iov_dump_error(int error, struct iovec *iov, int count)
{
    if (error<0) {
        error = -errno;
    }
    else if (error>0) {
        __iov_dump(iov, count);
    }
    
    return error;
}

#ifndef IOV_MAX
#define IOV_MAX     32
#endif

static inline int
io_vsendmsg_l(int fd, struct sockaddr *addr, int size, va_list args)
{
    struct iovec iov[IOV_MAX] = {[0 ... (IOV_MAX-1)] = OS_IOVEC_INITER(NULL, 0)};
    void *base;
    int len, count = 0;
    
    while((base = va_arg(args, void *))) {
        len = va_arg(args, int);

        iov[count].iov_base = base;
        iov[count].iov_len  = len;
        count++;
        
        if (count > IOV_MAX) {
            return -ETOOBIG;
        }
    }

    struct msghdr msg = OS_MSGHDR_INITER(iov, count, addr, size);
    
    return __iov_dump_error(sendmsg(fd, &msg, 0), iov, count);
}

static inline int
io_sendmsg_l(int fd, struct sockaddr *addr, int size, ...)
{
    int len = 0;
    va_list args;
    
    va_start(args, size);
    len = io_vsendmsg_l(fd, addr, size, args);
    va_end(args);
    
    return len;
}

static inline int
io_sendmsg(int fd, struct sockaddr *addr, int size, struct iovec *iov, int count)
{
    struct msghdr msg = OS_MSGHDR_INITER(iov, count, addr, size);
        
    return __iov_dump_error(sendmsg(fd, &msg, 0), iov, count);
}

static inline int
io_writev(int fd, struct iovec *iov, int count)
{
    return __iov_dump_error(writev(fd, iov, count), iov, count);
}

#endif
/******************************************************************************/
#endif /* __LIB_UTILS_IO_H__ */
