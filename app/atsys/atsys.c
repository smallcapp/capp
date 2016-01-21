#ifndef __THIS_APP
#define __THIS_APP      atsys
#endif

#include "utils.h"
#include "atcookie/atcookie.h"

OS_INITER;

#if AT_PRODUCT != AT_PRODUCT_LTEFI_MD2
#error "just support ltefi(v2) md board"
#endif

AT_ENV_INITER;

#define DIR_KERNEL(_idx)    AT_DIR_KERNEL(_idx)
#define DIR_ROOTFS(_idx)    AT_DIR_ROOTFS(_idx)
#define DIR_CONFIG(_idx)    AT_DIR_CONFIG(_idx)
#define DIR_DATA(_idx)      AT_DIR_DATA(_idx)
#define DIR_OTHER           AT_DIR_OTHER

#define DIR_CONFIG_MASTER   __AT_DIR_CONFIG
#define DIR_DATA_MASTER     __AT_DIR_DATA

#define DIR_HD              AT_DIR_HD
#define DIR_SD              AT_DIR_SD
#define DIR_USB             AT_DIR_USB

#define DIR_USB_UPGRADE     DIR_USB "/upgrade"
#define DIR_USB_ROOTFS      DIR_USB_UPGRADE "/rootfs"
#define DIR_USB_CONFIG      DIR_USB_UPGRADE "/config"
#define DIR_USB_DATA        DIR_USB_UPGRADE "/data"
#define DIR_USB_OTHER       DIR_USB_UPGRADE "/other"

#define USB_FILE(_file)     DIR_USB_ROOTFS "/" _file

#define SCRIPT_MOUNT            AT_FILE("etc/mount/mount.sh")
#define SCRIPT_XCOPY            AT_FILE("usr/sbin/xcopy")
#define SCRIPT_CURRENT          AT_FILE("usr/sbin/syscurrent")

#define __FILE_VERSION              "etc/.version"
#define __FILE_ROOTFS_VERSION       __FILE_VERSION
#define __FILE_KERNEL_VERSION       __FILE_VERSION
#define __FILE_KERNEL               "image/hi_kernel.bin"
#define __FILE_BOOT                 "image/fastboot-burn.bin"
#define __FILE_BOOTENV              "image/bootenv.bin"
#define __FILE_AP_BOOT              "image/u-boot.bin"
#define __FILE_AP_BOOTENV           "image/u-bootenv"
#define FILE_ROOTFS_VERSION         AT_FILE(__FILE_ROOTFS_VERSION)
#define FILE_KERNEL_VERSION         AT_FILE(__FILE_KERNEL_VERSION)
#define FILE_KERNEL                 AT_FILE(__FILE_KERNEL)
#define FILE_BOOT                   AT_FILE(__FILE_BOOT)
#define FILE_BOOTENV                AT_FILE(__FILE_BOOTENV)
#define FILE_AP_BOOT                AT_FILE(__FILE_AP_BOOT)
#define FILE_AP_BOOTENV             AT_FILE(__FILE_AP_BOOTENV)
#define USB_FILE_ROOTFS_VERSION     USB_FILE(__FILE_ROOTFS_VERSION)
#define USB_FILE_KERNEL_VERSION     USB_FILE(__FILE_KERNEL_VERSION)
#define USB_FILE_KERNEL             USB_FILE(__FILE_KERNEL)
#define USB_FILE_BOOT               USB_FILE(__FILE_BOOT)
#define USB_FILE_BOOTENV            USB_FILE(__FILE_BOOTENV)
#define USB_FILE_AP_BOOT            USB_FILE(__FILE_AP_BOOT)
#define USB_FILE_AP_BOOTENV         USB_FILE(__FILE_AP_BOOTENV)

#define ENV_TIMEOUT         "__ENV_TIMEOUT__"
#define ENV_PWDFILE         "__ENV_PWDFILE__"
#define ENV_VERSION         "__ENV_VERSION__"
#define ENV_SERVER          "__ENV_SERVER__"
#define ENV_PORT            "__ENV_PORT__"
#define ENV_USER            "__ENV_USER__"
#define ENV_PATH            "__ENV_PATH__"
#define ENV_ROOTFS          "__ENV_ROOTFS__"
#define ENV_FORCE           "__ENV_FORCE__"

#define __OBJ(_idx, _dev, _dir)     [_idx] = {.dev = _dev, .dir = _dir}
#define OBJ_KERNEL(_idx)            __OBJ(_idx, DEV_KERNEL(_idx), DIR_KERNEL(_idx))
#define OBJ_ROOTFS(_idx)            __OBJ(_idx, DEV_ROOTFS(_idx), DIR_ROOTFS(_idx))
#define OBJ_CONFIG(_idx)            __OBJ(_idx, DEV_CONFIG(_idx), DIR_CONFIG(_idx))
#define OBJ_DATA(_idx)              __OBJ(_idx, DEV_DATA(_idx),   DIR_DATA(_idx))

static struct {
    int current;
    int cmaster;
    int dmaster;
    
    bool dirty;
    at_version_t version;
    at_version_t old_version;
    
    struct {
        char *timeout;
        char *pwdfile;
        char *version;
        char *server;
        char *port;
        char *user;
        char *path;
        char *rootfs;
        char *force;
    } env;

