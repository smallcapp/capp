#ifndef __LIB_UMD_UMD_H__
#define __LIB_UMD_UMD_H__
/******************************************************************************/
#include "um/um.h"

#ifndef UM_USE_AUTO
#define UM_USE_AUTO             1
#endif

#ifndef UM_USE_PROMISC
#define UM_USE_PROMISC          0
#endif

#ifndef UM_USE_BINDIF
#define UM_USE_BINDIF           1
#endif

#ifndef UM_HASHSIZE
#define UM_HASHSIZE             256
#endif

#define UM_HASHMASK             (UM_HASHSIZE-1)

#ifndef UM_CONFIG_FILE
#ifdef __PC__
#define UM_CONFIG_FILE          "./umd.conf"
#else
#define UM_CONFIG_FILE          "/tmp/config/umd.conf"
#endif
#endif

#ifdef __PC__
#define UM_SCRIPT_PREFIX        "./"
#else
#define UM_SCRIPT_PREFIX        "/etc/um/"
#endif

#ifndef UM_SCRIPT_EVENT
#define UM_SCRIPT_EVENT         UM_SCRIPT_PREFIX "umevent"
#endif

#ifndef UM_SCRIPT_IP
#define UM_SCRIPT_IP            UM_SCRIPT_PREFIX "umip"
#endif

#ifndef UM_SCRIPT_MAC
#define UM_SCRIPT_MAC           UM_SCRIPT_PREFIX "ummac"
#endif

#ifndef UM_TICKS
#define UM_TICKS                10
#endif

#ifndef UM_IDLE
#ifdef __PC__
#define UM_IDLE                 60
#else
#define UM_IDLE                 300
#endif
#endif

#ifndef UM_SNIFF_COUNT
#define UM_SNIFF_COUNT          32
#endif

#ifndef UM_INTF_FLOW_DEFT
#define UM_INTF_FLOW_DEFT       "eth0"
#endif

#ifndef UM_INTF_FLOW_ETHERTYPE
#define UM_INTF_FLOW_ETHERTYPE  ETHERTYPE_IP
#endif

#ifdef __PC__
#   ifndef UM_INTF_TC_DEFT
#   define UM_INTF_TC_DEFT         UM_INTF_FLOW_DEFT
#   endif
#   ifndef UM_INTF_TC_ETHERTYPE
#   define UM_INTF_TC_ETHERTYPE    ETHERTYPE_IP
#   endif
#else
#   ifndef UM_INTF_TC_DEFT
#   define UM_INTF_TC_DEFT         "eth0.1"
#   endif
#   ifndef UM_INTF_TC_ETHERTYPE
#   define UM_INTF_TC_ETHERTYPE    ETHERTYPE_VLAN
#   endif
#endif

enum {
    UM_STATE_NONE,
    UM_STATE_BIND,
    UM_STATE_AUTH,

    UM_STATE_END
};

static inline bool
is_good_user_state(int state)
{
    return is_good_enum(state, UM_STATE_END);
}

#define UM_STATE_STRINGS        {   \
    [UM_STATE_NONE]     = "none",   \
    [UM_STATE_BIND]     = "bind",   \
    [UM_STATE_AUTH]     = "auth",   \
}   /* end */

static inline bool
__is_none(int state)
{
    return UM_STATE_NONE==state;
}

static inline bool
__is_auth(int state)
{
    return UM_STATE_AUTH==state;
}

static inline bool
__is_bind(int state)
{
    return UM_STATE_BIND==state;
}

static inline bool
__have_bind(int state)
{
    return __is_bind(state) || __is_auth(state);
}

#if UM_USE_MONITOR
#define is_monitor(_user)   os_hasflag((_user)->state, UM_F_MONITOR)
#else
#define is_monitor(_user)   false
#endif

#define is_auth(_user)      __is_auth((_user)->state)
#define is_bind(_user)      __is_bind((_user)->state)
#define is_noused(_user)    (__is_none((_user)->state) && 0==(_user)->flags)
#define have_bind(_user)    __have_bind((_user)->state)

static inline char **
__user_state_strings(void)
{
    static char *array[UM_STATE_END] = UM_STATE_STRINGS;

    return array;
}

static inline char *
user_state_string(int state)
{
    char **array = __user_state_strings();
    
    return is_good_user_state(state)?array[state]:__unknow;
}

