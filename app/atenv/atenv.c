#ifndef __THIS_APP
#define __THIS_APP      atenv
#endif

#include "atenv/atenv.h"
#include "atenv/bcookie.h"

#if 2!=AT_LTEFI_VERSION
#error "just support ltefi(v2)"
#endif

#ifdef __BOOT__
#     if AT_LTEFI_MD
#       include "atenv_boot_md.c"
#   elif AT_LTEFI_AP
#       include "atenv_boot_ap.c"
#   endif
#elif defined(__APP__)
#   include "atenv_app.c"
#endif
/******************************************************************************/