    struct {
        char *dev;
        char *dir;
    } kernel[AT_OS_COUNT], rootfs[AT_OS_COUNT], data[2], config[2];
} 
atsys = {
    .current= AT_OS_CURRENT,
    .dirty  = false,

    .kernel = {
        OBJ_KERNEL(0),
        OBJ_KERNEL(1),
        OBJ_KERNEL(2),
        OBJ_KERNEL(3),
        OBJ_KERNEL(4),
        OBJ_KERNEL(5),
        OBJ_KERNEL(6),
    },
    
    .rootfs = {
        OBJ_ROOTFS(0),
        OBJ_ROOTFS(1),
        OBJ_ROOTFS(2),
        OBJ_ROOTFS(3),
        OBJ_ROOTFS(4),
        OBJ_ROOTFS(5),
        OBJ_ROOTFS(6),
    },

    .config = {
        OBJ_CONFIG(0),
        OBJ_CONFIG(1),
    },

    .data = {
        OBJ_DATA(0),
        OBJ_DATA(1),
    },
};

#define __skips(_idx)           (__at_skips(atsys.current) | os_bit(_idx))

#define __cslave                (!atsys.cmaster)
#define __dslave                (!atsys.dmaster)
#define __cmaster               (!__cslave)
#define __dmaster               (!__dslave)

#define dev_kernel(_idx)        atsys.kernel[_idx].dev
#define dev_rootfs(_idx)        atsys.rootfs[_idx].dev
#define dev_config(_idx)        atsys.config[_idx].dev
#define dev_data(_idx)          atsys.data[_idx].dev

#define dev_kernel_current      dev_kernel(atsys.current)
#define dev_rootfs_current      dev_rootfs(atsys.current)
#define dev_config_master       dev_config(__cmaster)
#define dev_data_master         dev_data(__dmaster)

#define dir_kernel(_idx)        atsys.kernel[_idx].dir
#define dir_rootfs(_idx)        atsys.rootfs[_idx].dir
#define dir_config(_idx)        atsys.config[_idx].dir
#define dir_data(_idx)          atsys.data[_idx].dir

#define dir_kernel_current      dir_kernel(atsys.current)
#define dir_rootfs_current      dir_rootfs(atsys.current)
#define dir_config_master       dir_config(__cmaster)
#define dir_data_master         dir_data(__dmaster)

#define is_current_rootfs_dev(_dev) (0==os_strcmp(dev_rootfs_current, _dev))
#define is_current_rootfs_dir(_dir) (0==os_strcmp(dir_rootfs_current, _dir))

static char *
rootfs_file(int idx, char *file)
{
    static char line[1+OS_FILENAME_LEN];

    os_saprintf(line, "%s/%s", dir_rootfs(idx), file);

    return line;
}

#define dev_obj(_obj, _idx)     dev_##_obj(_idx)
#define dir_obj(_obj, _idx)     dir_##_obj(_idx)

#define set_obj(_obj, _idx, _field, _value) do{ \
    at_obj(_obj, _idx)->_field = _value;        \
                                                \
    atsys.dirty = true;                         \
}while(0)

#define efsm(_err)  ((_err)?AT_FSM_FAIL:AT_FSM_OK)

#define ATSYS_TRACE     0

#if ATSYS_TRACE
#define __trace(_fmt, _args...)     os_println(_fmt, ##_args)
#else
#define __trace(_fmt, _args...)     os_do_nothing()
#endif

static int
__dd(char *dst, char *src)
{
    return os_p_system("dd if=%s of=%s bs=4096", src, dst);
}

static int
__rsync(int idx, at_version_t *version)
{
    int err;
    char new[AT_VERSION_STRING_SIZE];
    char old[AT_VERSION_STRING_SIZE];
    char pwd[1+AT_SECOOKIE_LEN] = {0};
    char *pwdfile = atsys.env.pwdfile;
    
    if (NULL==atsys.env.pwdfile) {
        pwdfile = __atcookie_file(AT_RSYNC_PWDFILE, pwd);
        if (NULL==pwdfile) {
            return -EIO;
        }
    }

    __at_version_itoa(version, new);
    __at_version_itoa(&atsys.old_version, old);
    
    err = os_p_system("rsync"
            " -acz --delete --force --stats --partial"
            " --exclude=/dev/*"
            " --timeout=%s"
            " --password-file=%s"
            " rsync://%s@%s:%s/%s/%s/rootfs/"
            " %s",
            atsys.env.timeout,
            pwdfile,
            atsys.env.user,
                atsys.env.server,
                atsys.env.port,
                atsys.env.path,
                new,
            dir_rootfs(idx));
    set_obj(rootfs, idx, upgrade, efsm(err));
    if (NULL==atsys.env.pwdfile) {
        unlink(pwdfile);
    }
    
    jinfo("%o", 
        "upgrade", jobj_oprintf("%s%s%s%s%o%d",
                        "type", "cloud",
                        "obj", "rootfs",
                        "src", atsys.env.server,
                        "dst", dir_rootfs(idx),
                        "version", jobj_oprintf("%s%s", 
                                        "old", old, 
                                        "new", new),
                        "error", err));

    return err;
}

static int
__xcopy(char *dst, char *src)
{
    int err;
    
    __trace("before sync %s==>%s", src, dst);
    err = os_p_system(SCRIPT_XCOPY " %s %s", src, dst);
    __trace("after  sync %s==>%s", src, dst);
    
    return err;
}

