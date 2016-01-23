
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bootm.h"

#define BOOTENV_MMCBLK          "/dev/mmcblk0p2"

static int
env_common_check(char *value, char *array[], int count)
{
    int i;

    for (i=0; i<count; i++) {
        if (0==strcmp(array[i], value)) {
            return 0;
        }
    }

    return -1;
}

static int
env_rootfs_check(char *value)
{
    static char *array[] = { "0", "1", "2" };

    return env_common_check(value, array, countof_array(array));
}

static int
env_rootfsx_check(char *value)
{
    static char *array[] = { "o", "v", "f" };

    return env_common_check(value, array, countof_array(array));
}

static int
env_rootfsxerr_check(char *value)
{
    static char *array[] = { "0", "1", "2", "3" };

    return env_common_check(value, array, countof_array(array));
}

#define BOOT_VER_COUNT      2
#define BOOT_VER_MIN        0
#define BOOT_VER_MAX        9999

static int
env_bootver_check(char *value)
{
    int err = 0;
    int ver[BOOT_VER_COUNT];
    int i;
    
    err = sscanf(value, "%d.%d", &ver[0], &ver[1]);
    if (err<0) {
        return -errno;
    }

    for (i=0; i<BOOT_VER_COUNT; i++) {
        if (ver[i] < BOOT_VER_MIN || ver[i] > BOOT_VER_MAX) {
            return -1;
        }
    }
    
    return 0;
}

#define ENV_NAME_LEN            15
#define ENV_CHANGED             0x01
#define ENV_HIDDEN              0x02
#define ENV_READONLY            0x04

#define ENV_INITER(_name, _flag, _check) { \
    .name = _name,      \
    .flag = _flag,      \
    .check = _check,    \
}

#define ENV(_idx, _name, _flag, _check) \
    [_idx] = ENV_INITER(_name, _flag, _check)

