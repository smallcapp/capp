#ifndef __DUMP_H_202B4E0B91FFA0621EF08DACFCA3681E__
#define __DUMP_H_202B4E0B91FFA0621EF08DACFCA3681E__
/******************************************************************************/
#include "utils/printf.h"

#ifndef __RAW_LINE_BLOCK
#define __RAW_LINE_BLOCK        4
#endif

#ifndef __RAW_LINE_BLOCK_BYTES
#define __RAW_LINE_BLOCK_BYTES  4
#endif

#ifndef __RAW_LINE_LIMIT
#define __RAW_LINE_LIMIT        80
#endif

#define __RAW_LINE_BYTES        (__RAW_LINE_BLOCK_BYTES * __RAW_LINE_BLOCK)
#define __RAW_LINE_MAX       (0 \
        + 8 /* "xxxxH : " */    \
        + (2 * __RAW_LINE_BLOCK_BYTES + 1) * __RAW_LINE_BLOCK \
        + 2 /* "; " */          \
        + __RAW_LINE_BYTES      \
        + 1 /* "\n" */          \
)

#if __RAW_LINE_MAX > __RAW_LINE_LIMIT
#error "must __RAW_LINE_MAX < __RAW_LINE_LIMIT"
#endif

#ifndef __RAW_LINE_SEPARATOR
#define __RAW_LINE_SEPARATOR \
"=============================================================="
#endif

#ifndef __RAW_LINE_SEPARATOR_SUB
#define __RAW_LINE_SEPARATOR_SUB \
"--------------------------------------------------------------"
#endif

#ifndef __RAW_LINE_HEADER
#define __RAW_LINE_HEADER \
"      :                                     ;"                 __crlf \
" Line :       Hexadecimal Content           ; Raw Content"     __crlf \
"      : 0 1 2 3  4 5 6 7  8 9 A B  C D E F  ;"                 __crlf \
"      :                                     ;"                 __crlf
#endif

typedef void dump_line_f(char *line);

#ifdef OS_DUMP_PRINTF
#define os_dump_printf(_fmt, _args...)  OS_DUMP_PRINTF(_fmt, ##_args)
#else
#define os_dump_printf(_fmt, _args...)  os_printf(_fmt, ##_args)
#endif

static inline void
os_dump_line(int line, unsigned char *raw, int len, dump_line_f *dump_line)
{
    int i, offset = 0;
    char buf[1 + __RAW_LINE_MAX] = {0};

    offset += os_soprintf(buf, offset, "%.4XH :", __RAW_LINE_BYTES*line);

    for (i=0; i<len; i++) {
        if (0 == (i%__RAW_LINE_BLOCK_BYTES)) {
            offset += os_soprintf(buf, offset, __space);
        }
        
        offset += os_soprintf(buf, offset, "%.2X", raw[i]);
    }
    
    for (; i<__RAW_LINE_BYTES; i++) {
        if (0 == (i%__RAW_LINE_BLOCK_BYTES)) {
            offset += os_soprintf(buf, offset, __space);
        }
        
        offset += os_soprintf(buf, offset, __space __space);
    }
    offset += os_soprintf(buf, offset, " ; ");
    
    for (i=0; i<len; i++) {
        int c = (int)raw[i];
        
        offset += os_soprintf(buf, offset, "%c", os_isprint(c)?c:'.');
    }
    offset += os_soprintf(buf, offset, __crlf);

    if (dump_line) {
        (*dump_line)(buf);
    } else {
        os_dump_printf("%s", buf);
    }
}

static inline void
__os_dump_buffer(void *buffer, int len, dump_line_f *dump_line)
{
    int i, line, tail;
    unsigned char *raw = (unsigned char *)buffer;
    
    line = __os_align(len, __RAW_LINE_BYTES)/__RAW_LINE_BYTES;
    tail = len%__RAW_LINE_BYTES;
    tail = tail?tail:__RAW_LINE_BYTES;
    
    if (dump_line) {
        (*dump_line)(__RAW_LINE_HEADER);
    } else {
        os_dump_printf(__RAW_LINE_HEADER);
    }
    
    for (i=0; i<(line-1); i++) {
        os_dump_line(i, raw + i * __RAW_LINE_BYTES, __RAW_LINE_BYTES, dump_line);
    }
    os_dump_line(line, raw + i * __RAW_LINE_BYTES, tail, dump_line);
}

#define os_dump_buffer(_is_dump, _buffer, _len, _dump_line) do{ \
    if (_is_dump) {                                             \
        __os_dump_buffer(_buffer, _len, _dump_line);            \
    }                                                           \
}while(0)
/******************************************************************************/
#endif /* __DUMP_H_202B4E0B91FFA0621EF08DACFCA3681E__ */