static int
__rcopy(int idx, char *dir, at_version_t *version)
{
    int err;
    char new[AT_VERSION_STRING_SIZE];
    char old[AT_VERSION_STRING_SIZE];
    
    __at_version_itoa(version, new);
    __at_version_itoa(&atsys.old_version, old);
    
    err = __xcopy(dir_rootfs(idx), dir);
    set_obj(rootfs, idx, upgrade, efsm(err));
    
    jinfo("%o",
        "upgrade", jobj_oprintf("%s%s%s%s%o%d",
                        "type", "local",
                        "obj", "rootfs",
                        "src", dir,
                        "dst", dir_rootfs(idx),
                        "version", jobj_oprintf("%s%s", 
                                        "old", old, 
                                        "new", new),
                        "error", err));

    return err;
}

static int
__umount(char *dir)
{
    return os_p_system("umount %s", dir);
}

#if 0
#define ROOTFS_MOUNT_MODE   ",data=journal"
#else
#define ROOTFS_MOUNT_MODE   __empty
#endif

static int
__mount(char *dev, char *dir, bool readonly)
{
#if 0
mount has the following return codes (the bits can be ORed):

0:success
1 - incorrect invocation or permissions
2 - system error (out of memory, cannot fork, no more loop devices)
4 - internal mount bug
8 - user interrupt
16 - problems writing or locking /etc/mtab
32 - mount failure
64 - some mount succeeded
#endif
    int err;
    
    __trace("before mount %s", dev);
    err = os_p_system("mount -t ext4 "
            "-o %s,noatime,nodiratime "
            "%s %s",
            readonly?"ro":"rw" ROOTFS_MOUNT_MODE,
            dev, dir);
    __trace("after  mount %s", dev);

    return err;
}

static int
__remount(char *dir, bool readonly)
{
    int err = 0;

#if AT_ROOTFS_MODE_TYPE==AT_ROOTFS_MODE_TYPE_RO
    err = os_p_system("mount -o remount,%s %s",
                readonly?"ro":"rw" ROOTFS_MOUNT_MODE,
                dir);
#endif

    return err;
}

static int
do_umount(char *dir)
{
    if (is_current_rootfs_dev(dir) || is_current_rootfs_dir(dir)) {
        debug_trace("skip umount current rootfs %s", dir);
        
        return 0;
    } else {
        return __umount(dir);
    }
}

static int
do_mount(char *dev, char *dir, bool check, bool readonly, bool repair)
{
    return os_p_system(SCRIPT_MOUNT " %s %s %s %s %s", 
                dev, 
                dir,
                check?"yes":"no",
                readonly?"ro":"rw",
                repair?"yes":"no");
}

static int
do_mount_wait(int wait, char *dev, char *dir, bool check, bool readonly, bool repair)
{
    int i, err;

    for (i=0; i<wait; i++) {
        if (os_file_exist(dev)) {
            return do_mount(dev, dir, check, readonly, repair);
        }

        sleep(1);
    }

    return -ENOEXIST;
}

static int
mount_double(
    char *name, char *dir_master,
    char *dev0, char *dir0,
    char *dev1, char *dir1,
    int *master
)
{
    int err;
    int err0 = 0;
    int err1 = 0;
    
    err0 = do_mount(dev0, dir_master, 
            true,
            false,
            true);
    err1 = do_mount(dev1, dir1, 
            true,
            false,
            true);

    if (err0 || err1) {
        trace_assert(0, "mount %s error:%d and %s error:%d", dev0, err0, dev1, err1);
        
        return -EMOUNT;
    }
    else if (err0) {
        __xcopy(dir0, dir1);
    }
    
    else if (err1) {
        __xcopy(dir1, dir0);
    }
    
    *master = 0;
    
    return 0;
}

static int
mount_config(void)
{
    return mount_double("config", DIR_CONFIG_MASTER,
                dev_config(0), dir_config(0), 
                dev_config(1), dir_config(1),
                &atsys.cmaster);
}

static int
mount_data(void)
{
    return mount_double("data", DIR_DATA_MASTER,
                dev_data(0), dir_data(0), 
                dev_data(1), dir_data(1),
                &atsys.dmaster);
}

static int
mount_other(void)
{
    return do_mount(DEV_OTHER, DIR_OTHER, 
                true,
                false,
                true);
}

static int
mount_rootfs(void)
{
    int i, err, errs = 0;
    bool readonly = true;

    if (0==os_strcmp("rw", AT_ROOTFS_MODE)) {
        readonly = false;
    }
    
    for (i=0; i<AT_OS_COUNT; i++) {
        if (i!=atsys.current) {
            err = do_mount(dev_rootfs(i), dir_rootfs(i), 
                    true,
                    readonly,
                    true);
            if (err) {
                errs = err;
            }
        }
    }

    return errs;
}

static int
mount_hd(void)
{
    return do_mount_wait(3, DEV_HD, DIR_HD, 
            true,
            false,
            false);
}

static int
mount_sd(void)
{
    return do_mount_wait(2, DEV_SD, DIR_SD, 
            true,
            false,
            false);
}

static int
mount_usb(void)
{
    return do_mount_wait(2, DEV_USB, DIR_USB, 
            true,
            false,
            false);
}

static int
umount_double(char *dir_master, char *dir0, char *dir1)
{
    int err;

    err = do_umount(dir_master);
    if (err) {

    }

    err = do_umount(dir0);
    if (err) {

    }

    err = do_umount(dir1);
    if (err) {

    }

    return err;
}

static int
umount_config(void)
{
    return umount_double(DIR_CONFIG_MASTER, dir_config(0), dir_config(1));
}

static int
umount_data(void)
{
    return umount_double(DIR_DATA_MASTER, dir_data(0), dir_data(1));
}

