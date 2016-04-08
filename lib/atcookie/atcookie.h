#ifndef __LIB_ATCOOKIE_ATCOOKIE_H__
#define __LIB_ATCOOKIE_ATCOOKIE_H__
/******************************************************************************/
#include "oem/oem_api.h"

enum {
    AT_COOKIE_BEGIN,
    
    AT_COOKIE_FILE = AT_COOKIE_BEGIN,
    AT_COOKIE_DOMAIN,

    AT_COOKIE_END
};

static inline bool
is_good_atcookie_id(int id)
{
    return id >= AT_COOKIE_BEGIN && id < AT_COOKIE_END;
}

enum {
    AT_CERT_INVALID = 0,

    AT_COOKIE_FILE_BEGIN   = 1,
    AT_RSYNC_PWDFILE= AT_COOKIE_FILE_BEGIN,
    
    AT_LMS_CERT     = 2,
    AT_LMS_KEY      = 3,
    AT_LMS_CA       = 4,
    
    AT_LSS_CERT     = 5,
    AT_LSS_KEY      = 6,
    AT_LSS_CA       = 7,
    
    AT_UMS_CERT     = 8,
    AT_UMS_KEY      = 9,
    AT_UMS_CA       = 10,

    AT_COOKIE_FILE_END
};

static inline bool
is_good_atcookie_file_id(int id)
{
    return id >= AT_COOKIE_FILE_BEGIN && id < AT_COOKIE_FILE_END;
}

typedef struct {
    char *info;
    int mode;
} atcookie_file_t;

#define __COOKIE_FILE_ENTRY(_info, _mode) {  \
    .info = _info,  \
    .mode = _mode,  \
}   /* end */

#define COOKIE_FILE_ENTRY(_info)   __COOKIE_FILE_ENTRY(_info, 0)

#define COOKIE_FILES    { \
    [AT_RSYNC_PWDFILE] = __COOKIE_FILE_ENTRY(oem_rsync_pwdfile, 0x600), \
    [AT_LMS_CERT]   = COOKIE_FILE_ENTRY(oem_lms_cert),  \
    [AT_LMS_KEY]    = COOKIE_FILE_ENTRY(oem_lms_key),   \
    [AT_LMS_CA]     = COOKIE_FILE_ENTRY(oem_lms_ca),    \
    [AT_LSS_CERT]   = COOKIE_FILE_ENTRY(oem_lss_cert),  \
    [AT_LSS_KEY]    = COOKIE_FILE_ENTRY(oem_lss_key),   \
    [AT_LSS_CA]     = COOKIE_FILE_ENTRY(oem_lss_ca),    \
    [AT_UMS_CERT]   = COOKIE_FILE_ENTRY(oem_ums_cert),  \
    [AT_UMS_KEY]    = COOKIE_FILE_ENTRY(oem_ums_key),   \
    [AT_UMS_CA]     = COOKIE_FILE_ENTRY(oem_ums_ca),    \
}   /* end */

#define AT_SECOOKIE_PREFIX  "/tmp/.cookie"
#define AT_SECOOKIE_LEN     sizeof(AT_SECOOKIE_PREFIX ".XXXXXXXXXXXXXXXX")

static inline bool
__atcookie_file_exist(char *file)
{
    int fd = open(file, O_RDONLY, S_IRUSR | S_IRGRP);

    return fd<0?false:(close(fd), true);
}

#ifdef IN_CURL
static inline char *
__atcookie_file_create(atcookie_file_t *cert, char *tmp_file)
{
    char *file = NULL;
    char *decode = NULL;
    int len = 0;

    int err = Curl_base64_decode(cert->info, &decode, &len);
    if (err) {
        return NULL;
    }

    FILE *f = fopen(tmp_file, "r+");
    if (NULL==f) {
        goto error;
    }

    fwrite(decode, len, 1, f);
    fclose(f);
    
error:
    if (decode) {
        free(decode);
    }
    
    return file;
}
#else
static inline char *
__atcookie_file_create(atcookie_file_t *cert, char *tmp_file)
{
    char *cmd = (char *)calloc(256, 1024);
    if (NULL==cmd) {
        return NULL;
    }
    
    sprintf(cmd, "echo %s | base64 -d > %s", cert->info, tmp_file);
    system(cmd);
    
    if (cert->mode) {
        chmod(tmp_file, cert->mode);
    }

    free(cmd);

    return tmp_file;
}
#endif

static char *
__atcookie_file(int id, char *tmp_file)
{
    int err;
    atcookie_file_t cookie[AT_COOKIE_FILE_END] = COOKIE_FILES;

    if (false==is_good_atcookie_file_id(id)) {
        return NULL;
    }

    atcookie_file_t *cert = &cookie[id];
    if (NULL==cert) {
        return NULL;
    }
    
    srand(time(NULL));
    
    do{
        sprintf(tmp_file, AT_SECOOKIE_PREFIX ".%x%x", getpid(), rand());
    } while(__atcookie_file_exist(tmp_file));
    
    return __atcookie_file_create(cert, tmp_file);
}

static char *
atcookie_file(int id)
{
    static char tmp_file[1+AT_SECOOKIE_LEN];

    return __atcookie_file(id, tmp_file);
}
/******************************************************************************/

typedef struct {
    bool https;
    char *domain;
    char *path[0];
    int count;
} atcookie_domain_t;

static char *
__atcookie_domain_get(char *domain)
{
    static char *domains[] = {
        "http://update1.9797168.com:821/wifibox",
        
        "https://atbus.autelan.com:8443/LMS/lte/lte",
        ".autelan.com",
        ".pepfi.com",
        "ftp://lms2.autelan.com",
        
    };

    return NULL;
}

/******************************************************************************/
#endif /* __LIB_ATCOOKIE_ATCOOKIE_H__ */
