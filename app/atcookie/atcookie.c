#ifndef __THIS_APP
#define __THIS_APP      atcookie
#endif

#include "utils.h"
#include "atcookie/atcookie.h"

OS_INITER;

#ifndef __BUSYBOX__
#define atcookie_main  main
#endif

static int
__main(int argc, char *argv[])
{
    int type;

    if (argc<3) {
        return EFORMAT;
    }

    type = os_atoi(argv[1]);

    switch(type) {
        case AT_COOKIE_FILE: {
            int id = os_atoi(argv[2]);
            char *file = atcookie_file(id);
            if (NULL==file) {
                return ENOEXIST;
            }

            os_println("%s", file);
        }   break;
        case AT_COOKIE_DOMAIN: {
            char *domain = argv[2];
            
        }   break;
        default:
            return EFORMAT;
    }

    return 0;
}

int atcookie_main(int argc, char *argv[])
{
    os_setup_signal_exit(NULL);
    os_setup_signal_callstack(NULL);
    
    return os_main(__main, argc, argv);
}
/******************************************************************************/