static int
umount_other(void)
{
    return do_umount(DIR_OTHER);
}

static int
umount_rootfs(void)
{
    int i, err, errs = 0;

    for (i=0; i<AT_OS_COUNT; i++) {
        if (i!=atsys.current) {
            err = do_umount(dir_rootfs(i));
            if (err) {
                errs = err;
            }
        }
    }

    return errs;
}

static int
umount_hd(void)
{
    return do_umount(DIR_HD);
}

static int
umount_sd(void)
{
    return do_umount(DIR_SD);
}

static int
umount_usb(void)
{
    return do_umount(DIR_USB);
}

static struct {
    int (*mount)(void);
    int (*umount)(void);
} mounts[] = {
    {
         .mount =  mount_rootfs,
        .umount = umount_rootfs,
    },
    {
         .mount =  mount_config,
        .umount = umount_config,
    },
    {
         .mount =  mount_data,
        .umount = umount_data,
    },
    {
         .mount =  mount_other,
        .umount = umount_other,
    },
    {
         .mount =  mount_sd,
        .umount = umount_sd,
    },
    {
         .mount =  mount_hd,
        .umount = umount_hd,
    },
    {
         .mount =  mount_usb,
        .umount = umount_usb,
    },
};

static at_version_t *
__get_version(int idx, char *file)
{
    static at_version_t version;
    
    char string[1+OS_LINE_LEN] = {0};
    int err;
    
    err = os_v_sfgets(string, OS_LINE_LEN, "%s/%s", dir_rootfs(idx), file);
    if (err) {
        debug_error("read version file %s/%s error:%d", dir_rootfs(idx), file, err);
        
        return NULL;
    }
    else if (__at_version_atoi(&version, string)) {
        debug_error("bad version file %s/%s", dir_rootfs(idx), file);
        
        return NULL;
    }
    else {
        return &version;
    }
}

static at_version_t *
get_rootfs_version(int idx)
{
    return __get_version(idx, __FILE_ROOTFS_VERSION);
}

static at_version_t *
get_kernel_version(int idx)
{
    return __get_version(idx, __FILE_KERNEL_VERSION);
}

static int
save(void)
{
    int err = 0;
    
    if (atsys.dirty) {
        err = os_call(at_open, at_close, __at_save, AT_ENV_OS);
        
        atsys.dirty = false;
    }

    return err;
}

static int
load(void)
{
    save();
    
    return os_call(at_open, at_close, __at_load, AT_ENV_OS);
}

#define __upgrade_init(_obj, _idx, _version)    do{ \
    at_obj(_obj, _idx)->upgrade = AT_FSM_UNKNOW;    \
    at_obj(_obj, _idx)->other   = AT_FSM_UNKNOW;    \
    at_obj(_obj, _idx)->self    = AT_FSM_UNKNOW;    \
    at_obj(_obj, _idx)->error   = 0;                \
                                                    \
    os_objcpy(at_obj_version(_obj, _idx), _version);\
                                                    \
    os_objcpy(&atsys.old_version, at_obj_version(_obj, _idx)); \
    atsys.dirty = true;                             \
}while(0)

static void
upgrade_init(int idx, at_version_t *version)
{
    __upgrade_init(rootfs, idx, version);
    __upgrade_init(kernel, idx, version);

    save();
}


static int
__dir_rsync_init(int idx, at_version_t *version)
{
    __upgrade_init(rootfs, idx, version);
    
    save();
    
    return __remount(dir_rootfs(idx), false);
}

static int
__dir_rsync_fini(int idx, at_version_t *version)
{
    (void)version;
    
    save();
    
    return __remount(dir_rootfs(idx), true);
}

static int
rsync(int idx, at_version_t *version)
{
    return os_call_2(__dir_rsync_init, __dir_rsync_fini, __rsync, idx, version);
}

static int
__dir_rcopy_init(int dst, char *dir, at_version_t *version)
{
    (void)dir;
    
    __upgrade_init(rootfs, dst, version);
    
    save();
    
    return __remount(dir_rootfs(dst), false);
}

static int
__dir_rcopy_fini(int dst, char *dir, at_version_t *version)
{
    (void)dir;
    (void)version;
    
    save();
    
    return __remount(dir_rootfs(dst), true);
}

static int
rcopy(int idx, char *dir, at_version_t *version)
{
    return os_call_3(__dir_rcopy_init, __dir_rcopy_fini, __rcopy, idx, dir, version);
}

static int
__rdd_init(int dst, int src)
{
    __upgrade_init(rootfs, dst, at_rootfs_version(src));
    
    return save();
}

static int
__rdd_fini(int dst, int src)
{
    (void)dst;
    (void)src;
    
    return save();
}

static int
__rdd(int dst, int src)
{
    int err;
    char new[AT_VERSION_STRING_SIZE];
    char old[AT_VERSION_STRING_SIZE];
    
    __at_version_itoa(at_rootfs_version(src), new);
    __at_version_itoa(&atsys.old_version, old);

    err = __dd(dev_rootfs(dst), dev_rootfs(src));
    set_obj(rootfs, dst, upgrade, efsm(err));

    jinfo("%o",
        "upgrade", jobj_oprintf("%s%s%s%s%o%d",
                        "type", "local",
                        "obj", "rootfs",
                        "src", dev_rootfs(src),
                        "dst", dev_rootfs(dst),
                        "version", jobj_oprintf("%s%s", 
                                        "old", old, 
                                        "new", new),
                        "error", err));

    return err;
}

