#ifndef __LIB_UTILS_STRING_H__
#define __LIB_UTILS_STRING_H__
/******************************************************************************/
#ifndef __empty
#define __empty         ""
#endif

#ifndef __space
#define __space         " "
#endif

#ifndef __null
#define __null          "null"
#endif

#ifndef __nil
#define __nil           "nil"
#endif

#ifndef __space2
#define __space2        __space __space
#endif

#ifndef __space4
#define __space4        __space2 __space2
#endif

#ifndef __blank
#define __blank         " "
#endif

#ifndef __blanks
#define __blanks        "\t \r\n"
#endif

#ifndef __tab
#ifndef __TAB_AS_SPACE
#define __tab           "\t"
#elif 2==__TAB_AS_SPACE
#define __tab           __space2
#elif 4==__TAB_AS_SPACE
#define __tab           __space4
#endif
#endif

#ifndef __tab2
#define __tab2          __tab __tab
#endif

#ifndef __tab3
#define __tab3          __tab2 __tab
#endif

#ifndef __tab4
#define __tab4          __tab3 __tab
#endif

#ifndef __crlf
#define __crlf          "\n"
#endif

#ifndef __crlf2
#define __crlf2         __crlf __crlf
#endif

#ifndef __crlf3
#define __crlf3         __crlf2 __crlf
#endif

#ifndef __crlf4
#define __crlf4         __crlf3 __crlf
#endif

#ifndef __notes
#define __notes         "#"
#endif

#ifndef __unknow
#define __unknow        "unknow"
#endif

#ifndef __true
#define __true          "true"
#endif

#ifndef __false
#define __false         "false"
#endif

static inline bool
is_good_string(char *string)
{
    return string && string[0];
}

#ifndef string_array_item
#define string_array_item(_, idx)   [_(idx)] = #idx
#endif

#ifndef os_safestring
#define os_safestring(_s)           ((_s)?(_s):__nil)
#endif

#ifndef os_strlen
static inline int
os_strlen(const char *s)
{
    return s?strlen(s):0;
}
#endif

#ifndef os_strcpy
static inline char *
os_strcpy(char *dst, const char *src)
{
    return (dst && src)?strcpy(dst, src):dst;
}
#endif

#ifndef os_strmcpy
static inline char *
os_strmcpy(char *dst, const char *src, int len)
{
    if (dst && src) {
        os_memcpy(dst, src, len);

        dst[len] = 0;
    }

    return dst;
}
#endif

static inline uint32_t 
os_strlcpy(char *dst, const char *src, uint32_t size)
{
    char *d = (char *)dst;
    char *s = (char *)src;
    int n, len = 0;
    
    os_assert(NULL!=dst);
    os_assert(NULL!=src);

    if (size > 0) {
        n = size - 1;

        while(*s && n) {
            *d++ = *s++;
            n--;
        }

        len = size - 1 - n;
    }

    dst[len] = 0;
    
    return len;
}

#ifndef os_strdcpy
#define os_strdcpy(_dst, _src)      os_strlcpy(_dst, _src, sizeof(_dst))
#endif

#ifndef os_strscpy
#define os_strscpy(_dst, _src)      os_strlcpy(_dst, _src, sizeof(_src))
#endif

#ifndef os_strcmp
static inline int
os_strcmp(const char *a, const char *b)
{
    if (a) {
        if (b) {
            return strcmp(a, b);
        } else {
            return 1;
        }
    } else {
        if (b) {
            return -1;
        } else {
            return 0;
        }
    }
}
#endif

#ifndef os_strncmp
static inline int
os_strncmp(const char *a, const char *b, int len)
{
    if (a) {
        if (b) {
            return strncmp(a, b, len);
        } else {
            return 1;
        }
    } else {
        if (b) {
            return -1;
        } else {
            return 0;
        }
    }
}
#endif

#ifndef os_stracmp
#define os_stracmp(_a, _b)          os_strncmp(_a, _b, sizeof(_a))
#endif

#ifndef os_strbcmp
#define os_strbcmp(_a, _b)          os_strncmp(_a, _b, sizeof(_b))
#endif

#ifndef os_strtok
#define os_strtok(_s, _delim)       strtok(_s, _delim)
#endif

#ifndef os_strchr
#define os_strchr(_s, _ch)          strchr(_s, _ch)
#endif

