#ifndef __LIB_UTILS_SOCKER_H__
#define __LIB_UTILS_SOCKER_H__
#ifdef __APP__
/******************************************************************************/
typedef union {
    struct sockaddr_un un;
    struct sockaddr_in in;
    struct sockaddr_ll ll;
    struct sockaddr_nl nl;
    struct sockaddr    c;
} os_sockaddr_t;

#define OS_SOCKADDR_INITER(_family)         {.c = {.sa_family = _family}}
#define OS_SOCKADDR_ZERO()                  OS_SOCKADDR_INITER(0)
#define OS_SOCKADDR_UN_INITER(_path)        {.un = OS_SOCKADDR_UNIX("\0" _path)}
#define OS_SOCKADDR_IN_INITER(_ip, _port)   {.in = OS_SOCKADDR_INET(_ip, _port)}

static inline bool
is_abstract_sockaddr(void *addr)
{
    struct sockaddr_un *uaddr = (struct sockaddr_un *)addr;

    if (AF_UNIX==uaddr->sun_family) {
        return 0==uaddr->sun_path[0] && uaddr->sun_path[1];
    } else {
        return false;
    }
}

static inline char *
get_abstract_path(struct sockaddr_un *addr)
{
    return &addr->sun_path[1];
}

static inline void
set_abstract_path(struct sockaddr_un *addr, const char *path)
{
    addr->sun_path[0] = 0;

    os_memcpy(get_abstract_path(addr), path, os_strlen(path));
}

#define unix_path_size      (sizeof(struct sockaddr_un) - offsetof(struct sockaddr_un, sun_path))
#define abstract_path_size  (unix_path_size - 1)

static inline void
set_abstract_sockaddr_len(struct sockaddr_un *addr, int addrlen)
{
    addr->sun_path[addrlen - sizeof(addr->sun_family)] = 0;
}

static inline socklen_t
get_abstract_sockaddr_len(struct sockaddr_un *addr)
{
    return sizeof(addr->sun_family) + 1 + os_strlen(get_abstract_path(addr));
}

static inline socklen_t
os_sockaddr_len(struct sockaddr *addr)
{
    switch(addr->sa_family) {
        case AF_UNIX:
            if (is_abstract_sockaddr(addr)) {
                return get_abstract_sockaddr_len((struct sockaddr_un *)addr);
            } else {
                return sizeof(struct sockaddr_un);
            }
        case AF_INET:
            return sizeof(struct sockaddr_in);
        case AF_PACKET:
            return sizeof(struct sockaddr_ll);
#if 0
        case AF_INET6:
            return sizeof(struct sockaddr_in6);
#endif
        case AF_NETLINK:
            return sizeof(struct sockaddr_nl);
        default:
            return sizeof(struct sockaddr);
    }
}
/******************************************************************************/
#endif
#endif /* __LIB_UTILS_SOCKER_H__ */