static int
rdd(int dst, int src)
{
    return os_call_2(__rdd_init, __rdd_fini, __rdd, dst, src);
}

static int
__kdd_bydev_init(int dst, int src)
{
    (void)src;
    
    __upgrade_init(kernel, dst, at_kernel_version(src));
    
    return save();
}

static int
__kdd_bydev_fini(int dst, int src)
{
    (void)dst;
    (void)src;
    
    return save();
}

static int
__kdd_bydev(int dst, int src)
{
    int err;
    char new[AT_VERSION_STRING_SIZE];
    char old[AT_VERSION_STRING_SIZE];
    
    os_strcpy(new, at_kernel_version_string(src));
    __at_version_itoa(&atsys.old_version, old);
    
    err = __dd(dev_kernel(dst), dev_kernel(src));
    set_obj(kernel, dst, upgrade, efsm(err));

    jinfo("%o",
        "upgrade", jobj_oprintf("%s%s%s%s%o%d",
                        "type", "local",
                        "obj", "kernel",
                        "src", dev_kernel(src),
                        "dst", dev_kernel(dst),
                        "version", jobj_oprintf("%s%s", 
                                        "old", old, 
                                        "new", new),
                        "error", err));

    return err;
}

static int
kdd_bydev(int dst, int src)
{
    return os_call_2(__kdd_bydev_init, __kdd_bydev_fini, __kdd_bydev, dst, src);
}

static int
__kdd_byfile_init(int idx, char *file, at_version_t *version)
{
    (void)file;
    
    __upgrade_init(kernel, idx, version);
    
    return save();
}

static int
__kdd_byfile_fini(int idx, char *file, at_version_t *version)
{
    (void)idx;
    (void)file;
    (void)version;
    
    return save();
}

static int
__kdd_byfile(int idx, char *file, at_version_t *version)
{
    int err;
    char new[AT_VERSION_STRING_SIZE];
    char old[AT_VERSION_STRING_SIZE];
    
    __at_version_itoa(version, new);
    __at_version_itoa(&atsys.old_version, old);
    
    err = __dd(dev_kernel(idx), file);
    set_obj(kernel, idx, upgrade, efsm(err));

    jinfo("%o",
        "upgrade", jobj_oprintf("%s%s%s%s%o%d",
                        "type", "local",
                        "obj", "kernel",
                        "src", file,
                        "dst", dev_kernel(idx),
                        "version", jobj_oprintf("%s%s", 
                                        "old", old, 
                                        "new", new),
                        "error", err));

    return err;
}

static int
kdd_byfile(int idx, char *file, at_version_t *version)
{
    return os_call_3(__kdd_byfile_init, __kdd_byfile_fini, __kdd_byfile, idx, file, version);
}

static int
bdd(char *obj, char *dev, char *file)
{
    int err;

    err = __dd(dev, file);
    
    jinfo("%o",
        "upgrade", jobj_oprintf("%s%s%s%s%d",
                        "type", "local",
                        "obj", obj,
                        "src", file,
                        "dst", dev,
                        "error", err));

    return err;
}

static int
switch_to(int idx)
{
    jinfo("%o", 
        "switch", jobj_oprintf("%d%d",
                        "from", __at_current,
                        "to", idx));

    at_rootfs(idx)->error   = 0;
    at_kernel(idx)->error   = 0;
    
    at_rootfs(idx)->upgrade = AT_FSM_OK;
    at_kernel(idx)->upgrade = AT_FSM_OK;
    
    at_rootfs(idx)->self    = AT_FSM_UNKNOW;
    at_kernel(idx)->self    = AT_FSM_UNKNOW;
    
    __at_current = idx;
    atsys.dirty = true;
    save();
    
    return 0;
}

static void 
debug_rootfs_upgrade(int idx, char *master, char *rootfs)
{
    debug_ok("%s upgrade %s rootfs%d %s==>%s ", 
        master, 
        rootfs, 
        idx, 
        at_rootfs_version_string(idx), 
        atsys.env.version);
}

static int
get_upgrade_master(void)
{
    int skips = __skips(0);
    int idx;

#if AT_UPGRADE_TWO==AT_UPGRADE
    idx = at_find_first_good_byversion(rootfs, &atsys.version, skips);
    if (__at_rootfs_is_good(idx)) {
        debug_rootfs_upgrade(idx, "master", "good");

        return idx;
    }

    idx = at_find_first_byversion(rootfs, &atsys.version, skips);
    if (is_good_atidx(idx)) {
        debug_rootfs_upgrade(idx, "master", "bad");

        return idx;
    }
#endif

    idx = at_find_worst(rootfs, skips);
    if (is_good_atidx(idx)) {
        debug_rootfs_upgrade(idx, "master", "worst");

        return idx;
    }

    trace_assert(0, "no found worst master");
    idx = 1==atsys.current?2:1;

    debug_rootfs_upgrade(idx, "master", "force");

    return idx;
}

#if AT_UPGRADE_TWO==AT_UPGRADE
static int
get_upgrade_slave(int master)
{
    int skips = __skips(master);
    int idx;

    idx = at_find_first_good_buddy(rootfs, master, skips);
    if (__at_rootfs_is_good(idx)) {
        debug_rootfs_upgrade(idx, "slave", "good");
        
        return idx;
    }

    idx = at_find_first_bad_buddy(rootfs, master, skips);
    if (is_good_atidx(idx)) {
        debug_rootfs_upgrade(idx, "slave", "bad");
        
        return idx;
    }

    idx = at_find_worst(rootfs, skips);
    if (is_good_atidx(idx)) {
        debug_rootfs_upgrade(idx, "slave", "worst");
        
        return idx;
    }

    trace_assert(0, "no found worst slave");
    idx = at_first_idx(atsys.current, skips);
    debug_rootfs_upgrade(idx, "slave", "force");
    
    return idx;
}
#endif