#ifndef os_strrchr
#define os_strrchr(_s, _ch)         strrchr(_s, _ch)
#endif

#ifndef os_strstr
#define os_strstr(_s, _sub)         strstr(_s, _sub)
#endif

#ifndef os_strrstr
#define os_strrstr(_s, _sub)        strrstr(_s, _sub)
#endif

#ifndef os_strtok_foreach
#define os_strtok_foreach(_sub, _s, _delim) \
        for ((_sub)=os_strtok(_s, _delim);(_sub);(_sub)=os_strtok(NULL, _delim))
#endif

#ifdef __BOOT__
#ifndef os_strtol
#define os_strtol(_nptr, _endptr, _base)    simple_strtol(_nptr, _endptr, _base)
#endif

#ifndef os_strtoll
#define os_strtoll(_nptr, _endptr, _base)   simple_strtoll(_nptr, _endptr, _base)
#endif

#ifndef os_strtoul
#define os_strtoul(_nptr, _endptr, _base)   simple_strtoul(_nptr, _endptr, _base)
#endif

#ifndef os_strtoull
#define os_strtoull(_nptr, _endptr, _base)  simple_strtoull(_nptr, _endptr, _base)
#endif
#else
#ifndef os_strtol
#define os_strtol(_nptr, _endptr, _base)    strtol(_nptr, _endptr, _base)
#endif

#ifndef os_strtoll
#define os_strtoll(_nptr, _endptr, _base)   strtoll(_nptr, _endptr, _base)
#endif

#ifndef os_strtoul
#define os_strtoul(_nptr, _endptr, _base)   strtoul(_nptr, _endptr, _base)
#endif

#ifndef os_strtoull
#define os_strtoull(_nptr, _endptr, _base)  strtoull(_nptr, _endptr, _base)
#endif
#endif

#ifndef os_strtonumber_is_good
#define os_strtonumber_is_good(_endptr)     (NULL==(_endptr) || 0==(_endptr)[0])
#endif

#ifndef __os_strlast
static inline char *
__os_strlast(char *s)
{
    return s?(s + os_strlen(s) - 1):NULL;
}
#endif

#ifndef os_strlast
static inline char *
os_strlast(char *s, int ch)
{
    char *last = __os_strlast(s);

    return (*last==ch)?last:NULL;
}
#endif

#ifndef os_getstringarrayidx
#define os_getstringarrayidx(_array, _string, _begin, _end) \
        __os_getobjarrayidx(_array, _string, os_strcmp, _begin, _end)
#endif

typedef bool char_is_something_f(int ch);

static inline bool 
__char_is(int ch, char_is_something_f *IS)
{
    if (IS) {
        return (*IS)(ch);
    } else {
#ifdef DEFT_CHAR_IS
        return DEFT_CHAR_IS(ch);
#else
        return NULL!=strchr(__blanks, ch);
#endif
    }
}

static inline char *
__string_skip(char *string, char_is_something_f *IS)
{
    char *p = string;

    while(*p && __char_is(*p, IS)) {
        p++;
    }

    return p;
}

static inline char *
__string_replace(char *string, char_is_something_f *IS_OLD, int new)
{
    char *s = string;

    while(*s) {
        if (__char_is(*s, IS_OLD)) {
            *s++ = new;
        } else {
            s++;
        }
    }

    return string;
}

static inline char *
__string_reduce(char *string, char_is_something_f *IS)
{
    char *p = string;
    char *s = string;
    bool in_reduce = false;
    
    while(*s) {
        if (__char_is(*s, IS)) {
            in_reduce = true;
            
            *p = *s++;

        } else {
            if (in_reduce) {
                p++;
                
                in_reduce = false;
            }

            *p++ = *s++;
        }
    }

    *p = 0;

    return string;
}

static inline char *
__string_strim(char *string, char_is_something_f *IS)
{
    char *p = string;
    char *s = string;

    while(*s) {
        if (__char_is(*s, IS)) {
            s++;
        } else {
            *p++ = *s++;
        }
    }

    return string;
}

static inline char *
__string_strim_left(char *string, char_is_something_f *IS)
{
    char *p = string;
    char *s = string;

    while(*s && __char_is(*s, IS)) {
        s++;
    }

    while(*s) {
        *p++ = *s++;
    }
    
    *p = 0;
    
    return string;
}

