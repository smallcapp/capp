#ifndef __IP_H_46F65504978D5AD68BA98D390A65EB49__
#define __IP_H_46F65504978D5AD68BA98D390A65EB49__
/******************************************************************************/
#ifndef OS_IPSTRINGLEN
#define OS_IPSTRINGLEN              15
#endif

#ifdef __APP__

static inline char *
os_ipstring(uint32_t ip)
{
    return (char *)inet_ntoa(*(struct in_addr *)&ip);
}

static inline bool
is_good_ipstring(char *ip)
{
    return is_good_string(ip) && INADDR_NONE!=inet_addr(ip);
}
#endif

static inline bool
os_ipmatch(uint32_t ipa, uint32_t ipb, uint32_t mask)
{
    return (ipa & mask)==(ipb & mask);
}
/******************************************************************************/
#endif /* __IP_H_46F65504978D5AD68BA98D390A65EB49__ */