static int
repair_kernel(int idx)
{
    at_version_t version;
    at_version_t *fversion = get_kernel_version(idx);

    os_objcpy(&version, at_kernel_version(idx));

    if (NULL==fversion) {
        debug_bug("cannot get rootfs%d's kernel version", idx);

        return -EBUG;
    }
    else if (__at_kernel_is_good(idx)
        && at_version_eq(fversion, &version)) {

        debug_ok("kernel%d needn't repair", idx);

        return 0;
    }
    else {
        return kdd_byfile(idx, rootfs_file(idx, __FILE_KERNEL), &version);
    }
}

static int
repair_rootfs(int idx)
{
    at_version_t version;
    at_version_t *fversion = get_rootfs_version(idx);

    os_objcpy(&version, at_rootfs_version(idx));
    
    int err = 0, buddy;

    if (NULL==fversion) {
        debug_bug("cannot get rootfs%d's rootfs version", idx);

        return -EBUG;
    }
    else if (__at_rootfs_is_good(idx)
        && at_version_eq(fversion, &version)) {

        debug_ok("rootfs%d needn't repair", idx);

        return 0;
    }

    buddy = at_find_first_good_byversion(rootfs, &version, __skips(idx));
    if (__at_rootfs_is_good(buddy)) {
        err = rcopy(idx, dir_rootfs(buddy), &version);
    } else {
        err = rsync(idx, &version);
    }

    return err;
}

static int
upgrade(int idx, int master)
{
    int err = 0;

    if (NULL==atsys.env.force
        && __at_rootfs_is_good(idx)
        && __at_kernel_is_good(idx)
        && at_version_eq(&atsys.version, at_rootfs_version(idx))
        && at_version_eq(&atsys.version, at_kernel_version(idx))) {

        jinfo("%d%o",
            "rootfs", idx,
            "upgrade", jobj_oprintf("%s", 
                            "info", "needn't upgrade: "
                                    "not at force mode and "
                                    "target rootfs version==upgrade version"));

        os_println("needn't upgrade: not at force mode, "
            "target rootfs%d version==upgrade version(%s)",
            idx, atsys.env.version);

        return 0;
    }

    upgrade_init(idx, &atsys.version);

    err = rsync(idx, &atsys.version);
    if (err) {
        return err;
    }

    err = repair_kernel(idx);
    if (err) {
        return err;
    }
    
#if AT_UPGRADE_TWO==AT_UPGRADE
    if (NULL==atsys.env.rootfs) {
        if (idx!=master) {
            return 0;
        }
        
        err = upgrade(get_upgrade_slave(idx), master);
    }
#endif

    switch_to(idx);

    return err;
}

static int
usbupgrade(void)
{
    char kernel_version[1+OS_LINE_LEN] = {0};
    char rootfs_version[1+OS_LINE_LEN] = {0};
    at_version_t version;
    int i, err, begin, end; 
    bool kernel_exist = false;
    
    if (0==atsys.current) {
        return -ENOSUPPORT;
    }
    
    if (access(USB_FILE_KERNEL_VERSION, 0) || 
        access(USB_FILE_ROOTFS_VERSION, 0)) {
        debug_trace("no found version file, needn't usbupgrade");

        return 0;
    }
    
    err = os_sfgets(kernel_version, OS_LINE_LEN, USB_FILE_KERNEL_VERSION);
    if (err) {
        debug_error("get %s error:%d", USB_FILE_KERNEL_VERSION, err);
        
        return err;
    }
    
    err = os_sfgets(rootfs_version, OS_LINE_LEN, USB_FILE_ROOTFS_VERSION);
    if (err) {
        debug_error("get %s error:%d", USB_FILE_ROOTFS_VERSION, err);
        
        return err;
    }
    
    err = __at_version_atoi(&version, rootfs_version);
    if (err) {
        debug_error("bad rootfs version:%s", rootfs_version);
        
        return err;
    }
    
    if (at_version_eq(at_rootfs_version(0), &version)) {
        debug_trace("usb(%s)==rootfs0, needn't ubsupgrade", rootfs_version);
        
        return 0;
    }
    
    if (0==access(USB_FILE_BOOT, 0)) {
        bdd("boot", DEV_BOOT, USB_FILE_BOOT);
    }

    if (0==access(USB_FILE_BOOTENV, 0)) {
        bdd("bootenv", DEV_BOOTENV, USB_FILE_BOOTENV);
    }

    if (atsys.env.rootfs) {
        begin   = os_atoi(atsys.env.rootfs);
        end     = begin + 1;
    } else {
        begin   = 0; 
        end     = AT_OS_COUNT;
    }
    
    if (0==access(USB_FILE_KERNEL, 0)) {
        kernel_exist = true;
    }
    
    for (i=begin; i<end; i++) {
        if (i!=atsys.current) {
            if (kernel_exist) {
                __at_version_atoi(&version, kernel_version);
                err = kdd_byfile(i, USB_FILE_KERNEL, &version);
                if (err) {
                    return err;
                }
            }
            
            __at_version_atoi(&version, rootfs_version);
            err = rcopy(i, DIR_USB_ROOTFS, &version);
            if (err) {
                return err;
            }
        }
    }

    return 0;
}