static inline char *
__string_strim_right(char *string, char_is_something_f *IS)
{
    char *p = string + os_strlen(string) - 1;

    while(p>=string && __char_is(*p, IS)) {
        p--;
    }

    *(p+1) = 0;
    
    return string;
}

static inline char *
__string_strim_both(char *string, char_is_something_f *IS)
{
    __string_strim_right(string, IS);

    return __string_strim_left(string, IS);
}

static inline bool
__char_is_drop(char_is_something_f *IS, int ch)
{
    if (IS) {
        return (*IS)(ch);
    } else {
#ifdef CHAR_IS_DROP
        return CHAR_IS_DROP(ch)
#else
        return ch=='#';
#endif
    }
}

static inline char *
__string_r_drop(char *string, char_is_something_f *IS)
{
    char *p = string;

    while(*p && false==__char_is_drop(IS, *p)) {
        p++;
    }

    *p = 0;
    
    return string;
}

static inline bool 
__is_blank_line(char *line)
{
    return 0==line[0] || '\n'==line[0] || ('\r'==line[0] && '\n'==line[1]);
}

static inline bool 
__is_notes_line(char *line, char *notes)
{
    int len = os_strlen(notes);

    return os_memeq(line, notes, len);
}

static inline bool 
__is_notes_line_deft(char *line)
{
    return __is_notes_line(line, __notes);
}

static inline char *
__string_next(char *string, char_is_something_f *IS)
{
    char *p = string;

    if (NULL==string) {
        return NULL;
    }
    
    while(*p && false==__char_is(*p, IS)) {
        p++;
    }
    
    if (0==*p) {
        return NULL;
    } else {
        *p++ = 0;

        return p;
    }
}

static inline char *
__string_next_byifs(char *string, int ifs)
{
    bool is_ifs(int ch)
    {
        return ch==ifs;
    }
    
    return __string_next(string, is_ifs);
}

#ifndef OS_STRING_BKDR_NUMBER
#define OS_STRING_BKDR_NUMBER   31
#endif

static inline uint32_t
__bkdr_push(uint32_t a, uint32_t b)
{
    return a * OS_STRING_BKDR_NUMBER + b;
}

static inline uint32_t
__bkdr_pop(uint32_t a, uint32_t b)
{
    return (a - b) / OS_STRING_BKDR_NUMBER;
}

static inline uint32_t
__string_BKDR(const char *string, uint32_t *plen)
{
    uint32_t hash = 0;
    
    if (string) {
        const char *p = string;
        uint32_t len = 0;
        
        while(*p) {
            len++;
            hash = __bkdr_push(hash, *p++);
        }

        if (plen) {
            *plen = len;
        }
    }
    
    return hash;
}

static inline uint32_t
__string_bkdr(const char *string)
{
    uint32_t hash = 0;

    if (string) {
        const char *p = string;

        while(*p) {
            hash = __bkdr_push(hash, *p++);
        }
    }
    
    return hash;
}

static inline uint32_t
__binary_bkdr(const unsigned char *binary, uint32_t len)
{
    uint32_t hash = 0;

    if (binary) {
        int i;
        
        for (i=0; i<len; i++) {
            hash = __bkdr_push(hash, *(binary + i));
        }
    }
    
    return hash;
}

typedef struct {
    uint32_t sym:1;
    uint32_t bin:1;
    uint32_t len:30;
    uint32_t hash;
    char string[0];
} string_t;

static inline string_t *
__string_new(
    uint32_t resv, 
    const void *binary, 
    bool bin, 
    uint32_t len, 
    uint32_t hash
)
{
    if (NULL==binary) {
        return os_assertV(NULL);
    }

    unsigned char *p = (unsigned char *)os_zalloc(resv + 1 + len + sizeof(string_t));
    if (NULL==p) {
        return os_assertV(NULL);
    }
    
    string_t *s = (string_t *)(p + resv);
    os_memcpy(s->string, binary, len);
    s->string[len] = 0;
    s->len  = len;
    s->hash = hash;
    s->bin  = bin;
    
    return s;
}

static inline string_t *
string_new(uint32_t resv, const char *string)
{
    uint32_t len = 0;
    uint32_t hash = __string_BKDR(string, &len);

    return __string_new(resv, string, false, len, hash);
}

