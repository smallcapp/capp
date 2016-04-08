#ifndef __APP_BOOTM_BOOTM_H__
#define __APP_BOOTM_BOOTM_H__
/******************************************************************************/
#ifndef AT_ENV_LINE_SIZE
#define AT_ENV_LINE_SIZE        128
#endif

#ifndef AT_ENV_COUNT
#define AT_ENV_COUNT            100
#endif

#define AT_ROOTFS_OK            'o'
#define AT_ROOTFS_FAIL          'f'
#define AT_ROOTFS_VERFY         'v'

#define AT_ROOTFS_BASE          '7'

/*
* 只能新增表项，禁止修改已有表项的索引
*/
#define AT_ENV_INIT             0
#define AT_ENV_ROOTFS           1
#define AT_ENV_ROOTFS1          2
#define AT_ENV_ROOTFS1ERR       3
#define AT_ENV_ROOTFS2          4
#define AT_ENV_ROOTFS2ERR       5
#define AT_ENV_PCBA_MAC         6
#define AT_ENV_PCBA_SN          7
#define AT_ENV_PCBA_MODEL       8
#define AT_ENV_PCBA_VERSION     9
#define AT_ENV_PSN              10
#define AT_ENV_MID              11
#define AT_ENV_RT               12
#define AT_ENV_NA               13
#define AT_ENV_UT               14
#define AT_ENV_BOOTVER          15
#define AT_ENV_PCBA_VENDOR      16
#define AT_ENV_PCBA_COMPANY     17
#define AT_ENV_PRODUCT_MAC      18
#define AT_ENV_PRODUCT_SN       19
#define AT_ENV_PRODUCT_MODEL    20
#define AT_ENV_PRODUCT_VERSION  21
#define AT_ENV_PRODUCT_VENDOR   22
#define AT_ENV_PRODUCT_COMPANY  23
#define AT_ENV_PRODUCT_MANAGER  24

#define AT_ENV_OEM_MAC          33
#define AT_ENV_OEM_SN           34
#define AT_ENV_OEM_MODEL        35
#define AT_ENV_OEM_VERSION      36
#define AT_ENV_OEM_VENDOR       37
#define AT_ENV_OEM_COMPANY      38
#define AT_ENV_OEM_MANAGER      39

#define AT_ENV_PRIVATE          40
#define AT_ENV_PTEST            (AT_ENV_COUNT-1)

#define AT_DEFT_INIT            "f00d1e"
#define AT_DEFT_BOOTVER         "1.2"
#define AT_DEFT_ROOTFS          "1"
#define AT_DEFT_ROOTFS1         "o"
#define AT_DEFT_ROOTFS2         "o"
#define AT_DEFT_ROOTFS1ERR      "0"
#define AT_DEFT_ROOTFS2ERR      "0"

#define AT_NAME_INIT                "init"
#define AT_NAME_BOOTVER             "bootver"
#define AT_NAME_ROOTFS              "rootfs"
#define AT_NAME_ROOTFS1             "rootfs1"
#define AT_NAME_ROOTFS2             "rootfs2"
#define AT_NAME_ROOTFS1ERR          "rootfs1err"
#define AT_NAME_ROOTFS2ERR          "rootfs2err"
#define AT_NAME_PSN                 "psn"
#define AT_NAME_MID                 "mid"
#define AT_NAME_RT                  "rt"
#define AT_NAME_NA                  "na"
#define AT_NAME_UT                  "ut"

#define AT_NAME_PCBA_VENDOR         "pcba.vendor"
#define AT_NAME_PCBA_COMPANY        "pcba.company"
#define AT_NAME_PCBA_MODEL          "pcba.model"
#define AT_NAME_PCBA_MAC            "pcba.mac"
#define AT_NAME_PCBA_SN             "pcba.sn"
#define AT_NAME_PCBA_VERSION        "pcba.version"

#define AT_NAME_PRODUCT_VENDOR      "product.vendor"
#define AT_NAME_PRODUCT_COMPANY     "product.company"
#define AT_NAME_PRODUCT_MODEL       "product.model"
#define AT_NAME_PRODUCT_MAC         "product.mac"
#define AT_NAME_PRODUCT_SN          "product.sn"
#define AT_NAME_PRODUCT_VERSION     "product.version"
#define AT_NAME_PRODUCT_MANAGER     "product.manager"

#define AT_NAME_OEM_VENDOR          "oem.vendor"
#define AT_NAME_OEM_COMPANY         "oem.company"
#define AT_NAME_OEM_MODEL           "oem.model"
#define AT_NAME_OEM_MAC             "oem.mac"
#define AT_NAME_OEM_SN              "oem.sn"
#define AT_NAME_OEM_VERSION         "oem.version"
#define AT_NAME_OEM_MANAGER         "oem.manager"

#define AT_NAME_PRIVATE             "idx%d"

#define println(fmt, args...)   printf(fmt "\n", ##args)
#define dprintln println
#define countof_array(x)        (sizeof(x)/sizeof((x)[0]))


#define CONFIG_BOOTARGS_HEAD    \
    "root=/dev/mmcblk0p8"       \
        " "                     \
    /* end */

#define CONFIG_BOOTARGS_BODY    \
    "mem=2G"                    \
        " "                     \
    "console=ttyAMA0,115200"    \
        " "                     \
    "rootfstype=ext4"           \
        " "                     \
    "rootwait"                  \
        " "                     \
    "blkdevparts="              \
        "mmcblk0:"              \
        "512K(boot),"           \
        "512K(argv),"           \
        "4M(baseparam),"        \
        "4M(pqparam),"          \
        "4M(logo),"             \
        "8M(kernel),"           \
        "200M(rootfs0),"        \
        "200M(rootfs1),"        \
        "200M(rootfs2),"        \
        "3000M(rootfs_data),"   \
        "-(others)"             \
        " "                     \
    "mmz=ddr,0,0,300M"          \
    /* end */

#ifdef CONFIG_BOOTARGS
#undef CONFIG_BOOTARGS
#endif

#define CONFIG_BOOTARGS         \
    CONFIG_BOOTARGS_HEAD        \
    CONFIG_BOOTARGS_BODY        \
    /* end */

#ifdef CONFIG_BOOTCOMMAND
#undef CONFIG_BOOTCOMMAND
#endif

#define CONFIG_BOOTCOMMAND "mmc read 0 0x1000000 0x6800 0x4000;bootm 0x1000000"
/******************************************************************************/
#endif /* __APP_BOOTM_BOOTM_H__ */