static int
toolupgrade(void)
{
    if (0==atsys.current) {
        return -ENOSUPPORT;
    }
    
    return 0;
}

static int
super_startup(void)
{
    int skips = os_bit(0);
    int best, idx;
    
    os_println("super startup");

    best = at_find_best(rootfs, skips);
    
    if (__at_rootfs_is_bad(best)) {
        best = 1;
        debug_error("no found good rootfs, force use rootfs1 and use rootfs0 repair rootfs1");

        rdd(1, 0);
    }
    idx = best;

    if (__at_kernel_is_good(idx)) {
        debug_error("firmware%d is good, why in super ???", idx);

        goto ok;
    }

    best = at_find_best(kernel, skips);
    if (__at_kernel_is_bad(best)) {
        debug_error("no found good kernel, use kernel0 repair kernel%d", idx);
        best = 0;
    }
    kdd_bydev(idx, best);

ok:
    switch_to(idx);
    
    return os_p_system("sysreboot &");
}

#define __update_other_ok(_obj, _idx, _skips)  do{  \
    int i;                                          \
                                                    \
    at_foreach(i) {                                 \
        if (false==is_at_skip(i, _skips)            \
            && at_obj_is_good(_obj, i)              \
            && at_obj_version_eq(_obj, i, _idx)) {  \
            debug_ok("tagged " #_obj "%d's other is ok", i); \
            at_obj(_obj, i)->other = AT_FSM_OK;     \
        }                                           \
    }                                               \
}while(0)

#define __normal_startup(_obj)                  do{ \
    int current = atsys.current;                    \
                                                    \
    if (AT_FSM_UNKNOW==at_obj(_obj, current)->self) { \
        int skips = __at_skips(current);            \
        __update_other_ok(_obj, current, skips);    \
    }                                               \
                                                    \
    at_obj(_obj, current)->error    = 0;            \
    at_obj(_obj, current)->self     = AT_FSM_OK;    \
                                                    \
    atsys.dirty = true;                             \
}while(0)

static int
normal_startup(void)
{
    os_println("normal startup");
    
    __normal_startup(rootfs);
    __normal_startup(kernel);

    save();
    
    return 0;
}

static int
init_env(void)
{
    char *env;
    
    env = get_string_env(ENV_TIMEOUT, oem_rsync_timeout);
    if (env) {
        if (0==os_atoi(env)) {
            debug_error("bad timeout:%s", env);
            
            return -EFORMAT;
        }
        
        atsys.env.timeout = env;
    }
    
    env = get_string_env(ENV_PWDFILE, NULL);
    if (env) {
        atsys.env.pwdfile = env;
    }

    env = get_string_env(ENV_VERSION, NULL);
    if (env) {
        atsys.env.version = env;
        
        if (__at_version_atoi(&atsys.version, env)) {
            debug_error("bad version:%s", env);
            
            return -EFORMAT;
        }
    }
    
    env = get_string_env(ENV_ROOTFS, NULL);
    if (env) {
        int idx = os_atoi(env);
        
        if (idx>=0 && false==is_normal_atidx(idx)) {
            debug_error("bad rootfs idx:%s", env);
            
            return -EFORMAT;
        }
        else if (idx==atsys.current) {
            debug_error("can not upgrade current rootfs%s", env);
            
            return -EFORMAT;
        }
        
        atsys.env.rootfs = env;
    }
    
    env = get_string_env(ENV_FORCE, NULL);
    if (env) {
        atsys.env.force = env;
    }
    
    env = get_string_env(ENV_SERVER, oem_rsync_server);
    if (env) {
        atsys.env.server = env;
    }

    env = get_string_env(ENV_PORT, oem_rsync_port);
    if (env) {
        if (0==os_atoi(env)) {
            debug_error("bad port:%s", env);
            
            return -EFORMAT;
        }
        
        atsys.env.port = env;
    }

    env = get_string_env(ENV_USER, oem_rsync_user);
    if (env) {
        atsys.env.user = env;
    }

    env = get_string_env(ENV_PATH, oem_rsync_path);
    if (env) {
        atsys.env.path = env;
    }

    return 0;
}

static int
get_current(void)
{
    int i, err;
    char line[1+OS_LINE_LEN] = {0};
    char *dev, *p;
    
    err = os_spgets(line, OS_LINE_LEN, "cat " AT_PROC_CMDLINE);
    if (err) {
        debug_error("read " AT_PROC_CMDLINE " error:%d", err);
        
        goto error;
    }

    dev = os_strstr(line, "root=");
    if (NULL==dev) {
        debug_error("no found rootfs in " AT_PROC_CMDLINE);
        
        goto error;
    }
    dev++;

    p = os_strchr(dev, ' ');
    if (p) {
        *p = 0;
    }
    
    __at_foreach(i, 0) {
        if (0==os_strcmp(dev, dev_rootfs(i))) {
            return i;
        }
    }

error:
    error_assert(0, "no found good rootfs in " AT_PROC_CMDLINE);
    
#ifdef __PC__
    return __at_current;
#else
    return AT_OS_CURRENT;
#endif
}

static int
init_current(void)
{
    int i, err, old = atsys.current;

    err = os_spgeti(&atsys.current, SCRIPT_CURRENT);
        debug_trace_error(err, SCRIPT_CURRENT);
    if (err) {
        atsys.current = get_current();
    }

    debug_trace("current %d==>%d", old, atsys.current);

    return 0;
}