#define string_NEW(_type, _member, _string) ({  \
    _type *p = NULL;                            \
    string_t *s = string_new(offsetof(_type, _member), _string); \
    if (s) {                                    \
        p = container_of(s, _type, _member);    \
    }                                           \
                                                \
    p;                                          \
})

static inline string_t *
binary_new(uint32_t resv, const void *binary, uint32_t len)
{
    return __string_new(resv, binary, true, len, __binary_bkdr(binary, len));
}

#define binary_NEW(_type, _member, _binary, _len) ({  \
    _type *p = NULL;                            \
    string_t *s = binary_new(offsetof(_type, _member), _binary, _len); \
    if (s) {                                    \
        p = container_of(s, _type, _member);    \
    }                                           \
                                                \
    p;                                          \
})

static inline bool
__string_eq(
    const string_t *s, 
    const void *binary, 
    bool sym, 
    bool bin, 
    uint32_t len, 
    uint32_t hash
)
{
    return s->sym==sym
        && s->bin==bin
        && s->len==len
        && s->hash==hash
        && 0==os_memcmp(s->string, binary, len);
}

static inline int
__string_cmp(
    const string_t *s, 
    const void *binary, 
    bool sym, 
    bool bin, 
    uint32_t len
)
{
    if (s->sym != sym) {
        return (int)s->sym - (int)sym;
    }
    else if (s->bin != bin) {
        return (int)s->bin - (int)bin;
    }
    else if (s->len != len) {
        int ret = os_memcmp(s->string, binary, os_min((uint32_t)s->len, len));
        if (ret) {
            return ret;
        } else {
            return (int)s->len - (int)len;
        }
    }
    else {
        return os_memcmp(s->string, binary, len);
    }
}

static inline bool
string_eq(const string_t *a, const string_t *b)
{
    if (NULL==a) {
        return os_assertV(false);
    }
    else if (NULL==b) {
        return os_assertV(false);
    }
    else {
        return __string_eq(a, b->string, b->sym, b->bin, b->len, b->hash);
    }
}

static inline int
string_cmp(const string_t *a, const string_t *b)
{
    if (NULL==a && NULL==b) {
        return os_assertV(0);
    }
    else if (NULL==a) {
        return os_assertV(-1);
    }
    else if (NULL==b) {
        return os_assertV(1);
    }
    else {
        return __string_cmp(a, b->string, b->sym, b->bin, b->len);
    }
}

static inline bool
string_is_symbol(const string_t *s)
{
    return s?s->sym:false;
}

static inline bool
string_is_binary(const string_t *s)
{
    return s?s->bin:false;
}

typedef struct {
    struct mlist_node node;
    string_t s;
} symbol_t;

typedef struct mlist_table symbol_table_t;

static inline int
sysbol_table_init(symbol_table_t *table, uint32_t size)
{
    return mlist_table_init(table, size);
}

static inline symbol_t *
__symbol_init(symbol_t *sym)
{
    INIT_LIST_HEAD(&sym->node.list);
    sym->s.sym = true;
    
    return sym;
}

static inline symbol_t *
__symbol_string_new(const char *string)
{
    symbol_t *sym = string_NEW(symbol_t, s, string);

    return sym?__symbol_init(sym):NULL;
}

static inline symbol_t *
__symbol_binary_new(const void *binary, uint32_t len)
{
    symbol_t *sym = binary_NEW(symbol_t, s, binary, len);

    return sym?__symbol_init(sym):NULL;
}

static inline symbol_t *
__symbol_entry(const struct mlist_node *node)
{
    return container_of(node, symbol_t, node);
}

static inline bool
__symbol_eq(
    const symbol_t *sym, 
    const void *binary, 
    bool bin, 
    uint32_t len, 
    uint32_t hash
)
{
    return __string_eq(&sym->s, binary, true, bin, len, hash);
}

static inline int
__symbol_cmp(
    const symbol_t *sym, 
    const void *binary, 
    bool bin, 
    uint32_t len
)
{
    return __string_cmp(&sym->s, binary, true, bin, len);
}