static inline int
user_state_idx(char *state)
{
    char **array = __user_state_strings();
    
    return os_getstringarrayidx(array, state, 0, UM_STATE_END);
}

enum {
    UM_DEAUTH_NONE,
    
    UM_DEAUTH_AUTO,
    UM_DEAUTH_ONLINETIME,
    UM_DEAUTH_FLOWLIMIT,
    UM_DEAUTH_ADMIN,
    UM_DEAUTH_AGING,
    UM_DEAUTH_INITIATIVE,
    
    UM_DEAUTH_END
};

#define UM_DEAUTH_REASONS               {   \
    [UM_DEAUTH_NONE]        = "none",       \
    [UM_DEAUTH_AUTO]        = "auto",       \
    [UM_DEAUTH_ONLINETIME]  = "onlinetime", \
    [UM_DEAUTH_FLOWLIMIT]   = "flowlimit",  \
    [UM_DEAUTH_ADMIN]       = "admin",      \
    [UM_DEAUTH_AGING]       = "aging",      \
    [UM_DEAUTH_INITIATIVE]  = "initiative", \
}   /* end */

static inline bool
is_good_deauth_reason(int reason)
{
    return is_good_enum(reason, UM_DEAUTH_END);
}

static inline bool
is_valid_deauth_reason(int reason)
{
    return is_good_value(reason, UM_DEAUTH_NONE+1, UM_DEAUTH_END);
}

static inline char **
__deauth_reason_strings(void)
{
    static char *array[UM_DEAUTH_END] = UM_DEAUTH_REASONS;

    return array;
}

static inline char *
deauth_reason_string(int reason)
{
    char **array = __deauth_reason_strings();

    return is_good_deauth_reason(reason)?array[reason]:__unknow;
}

static inline int
deauth_reason_idx(char *reason)
{
    char **array = __deauth_reason_strings();
    
    return os_getstringarrayidx(array, reason, 0, UM_DEAUTH_END);
}

enum {
    um_flow_type_lan,
    um_flow_type_wan,
    
    um_flow_type_end,

    um_flow_type_local = um_flow_type_end,
};

#define UM_FLOW_TYPES           {   \
    [um_flow_type_lan]  = "lan",    \
    [um_flow_type_wan]  = "wan",    \
}   /* end */

static inline bool
is_good_flow_type(int type)
{
    return is_good_enum(type, um_flow_type_end);
}

static inline char **
__flow_type_string(void)
{
    static char *array[um_flow_type_end] = UM_FLOW_TYPES;

    return array;
}

static inline char *
flow_type_string(int type)
{
    char **array = __flow_type_string();

    return is_good_flow_type(type)?array[type]:__unknow;
}

static inline int
flow_type_idx(char *types)
{
    char **array = __flow_type_string();
    
    return os_getstringarrayidx(array, types, 0, um_flow_type_end);
}

enum {
    um_flow_dir_up,
    um_flow_dir_down,
    um_flow_dir_all,
    
    um_flow_dir_end
};

#define UM_FLOW_DIRS            {   \
    [um_flow_dir_up]    = "up",     \
    [um_flow_dir_down]  = "down",   \
    [um_flow_dir_all]   = "all",    \
}   /* end */

static inline bool
is_good_flow_dir(int dir)
{
    return is_good_enum(dir, um_flow_dir_end);
}

static inline char **
__flow_dir_string(void)
{
    static char *array[um_flow_dir_end] = UM_FLOW_DIRS;

    return array;
}

static inline char *
flow_dir_string(int dir)
{
    char **array = __flow_dir_string();

    return is_good_flow_dir(dir)?array[dir]:__unknow;
}

static inline int
flow_dir_idx(char *dirs)
{
    char **array = __flow_dir_string();
    
    return os_getstringarrayidx(array, dirs, 0, um_flow_dir_end);
}

struct um_limit_online {
    uint32_t max;
    uint32_t idle;
    uint32_t numerator;
    uint32_t denominator;

    int aging;
    time_t uptime;
    time_t downtime;
};

struct um_limit_flow {
    uint64_t max;
    uint64_t now;
    
    uint64_t numerator;
    uint64_t denominator;
};

struct um_limit_rate {
    uint32_t max;
    uint32_t avg;
};

struct um_limit {
    struct um_limit_online online;
    struct um_limit_flow flow[um_flow_dir_end];
    struct um_limit_rate rate[um_flow_dir_end];
};

struct um_tag {
    struct list_head tag;

