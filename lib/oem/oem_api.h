#ifndef __LIB_OEM_OEM_API_H__
#define __LIB_OEM_OEM_API_H__
/******************************************************************************/
#include "oem/oem.h"
#include "atenv/atenv.h"

#define OEM_FILE    "/etc/.oem"

static inline int
__oem_type_get_by_vendor(char *vendor)
{
    static struct {
        int type;
        char *vendor;
    } map[] = {
        { .type = OEM_DEFT, .vendor = OEM_NAME},
        { .type = OEM_1,    .vendor = OEM1_NAME},
    };

    int i;

    for (i=0; i<os_count_of(map); i++) {
        if (0==os_strcmp(vendor, map[i].vendor)) {
            return map[i].type;
        }
    }

    return OEM_END;
}

static inline int
__oem_type(void)
{
    int type;
    char vendor[1+OEM_NAME_LEN] = {0};

    if (os_file_exist(OEM_FILE)) {
        os_v_sfgets(vendor, OEM_NAME_LEN, OEM_FILE);
        type = __oem_type_get_by_vendor(vendor);
        if (is_good_oem_type(type)) {
            return type;
        }
    }

#if AT_PRODUCT==AT_PRODUCT_LTEFI_MD1
    os_v_spgets(vendor, OEM_NAME_LEN, "bootm oem.vendor");
#elif AT_PRODUCT==AT_PRODUCT_LTEFI_MD2
    os_v_spgets(vendor, OEM_NAME_LEN, "atenv oem/vendor");
#endif
    type = __oem_type_get_by_vendor(vendor);
    if (is_good_oem_type(type)) {
        return type;
    }

    return OEM_DEFT;
}

static inline oem_t *
__oem_get(void)
{
    static int type = OEM_END;

    if (OEM_END==type) {
        type = __oem_type();
    }

    return &__THIS_OEM[type];
}

#define oem_base_name       __oem_get()->base.name
#define oem_base_mac        __oem_get()->base.mac
#define oem_base_sn         __oem_get()->base.sn
#define oem_base_type       __oem_get()->base.type
#define oem_base_version    __oem_get()->base.version

#define oem_rsync_user      __oem_get()->rsync.user
#define oem_rsync_pwdfile   __oem_get()->rsync.pwdfile
#define oem_rsync_path      __oem_get()->rsync.path
#define oem_rsync_server    __oem_get()->rsync.server
#define oem_rsync_port      __oem_get()->rsync.port
#define oem_rsync_timeout   __oem_get()->rsync.timeout

#define oem_lss_user        __oem_get()->lss.user
#define oem_lss_password    __oem_get()->lss.password
#define oem_lss_server      __oem_get()->lss.server
#define oem_lss_port        __oem_get()->lss.port

#define oem_lss_ca          __oem_get()->cert.cert[OEM_APP_LSS][OEM_CERT_CA]
#define oem_lss_key         __oem_get()->cert.cert[OEM_APP_LSS][OEM_CERT_KEY]
#define oem_lss_cert        __oem_get()->cert.cert[OEM_APP_LSS][OEM_CERT_CERT]

#define oem_lms_ca          __oem_get()->cert.cert[OEM_APP_LMS][OEM_CERT_CA]
#define oem_lms_key         __oem_get()->cert.cert[OEM_APP_LMS][OEM_CERT_KEY]
#define oem_lms_cert        __oem_get()->cert.cert[OEM_APP_LMS][OEM_CERT_CERT]

#define oem_ums_ca          __oem_get()->cert.cert[OEM_APP_UMS][OEM_CERT_CA]
#define oem_ums_key         __oem_get()->cert.cert[OEM_APP_UMS][OEM_CERT_KEY]
#define oem_ums_cert        __oem_get()->cert.cert[OEM_APP_UMS][OEM_CERT_CERT]
/******************************************************************************/
#endif /* __LIB_OEM_OEM_API_H__ */