static inline symbol_t *
__symbol_insert(
    struct mlist_table *table, 
    int (*data_hash)(void),
    int (*node_hash)(struct mlist_node *node),
    bool (*eq)(struct mlist_node *node),
    struct mlist_node *(*new)(void)
)
{
    struct mlist_node *node = mlist_insert(table, data_hash, node_hash, eq, new);
    if (NULL==node) {
        return NULL;
    }

    symbol_t *sym = __symbol_entry(node);

    return sym;
}

static inline symbol_t *
__symbol_push_string(symbol_table_t *table, const char *string)
{
    uint32_t len = 0;
    uint32_t hash = __string_BKDR(string, &len);

    return __symbol_insert(table, 
        lanmbda(int, (void) {
            return hash & (table->size - 1);
        }), 
        lanmbda(int, (struct mlist_node *node) {
            symbol_t *sym = __symbol_entry(node);
            
            return sym->s.hash & (table->size - 1);
        }), 
        lanmbda(bool, (struct mlist_node *node) {
            return __symbol_eq(__symbol_entry(node), string, false, len, hash);
        }), 
        lanmbda(struct mlist_node *, (void) {
            symbol_t *sym = __symbol_string_new(string);

            return sym?&sym->node:NULL;
        }));
}

static inline symbol_t *
__symbol_push_binary(symbol_table_t *table, void *binary, uint32_t len)
{
    uint32_t hash = __binary_bkdr(binary, len);

    return __symbol_insert(table, 
        lanmbda(int, (void) {
            return hash & (table->size - 1);
        }), 
        lanmbda(int, (struct mlist_node *node) {
            symbol_t *sym = __symbol_entry(node);
            
            return sym->s.hash & (table->size - 1);
        }), 
        lanmbda(bool, (struct mlist_node *node) {
            return __symbol_eq(__symbol_entry(node), binary, true, len, hash);
        }), 
        lanmbda(struct mlist_node *, (void) {
            symbol_t *sym = __symbol_binary_new(binary, len);

            return sym?&sym->node:NULL;
        }));
}

static inline symbol_t *
symbol_push_string(symbol_table_t *table, char *string)
{
    if (NULL==table) {
        return os_assertV(NULL);
    }
    else if (NULL==string) {
        return os_assertV(NULL);
    }
    else {
        return __symbol_push_string(table, string);
    }
}

static inline symbol_t *
symbol_push_binary(symbol_table_t *table, void *binary, uint32_t len)
{
    if (NULL==table) {
        return os_assertV(NULL);
    }
    else if (NULL==binary) {
        return os_assertV(NULL);
    }
    else {
        return __symbol_push_binary(table, binary, len);
    }
}

static inline int
symbol_foreach(symbol_table_t *table, multi_value_t (*cb)(symbol_t *sym))
{
    return mlist_foreach(table, lanmbda(int, (struct mlist_node *node) {
        return (*cb)(__symbol_entry(node));
    }));
}

struct string_cursor {
    char *var;
    int len;
};

static inline int
string_cursor_end(struct string_cursor *cursor, int ch)
{
    int value = cursor->var[cursor->len];

    cursor->var[cursor->len] = ch;

    return value;
}

#define string_cursor_save(_cursor, _ch)    _ch = string_cursor_end(_cursor, 0)
#define string_cursor_restore(_cursor, _ch) string_cursor_end(_cursor, _ch)

#define string_cursor_string_call(_cursor, _call) do{ \
    int ch;                                         \
                                                    \
    string_cursor_save(_cursor, ch);                \
    _call, (void)0;                                 \
    string_cursor_restore(_cursor, ch);             \
}while(0)

static inline bool
string_cursor_eq(struct string_cursor *cursor, char *string)
{
    return 0==string[cursor->len] 
        && 0==os_memcmp(string, cursor->var, cursor->len);
}

#define __STRING_CURSOR(_var, _len) {   \
    .var = _var,                        \
    .len = _len,                        \
}   /* end */

#define STRING_CURSOR   __STRING_CURSOR(NULL, 0)

struct string_kv {
    struct string_cursor k, v;
};

#define __STRING_KV(_k, _klen, _v, _vlen) { \
    .k = __STRING_CURSOR(_k, _klen),        \
    .v = __STRING_CURSOR(_v, _vlen),        \
}   /* end */

#define STRING_KV   __STRING_KV(NULL, 0, NULL, 0)

/******************************************************************************/
#endif /* __LIB_UTILS_STRING_H__ */