static int
__fini(void)
{
    os_file_unlock();

    os_fini();

    return 0;
}

static int
__init(void)
{
    int err;
    
    os_setup_signal_exit(NULL);
    os_setup_signal_callstack(NULL);

    err = os_init();
    if (err) {
        return err;
    }

    err = init_current();
    if (err) {
        return err;
    }

    err = init_env();
    if (err) {
        return err;
    }

    err = os_file_lock(false);
    if (err) {
        return err;
    }

    err = load();
    if (err) {
        return err;
    }

    return 0;
}

static int
usage(char *self)
{
#if USE_USAGE
    os_fprintln(stderr, "%s usb",       self);
    os_fprintln(stderr, "%s repair",    self);
    os_fprintln(stderr, "%s upgrade",   self);
    os_fprintln(stderr, "%s startup",   self);
#endif

    return -EFORMAT;
}

static int
cmd_repair(int argc, char *argv[])
{
    int skips = __skips(0);
    int i, err;
    
    if (0==atsys.current) {
        return -ENOSUPPORT;
    }
    
    at_foreach(i) {
        if (false==is_at_skip(i, skips) && 0==repair_rootfs(i)) {
            repair_kernel(i);
        }
	}
    
	return 0;
}


static int
cmd_upgrade(int argc, char *argv[])
{
    int i, idx, err;
    
    if (0==atsys.current) {
        jinfo("%o",
        "upgrade", jobj_oprintf("%s", 
                        "info", "rootfs0 not support upgrade"));

        return -ENOSUPPORT;
    }
    else if (NULL==atsys.env.version) {
        jinfo("%o",
        "upgrade", jobj_oprintf("%s", 
                        "info", "not input upgrade version"));

        return -EFORMAT;
    }

    if (atsys.env.rootfs) {
        idx = os_atoi(atsys.env.rootfs);

        debug_trace("upgrade rootfs%d by env select", idx);
    } else {
        idx = get_upgrade_master();

        debug_trace("upgrade rootfs%d by auto select", idx);
    }

    if (idx<0) {
        for (i=1; i<AT_OS_COUNT; i++) {
            if (i!=atsys.current) {
                err = upgrade(i, i);
                if (err) {
                    return err;
                }
            }
        }

        return 0;
    } else {
        return upgrade(idx, idx);
    }
}

static int
cmd_startup(int argc, char *argv[])
{
    if (0==atsys.current) {
        return super_startup();
    } else {
        return normal_startup();
    }
}

static int
cmd_usbupgrade(int argc, char *argv[])
{
    return usbupgrade();
}

static int
cmd_toolupgrade(int argc, char *argv[])
{
    return toolupgrade();
}

static int
cmd_mount(int argc, char *argv[])
{
    int i, err, errs = 0;
        
    for (i=0; i<os_count_of(mounts); i++) {
        err = (*mounts[i].mount)();
        if (err) {
            errs = err;
        }
    }

    return errs;
}

static int
cmd_umount(int argc, char *argv[])
{
    int i, err, errs = 0;
    
    for (i=os_count_of(mounts) - 1; i>=0; i--) {
        err = (*mounts[i].umount)();
        if (err) {
            errs = err;
        }
    }
    
    return errs;
}

static simpile_cmd_t cmd[] = {
    {
        .argc   = 1,
        .argv   = {"mount"},
        .handle = cmd_mount,
    },
    {
        .argc   = 1,
        .argv   = {"umount"},
        .handle = cmd_umount,
    },
    {
        .argc   = 1,
        .argv   = {"startup"},
        .handle = cmd_startup,
    },
    {
        .argc   = 1,
        .argv   = {"usbupgrade"},
        .handle = cmd_usbupgrade,
    },
    {
        .argc   = 1,
        .argv   = {"toolupgrade"},
        .handle = cmd_toolupgrade,
    },
    {
        .argc   = 1,
        .argv   = {"upgrade"},
        .handle = cmd_upgrade,
    },
    {
        .argc   = 1,
        .argv   = {"repair"},
        .handle = cmd_repair,
    },
};

#ifndef __BUSYBOX__
#define atsys_main  main
#endif

static int
__main(int argc, char *argv[])
{
    return simpile_cmd_handle(cmd, argc, argv, usage);
}

int atsys_main(int argc, char *argv[])
{
    return os_call(__init, __fini, __main, argc, argv);
}

#ifdef __BUSYBOX__
int sysmount_main(int argc, char *argv[])
{
    return os_call(__init, __fini, cmd_mount, argc, argv);
}

int sysrepair_main(int argc, char *argv[])
{
    return os_call(__init, __fini, cmd_repair, argc, argv);
}

int sysstartup_main(int argc, char *argv[])
{
    return os_call(__init, __fini, cmd_startup, argc, argv);
}

int sysumount_main(int argc, char *argv[])
{
    return os_call(__init, __fini, cmd_umount, argc, argv);
}

int sysupgrade_main(int argc, char *argv[])
{
    return os_call(__init, __fini, cmd_upgrade, argc, argv);
}

int sysusbupgrade_main(int argc, char *argv[])
{
    return os_call(__init, __fini, cmd_usbupgrade, argc, argv);
}

int systoolupgrade_main(int argc, char *argv[])
{
    return os_call(__init, __fini, cmd_toolupgrade, argc, argv);
}
#endif
/******************************************************************************/
