#ifndef __MAC_H_F836BDB5B61CB538446562E3E6DD918C__
#define __MAC_H_F836BDB5B61CB538446562E3E6DD918C__
/******************************************************************************/
#include "number.h"

enum {
    OS_MACSIZE      = 6,
    OS_OUISIZE      = 3,

    MACSTRINGLEN_L  = (3*OS_MACSIZE-1),
    MACSTRINGLEN_S  = (2*OS_MACSIZE),
};

#define OS_ZEROMAC_STRING   "00:00:00:00:00:00"
#define OS_FULLMAC_STRING   "ff:ff:ff:ff:ff:ff"
#define OS_2BMAC_STRING     "2b:2b:2b:2b:2b:2b"
#define OS_TEMPMAC_STRING   "00:00:01:00:00:01"

#define OS_ZEROMAC  ((unsigned char *)"\x00\x00\x00\x00\x00\x00")
#define OS_FULLMAC  ((unsigned char *)"\xff\xff\xff\xff\xff\xff")
#define OS_2BMAC    ((unsigned char *)"\x2b\x2b\x2b\x2b\x2b\x2b")
#define OS_TEMPMAC  ((unsigned char *)"\x00\x00\x01\x00\x00\x01")

static inline unsigned char *
os_maccpy(unsigned char dst[], unsigned char src[])
{
    *(uint32_t *)(dst+0) = *(uint32_t *)(src+0);
    *(uint16_t *)(dst+4) = *(uint16_t *)(src+4);

    return dst;
}

static inline unsigned char *
os_maczero(unsigned char mac[])
{
    *(uint32_t *)(mac+0) = 0;
    *(uint16_t *)(mac+4) = 0;
    
    return mac;
}

static inline unsigned char *
os_macfull(unsigned char mac[])
{
    *(uint32_t *)(mac+0) = 0xffffffff;
    *(uint16_t *)(mac+4) = 0xffff;
    
    return mac;
}

static inline int
os_maccmp(unsigned char a[], unsigned char b[])
{
    return os_memcmp(a, b, OS_MACSIZE);
}

static inline bool
os_maceq(unsigned char a[], unsigned char b[])
{
    return *(uint32_t *)(a+0) == *(uint32_t *)(b+0)
        && *(uint16_t *)(a+4) == *(uint16_t *)(b+4);
}

static inline bool
os_macmaskmach(unsigned char a[], unsigned char b[], unsigned char mask[])
{
    return os_bufmaskmatch(a, b, mask, OS_MACSIZE);
}

static inline bool
is_zero_mac(unsigned char mac[])
{
    return *(uint32_t *)(mac+0) == 0
        && *(uint16_t *)(mac+4) == 0;
}

static inline bool
is_full_mac(unsigned char mac[])
{
    return *(uint32_t *)(mac+0) == 0xffffffff
        && *(uint16_t *)(mac+4) == 0xffff;
}

static inline bool
is_good_mac(unsigned char mac[])
{
    return false==is_zero_mac(mac) 
        && false==is_full_mac(mac);
}

static inline bool
__is_good_macchar(int ch)
{
    return (ch >= '0' && ch <= '9')
        || (ch >= 'a' && ch <= 'f')
        || (ch >= 'A' && ch <= 'F');
}

static inline bool
__is_good_macstring(char *macstring)
{
    int len = os_strlen(macstring);
    int ifs;
    
    switch(len) {
        case MACSTRINGLEN_S:
            return __is_good_macchar(macstring[0])
                && __is_good_macchar(macstring[1])
                && __is_good_macchar(macstring[2])
                && __is_good_macchar(macstring[3])
                && __is_good_macchar(macstring[4])
                && __is_good_macchar(macstring[5])
                && __is_good_macchar(macstring[6])
                && __is_good_macchar(macstring[7])
                && __is_good_macchar(macstring[8])
                && __is_good_macchar(macstring[9])
                && __is_good_macchar(macstring[10])
                && __is_good_macchar(macstring[11]);
        case MACSTRINGLEN_L:
            ifs = macstring[2];
            return (':'==ifs || '-'==ifs)
                && macstring[5] ==ifs
                && macstring[8] ==ifs
                && macstring[11]==ifs
                && macstring[14]==ifs
                && __is_good_macchar(macstring[0])
                && __is_good_macchar(macstring[1])
                && __is_good_macchar(macstring[3])
                && __is_good_macchar(macstring[4])
                && __is_good_macchar(macstring[6])
                && __is_good_macchar(macstring[7])
                && __is_good_macchar(macstring[9])
                && __is_good_macchar(macstring[10])
                && __is_good_macchar(macstring[12])
                && __is_good_macchar(macstring[13])
                && __is_good_macchar(macstring[15])
                && __is_good_macchar(macstring[16]);
        default:
            return false;
    }
}

static inline unsigned char *
__os_getmac_bystring(unsigned char mac[], char macstring[])
{
    int len = os_strlen(macstring);
    int width = 3;
    int i;

    if (MACSTRINGLEN_L==len) {
        width = 3;
    }
    else if (MACSTRINGLEN_S==len) {
        width = 2;
    }
    else {
        return OS_2BMAC;
    }
    
    for (i=0; i<OS_MACSIZE; i++) {
        mac[i] = os_digitstring2number(macstring + width*i, 2, 16, int);
    }

    return mac;
}

static inline unsigned char *
os_getmac_bystring(unsigned char mac[], char macstring[])
{
    unsigned char tmp[6];
    
    if (__is_good_macstring(macstring) && 
        __os_getmac_bystring(tmp, macstring) &&
        is_good_mac(tmp)) {
        return os_maccpy(mac, tmp);
    } else {
        return NULL;
    }
}

static inline bool
is_good_macstring(char *macstring)
{
    unsigned char mac[6];
    
    return NULL!=os_getmac_bystring(mac, macstring);
}

static inline unsigned char *
os_mac(char *macstring)
{
    static unsigned char mac[OS_MACSIZE] = {0};
    
    return __is_good_macstring(macstring)?__os_getmac_bystring(mac, macstring):OS_ZEROMAC;
}

static inline int
os_macsnprintf(unsigned char mac[], char macstring[], int len, int sep)
{
    if (0==sep) {
        return os_snprintf(macstring, len,
            "%.2x"
            "%.2x"
            "%.2x"
            "%.2x"
            "%.2x"
            "%.2x",
            mac[0], 
            mac[1], 
            mac[2], 
            mac[3], 
            mac[4], 
            mac[5]);
    } else {
        return os_snprintf(macstring, len,
            "%.2x"  "%c"
            "%.2x"  "%c"
            "%.2x"  "%c"
            "%.2x"  "%c"
            "%.2x"  "%c"
            "%.2x",
            mac[0], sep,
            mac[1], sep,
            mac[2], sep,
            mac[3], sep,
            mac[4], sep,
            mac[5]);
    }
}

#define os_macsaprintf(_mac, _macstring, _sep) \
        os_macsnprintf(_mac, _macstring, sizeof(_macstring), _sep)

static inline char *
os_getmacstring(unsigned char mac[], int sep)
{
    static char macstring[1+MACSTRINGLEN_L] = {0};

    os_macsaprintf(mac, macstring, sep);

    return macstring;
}

static inline char *
os_macstring(unsigned char mac[])
{
    return os_getmacstring(mac, ':');
}

#ifndef ENV_BASEMAC
#define ENV_BASEMAC         "__BASEMAC__"
#endif
/******************************************************************************/
#endif /* __MAC_H_F836BDB5B61CB538446562E3E6DD918C__ */