    char *k;
    char *v;
};

#ifndef UM_SSID_MAX
#define UM_SSID_MAX     32
#endif

struct um_monitor {
    char ssid[os_align(1+UM_SSID_MAX, 4)];
};

#define UM_F_MONITOR    0x01

struct um_user {
    unsigned char mac[OS_MACSIZE];
    unsigned char flags;
    unsigned char __r0;
    uint32_t ip;
    
    int state;
    int reason;
    int group;

    time_t create;
    time_t noused;

    struct um_limit limit[um_flow_type_end];
    
#if UM_USE_MONITOR
    struct um_monitor monitor;
#endif

    struct {
        struct hlist_node mac;
        struct hlist_node ip;
        struct list_head  list;
    } node;

    struct {
        struct list_head tag;
    } head;
};

static inline struct um_limit *
__limit(struct um_user *user, int type)
{
    return &user->limit[type];
}

static inline struct um_limit_online *
__limit_online(struct um_user *user, int type)
{
    return &__limit(user, type)->online;
}

static inline struct um_limit_flow *
__limit_flow(struct um_user *user, int type, int dir)
{
    return &__limit(user, type)->flow[dir];
}

static inline struct um_limit_rate *
__limit_rate(struct um_user *user, int type, int dir)
{
    return &__limit(user, type)->rate[dir];
}

#define __limit_reauth(_max, _numerator, _denominator) ({ \
    typeof(_max) _m = (_max);                   \
    typeof(_numerator) _n = (_numerator);       \
    typeof(_denominator) _d = (_denominator);   \
                                                \
    (_m && _n)?(_d?(_m/_d)*_n:_n):0;            \
})

#define __online_max(_user, _type)          __limit_online(_user, _type)->max
#define __online_idle(_user, _type)         __limit_online(_user, _type)->idle
#define __online_aging(_user, _type)        __limit_online(_user, _type)->aging
#define __online_uptime(_user, _type)       __limit_online(_user, _type)->uptime
#define __online_downtime(_user, _type)     __limit_online(_user, _type)->downtime
#define __online_numerator(_user, _type)    __limit_online(_user, _type)->numerator
#define __online_denominator(_user, _type)  __limit_online(_user, _type)->denominator
#define __online_reauth(_user, _type)       \
    __limit_reauth(__online_max(_user, _type), __online_numerator(_user, _type), __online_denominator(_user, _type))

#define __flow_max(_user, _type, _dir)          __limit_flow(_user, _type, _dir)->max
#define __flow_now(_user, _type, _dir)          __limit_flow(_user, _type, _dir)->now
#define __flow_numerator(_user, _type, _dir)    __limit_flow(_user, _type, _dir)->numerator
#define __flow_denominator(_user, _type, _dir)  __limit_flow(_user, _type, _dir)->denominator
#define __flow_reauth(_user, _type, _dir)       \
    __limit_reauth(__flow_max(_user, _type, _dir), __flow_numerator(_user, _type, _dir), __flow_denominator(_user, _type, _dir))

#define __rate_max(_user, _type, _dir)          __limit_rate(_user, _type, _dir)->max
#define __rate_avg(_user, _type, _dir)          __limit_rate(_user, _type, _dir)->avg

#define limit_online(_user, _TYPE)          __limit_online(_user, um_flow_type_##_TYPE)
#define limit_flow(_user, _TYPE, _DIR)      __limit_flow(_user, um_flow_type_##_TYPE, um_flow_dir_##_DIR)
#define limit_rate(_user, _TYPE, _DIR)      __limit_rate(_user, um_flow_type_##_TYPE, um_flow_dir_##_DIR)

static inline void
__update_aging(struct um_user *user, int type, bool debug)
{
    __online_aging(user, type) = __online_idle(user, type);

    if (debug) {
        debug_aging("update %s aging to %d",
            flow_type_string(type),
            __online_aging(user, type));
    }
}

static inline void
update_aging(struct um_user *user, bool debug)
{
    __update_aging(user, um_flow_type_wan, debug);
    __update_aging(user, um_flow_type_lan, debug);
}

typedef multi_value_t um_foreach_f(struct um_user *user);
typedef multi_value_t um_get_f(struct um_user *user);

enum {
    UM_SERVER_TIMER,
    UM_SERVER_CLI,
    UM_SERVER_FLOW,
    
    UM_SERVER_END
};