static struct {
    char name[1+ENV_NAME_LEN];
    int flag;
    int (*check)(char *value);
} envctl[AT_ENV_COUNT] = {
    [0 ... (AT_ENV_COUNT-1)] = ENV_INITER("", ENV_HIDDEN, NULL),
    
    ENV(AT_ENV_INIT,    AT_NAME_INIT,       ENV_HIDDEN | ENV_READONLY,  NULL),
    ENV(AT_ENV_BOOTVER, AT_NAME_BOOTVER,    ENV_HIDDEN | ENV_READONLY,  NULL),
    ENV(AT_ENV_PSN,     AT_NAME_PSN,        ENV_HIDDEN | ENV_READONLY,  NULL),
    ENV(AT_ENV_MID,     AT_NAME_MID,        ENV_HIDDEN | ENV_READONLY,  NULL),

    ENV(AT_ENV_RT,              AT_NAME_RT,                 ENV_HIDDEN, NULL),
    ENV(AT_ENV_UT,              AT_NAME_UT,                 ENV_HIDDEN, NULL),
    ENV(AT_ENV_NA,              AT_NAME_NA,                 ENV_HIDDEN, NULL),
    
    ENV(AT_ENV_PCBA_VENDOR,     AT_NAME_PCBA_VENDOR,        ENV_HIDDEN, NULL),
    ENV(AT_ENV_PCBA_COMPANY,    AT_NAME_PCBA_COMPANY,       ENV_HIDDEN, NULL),
    ENV(AT_ENV_PCBA_MODEL,      AT_NAME_PCBA_MODEL,         ENV_HIDDEN, NULL),
    ENV(AT_ENV_PCBA_MAC,        AT_NAME_PCBA_MAC,           0, NULL),
    ENV(AT_ENV_PCBA_SN,         AT_NAME_PCBA_SN,            0, NULL),
    ENV(AT_ENV_PCBA_VERSION,    AT_NAME_PCBA_VERSION,       ENV_HIDDEN, NULL),
    
    ENV(AT_ENV_PRODUCT_VENDOR,  AT_NAME_PRODUCT_VENDOR,     ENV_HIDDEN, NULL),
    ENV(AT_ENV_PRODUCT_COMPANY, AT_NAME_PRODUCT_COMPANY,    ENV_HIDDEN, NULL),
    ENV(AT_ENV_PRODUCT_MODEL,   AT_NAME_PRODUCT_MODEL,      ENV_HIDDEN, NULL),
    ENV(AT_ENV_PRODUCT_MAC,     AT_NAME_PRODUCT_MAC,        ENV_HIDDEN, NULL),
    ENV(AT_ENV_PRODUCT_SN,      AT_NAME_PRODUCT_SN,         ENV_HIDDEN, NULL),
    ENV(AT_ENV_PRODUCT_VERSION, AT_NAME_PRODUCT_VERSION,    ENV_HIDDEN, NULL),
    ENV(AT_ENV_PRODUCT_MANAGE,  AT_NAME_PRODUCT_MANAGE,     ENV_HIDDEN, NULL),
    
    ENV(AT_ENV_OEM_VENDOR,      AT_NAME_OEM_VENDOR,         ENV_HIDDEN, NULL),
    ENV(AT_ENV_OEM_COMPANY,     AT_NAME_OEM_COMPANY,        ENV_HIDDEN, NULL),
    ENV(AT_ENV_OEM_MODEL,       AT_NAME_OEM_MODEL,          ENV_HIDDEN, NULL),
    ENV(AT_ENV_OEM_MAC,         AT_NAME_OEM_MAC,            ENV_HIDDEN, NULL),
    ENV(AT_ENV_OEM_SN,          AT_NAME_OEM_SN,             ENV_HIDDEN, NULL),
    ENV(AT_ENV_OEM_VERSION,     AT_NAME_OEM_VERSION,        ENV_HIDDEN, NULL),
    ENV(AT_ENV_OEM_MANAGE,      AT_NAME_OEM_MANAGE,         ENV_HIDDEN, NULL),

    ENV(AT_ENV_ROOTFS,      AT_NAME_ROOTFS,     0,  env_rootfs_check),
    ENV(AT_ENV_ROOTFS1,     AT_NAME_ROOTFS1,    0,  env_rootfsx_check),
    ENV(AT_ENV_ROOTFS2,     AT_NAME_ROOTFS2,    0,  env_rootfsx_check),
    ENV(AT_ENV_ROOTFS1ERR,  AT_NAME_ROOTFS1ERR, 0,  env_rootfsxerr_check),
    ENV(AT_ENV_ROOTFS2ERR,  AT_NAME_ROOTFS2ERR, 0,  env_rootfsxerr_check),

    ENV(AT_ENV_PTEST, "idx99", 0, NULL),
};

static int
env_check(int idx, char *value)
{
    if (NULL==envctl[idx].check || 0==(*envctl[idx].check)(value)) {
        return 0;
    } else {
        return -1;
    }
}

static char bootenv[AT_ENV_COUNT][AT_ENV_LINE_SIZE];

#define BLKSIZE     512
#define MULTIPLE    (BLKSIZE/AT_ENV_LINE_SIZE)

static int
init_private(void)
{
    int i;

    for (i=AT_ENV_PRIVATE; i<AT_ENV_COUNT; i++) {
        sprintf(envctl[i].name, AT_NAME_PRIVATE, i);
    }
}

static int
env_write(FILE *f)
{
    int i, k, idx, ret = 0;

    for (i=0; i<AT_ENV_COUNT; i++) {
        if (ENV_CHANGED & envctl[i].flag) {
            int err = fseek(f, AT_ENV_LINE_SIZE*i, SEEK_SET);
            if (err) {
                println("seek %s error(%d), skip it", envctl[i].name, -errno);

                ret = -1; continue;
            }
            
            int count = fwrite(bootenv[i], AT_ENV_LINE_SIZE, 1, f);
            if (1 != count) {
                println("write %s=%s error(%d), skip it", envctl[i].name, bootenv[i], -errno);

                ret = -1; continue;
            }
        }
    }

    return ret;
}

