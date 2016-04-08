#ifndef __LIB_OEM_OEM_H__
#define __LIB_OEM_OEM_H__
/******************************************************************************/
#ifndef OEM_RSYNC_USER_LEN
#define OEM_RSYNC_USER_LEN      31
#endif

#ifndef OEM_RSYNC_PWDFILE_LEN
#define OEM_RSYNC_PWDFILE_LEN   127
#endif

#ifndef OEM_RSYNC_PATH_LEN
#define OEM_RSYNC_PATH_LEN      127
#endif

#ifndef OEM_RSYNC_SERVER_LEN
#define OEM_RSYNC_SERVER_LEN    31
#endif

#ifndef OEM_RSYNC_PORT_LEN
#define OEM_RSYNC_PORT_LEN      15
#endif

#ifndef OEM_RSYNC_TIMEOUT_LEN
#define OEM_RSYNC_TIMEOUT_LEN   15
#endif

typedef struct {
    char user[1+OEM_RSYNC_USER_LEN];
    char pwdfile[1+OEM_RSYNC_PWDFILE_LEN];
    char path[1+OEM_RSYNC_PATH_LEN];
    char server[1+OEM_RSYNC_SERVER_LEN];
    char port[1+OEM_RSYNC_PORT_LEN];
    char timeout[1+OEM_RSYNC_TIMEOUT_LEN];
} oem_rsync_t;

#define __OEM_RSYNC_INITER(_user, _pwdfile, _path, _server, _port, _timeout) { \
    .user   = _user,    \
    .pwdfile= _pwdfile, \
    .path   = _path,    \
    .server = _server,  \
    .port   = _port,    \
    .timeout= _timeout, \
}

#ifndef OEM_LSS_USER_LEN
#define OEM_LSS_USER_LEN        31
#endif

#ifndef OEM_LSS_PASSWORD_LEN
#define OEM_LSS_PASSWORD_LEN    127
#endif

#ifndef OEM_LSS_SERVER_LEN
#define OEM_LSS_SERVER_LEN      31
#endif

#ifndef OEM_LSS_PORT_LEN
#define OEM_LSS_PORT_LEN        15
#endif

typedef struct {
    char user[1+OEM_LSS_USER_LEN];
    char password[1+OEM_LSS_PASSWORD_LEN];
    char server[1+OEM_LSS_SERVER_LEN];
    char port[1+OEM_LSS_PORT_LEN];
} oem_lss_t;

#define __OEM_LSS_INITER(_user, _pass, _server, _port) { \
    .user       = _user,    \
    .password   = _pass,    \
    .server     = _server,  \
    .port       = _port,    \
}

#ifndef OEM_NAME_LEN
#define OEM_NAME_LEN    31
#endif

#ifndef OEM_MAC_LEN
#define OEM_MAC_LEN     31
#endif

#ifndef OEM_SN_LEN
#define OEM_SN_LEN      63
#endif

#ifndef OEM_TYPE_LEN
#define OEM_TYPE_LEN    31
#endif

#ifndef OEM_VERSION_LEN
#define OEM_VERSION_LEN 31
#endif

typedef struct {
    char name[1+OEM_NAME_LEN];
    char mac[1+OEM_MAC_LEN];
    char sn[1+OEM_SN_LEN];
    char type[1+OEM_TYPE_LEN];
    char version[1+OEM_VERSION_LEN];
} oem_base_t;

#define __OEM_BASE_INITER(_name, _mac, _sn, _type, _version) { \
    .name   = _name,    \
    .mac    = _mac,     \
    .sn     = _sn,      \
    .type   = _type,    \
    .version= _version, \
}

enum {
    OEM_CERT_CA,
    OEM_CERT_KEY,
    OEM_CERT_CERT,

    OEM_CERT_END
};

enum {
    OEM_APP_LMS,
    OEM_APP_LSS,
    OEM_APP_UMS,

    OEM_APP_END
};

#ifndef OEM_CERT_LEN
#define OEM_CERT_LEN     (8*1024-1)
#endif

typedef struct {
    char cert[OEM_APP_END][OEM_CERT_END][1+OEM_CERT_LEN];
} oem_cert_t;

typedef struct {
    oem_base_t  base;
    oem_rsync_t rsync;
    oem_lss_t   lss;
    oem_cert_t  cert;
}
oem_t;

#define __OEM_INITER(_base, _rsync, _lss, _cert) { \
    .base   = _base,    \
    .rsync  = _rsync,   \
    .lss    = _lss,     \
    .cert   = _cert,    \
}
/******************************************************************************/
#include "deft.h"
#include "oem1.h"
/******************************************************************************/
enum {
    OEM_DEFT,
    OEM_1,
    
    OEM_END
};

static inline bool
is_good_oem_type(int type)
{
    return is_good_enum(type, OEM_END);
}

#define DECLARE_FAKE_OEM    extern oem_t __THIS_OEM[]
#define DECLARE_REAL_OEM    oem_t __THIS_OEM[OEM_END] = { \
    [OEM_DEFT]  = OEM_INITER,   \
    [OEM_1]     = OEM1_INITER,  \
}

#ifdef __BUSYBOX__
#define DECLARE_OEM     DECLARE_FAKE_OEM
#else
#define DECLARE_OEM     DECLARE_REAL_OEM
#endif

DECLARE_FAKE_OEM;
/******************************************************************************/
#endif /* __LIB_OEM_OEM_H__ */
