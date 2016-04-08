#ifndef __LIB_UTILS_ASSERT_H__
#define __LIB_UTILS_ASSERT_H__

#if defined(__BOOT__) || defined(__APP__)
#define os_assert(_x)           assert(_x)
#else
#define os_assert(_x)           (void)0
#endif

#define os_assertV(_x)          (os_assert(0), _x)
/******************************************************************************/
#endif /* __LIB_UTILS_ASSERT_H__ */