static int 
env_read(FILE *f)
{
    int count;
    
    count = fread((void *)bootenv, AT_ENV_LINE_SIZE, AT_ENV_COUNT, f);
    if (AT_ENV_COUNT != count) {
        println("read " BOOTENV_MMCBLK " error");

        return -1;
    }
    
    return 0;
}

void usage(int argc, char *argv[])
{
    println("%s ==> show all env", argv[0]);
    println("%s name ==> show env by name", argv[0]);
    println("%s name1=value1 name2=value2 ... ==> set env by name and value", argv[0]);
}

static int
getidx_byname(char *name)
{
    int i;

    for (i=0; i<AT_ENV_COUNT; i++) {
        if (0==strcmp(name, envctl[i].name)) {
            return i;
        }
    }

    return -1;
}

static char *
getvalue_byname(char *name)
{
    int idx = getidx_byname(name);

    if (idx<0) {
        return NULL;
    } else {
        return bootenv[idx];
    }
}

#ifndef __BUSYBOX__
#define bootm_main  main
#endif

int bootm_main(int argc, char *argv[])
{
    FILE *f = NULL;
    int count;
    int err = 0;
    int len;
    int idx;
    int i;
    char *name;
    char *value;

    init_private();
    
    f = fopen(BOOTENV_MMCBLK, "rb+");
    if (NULL==f) {
        println("can not open " BOOTENV_MMCBLK);
        
        err = -1; goto exit;
    }
    
    if (env_read(f)) {
        err = -1; goto exit;
    }
    
    if (1==argc) {
        for (i=1; i<AT_ENV_COUNT; i++) {
            if (bootenv[i][0] && !(ENV_HIDDEN & envctl[i].flag)) {
                println("%s=%s", envctl[i].name, bootenv[i]);
            }
        }

        err = 0; goto exit;
    }
    else if (2==argc) {
        char *name = argv[1];
 
        if (0==strcmp("-h", name) || 0==strcmp("--help", name)) {
            usage(argc, argv);
            err = 0; goto exit;
        }
        else if (NULL==strchr(name, '=')) {
            value = getvalue_byname(name);
            if (NULL==value) {
                println("argv[1](%s) bad name", name);

                err = -1;
            }
            else if (0==value[0]) {
                err = 0;
            }
            else {
                println("%s", value);
                
                err = 0;
            }

            goto exit;
        }
    }
    
    for (i=1; i<argc; i++) {
        char line[2*AT_ENV_LINE_SIZE] = {0};
        len = strlen(argv[i]);
        
        if (len > (2*AT_ENV_LINE_SIZE-1)) {
            println("argv[%d](%s) too long", i, argv[i]);

            err = -1; goto exit;
        }
        
        strcpy(line, argv[i]);

        name = line;
        value = strchr(line, '=');
        if (NULL==value) {
            println("argv[%d](%s) should as xxx=xxxx", i, argv[i]);

            err = -1; goto exit;
        }
        *value = 0; value++;
        
        idx = getidx_byname(name);
        if (idx<0) {
            println("argv[%d](%s) bad name(%s)", i, argv[i], name);
            
            err = -1; goto exit;
        }
        
        if (0==value[0]) {
            bootenv[idx][0] = 0;
        }
        else if (strlen(value) > (AT_ENV_LINE_SIZE-1)) {
            println("argv[%d](%s) value length max %d", i, argv[i], (AT_ENV_LINE_SIZE-1));

            err = -1; goto exit;
        }
        else if (env_check(idx, value) < 0) {
            println("argv[%d](%s) value is invalid", i, argv[i]);

            err = -1; goto exit;
        }
        else if (ENV_READONLY & envctl[idx].flag) {
            println("argv[%d](%s) is readonly", i, argv[i]);

            err = -1; goto exit;
        }
        else {
            strcpy(bootenv[idx], value);
            
            envctl[idx].flag |= ENV_CHANGED;
        }
    }
    
    if (env_write(f)) {
        err = -1; goto exit;
    }
    
exit:
    if (f) {
        fclose(f);
    }
    
    return err;
}