struct um_intf {
    char name[1+OS_IFNAMELEN];
    
    unsigned int index;
    unsigned int ip;
    unsigned int mask;
    unsigned int flag;
    unsigned char mac[OS_MACSIZE], __r0[2];
};

enum {
    UM_INTF_FLOW    = 0,
#ifdef __PC__
    UM_INTF_TC      = 0,
#else
    UM_INTF_TC      = 1,
#endif

    UM_INTF_END
};

struct um_control {
    unsigned char basemac[OS_MACSIZE];
    unsigned int ticks;
    bool deinit;

    simpile_server_t *server[UM_SERVER_END];
    struct um_intf intf[UM_INTF_END];
    uint32_t gc;
    
    struct {
        struct hlist_head mac[UM_HASHSIZE];
        struct hlist_head ip[UM_HASHSIZE];
        struct list_head list;
        uint32_t count;
    } head;
};

extern struct um_control umd;

struct um_user_filter {
    int state;
    
    unsigned char mac[OS_MACSIZE];
    unsigned char macmask[OS_MACSIZE];
    
    uint32_t ip;
    uint32_t ipmask;
};

struct vlan_header {
#if __BYTE_ORDER == __BIG_ENDIAN
    unsigned short pri:3;
    unsigned short cfi:1;
    unsigned short vid:12;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned short vid:12;
    unsigned short cfi:1;
    unsigned short pri:3;
#endif

    unsigned short type;
};

struct um_flow {
    unsigned char packet[2048];
    int len;

    struct ether_header *eth;
    struct vlan_header  *vlan;
    struct ip           *iph;

    unsigned short ether_type_ip;
    unsigned short ether_type_vlan;
    unsigned short ether_type_all;
    unsigned short __r0;
    
    unsigned int userip;
    unsigned char *usermac;
    
    int type;
    int dir;
    
    uint32_t eth_packets;
    uint64_t eth_bytes;
    
    uint32_t vlan_packets;
    uint64_t vlan_bytes;
    
    uint32_t ip_packets;
    uint64_t ip_bytes;
};
/******************************************************************************/
extern jobj_t
um_juser(struct um_user *user);

static inline void
um_user_debug(struct um_user *user, char *tag, bool debug)
{
    if (debug) {
        jobj_t obj = um_juser(user);

        jdebug("%o", tag, jobj_string(obj));

        jobj_put(obj);
    }
}

extern jobj_t
um_jflow(void);

extern int
user_delete(struct um_user *user);

extern int
user_unbind(struct um_user *user, int reason);

extern int
user_reauth(struct um_user *user);

extern int
user_deauth(struct um_user *user, int reason);

extern struct um_tag *
um_tag_get(unsigned char mac[], char *k);

extern struct um_tag *
um_tag_set(unsigned char mac[], char *k, char *v);

extern int
um_user_delete(unsigned char mac[]);

extern struct um_user *
um_user_create(unsigned char mac[]);

#if UM_USE_MONITOR
extern struct um_user *
um_user_enter(unsigned char mac[], jobj_t obj);

extern int
um_user_leave(unsigned char mac[]);
#endif

extern struct um_user *
um_user_bind(unsigned char mac[], uint32_t ip);

extern int
um_user_unbind(unsigned char mac[]);

extern struct um_user *
um_user_auth(unsigned char mac[], int group, jobj_t obj);

extern int
um_user_reauth(unsigned char mac[]);

extern int
um_user_deauth(unsigned char mac[], int reason);

extern int
um_user_foreach(um_foreach_f *foreach);

extern int
um_user_foreach_safe(um_foreach_f *foreach);

extern struct um_user *
um_user_get(unsigned char mac[]);

extern struct um_user *
um_user_getbyip(uint32_t ip);

extern int
um_user_getby(struct um_user_filter *filter, um_get_f *get);

extern int
um_user_delbyip(uint32_t ip);

extern int
um_user_delby(struct um_user_filter *filter);

extern int
um_cfg_init(void);

#define UM_TEST_JSON    0x01

#ifndef UM_TEST
#ifdef __PC__
#define UM_TEST         UM_TEST_JSON
#else
#define UM_TEST         0
#endif
#endif

extern void
update_limit_test(void);

#if UM_USE_GC
extern multi_value_t 
umd_gc(struct um_user *user, time_t now);
#endif
/******************************************************************************/
#endif /* __LIB_UMD_UMD_H__ */
