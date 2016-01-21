#ifndef __TIME_H_1E5C2E2238BA2157D2C85F1285F8A579__
#define __TIME_H_1E5C2E2238BA2157D2C85F1285F8A579__
/******************************************************************************/
#define time_sec(_ms)       ((_ms)/1000)
#define time_usec(_ms)      (((_ms) % 1000) * 1000)
#define time_nsec(_ms)      (((_ms) % 1000) * 1000 * 1000)

#define time_ms2tick(_ms, _tick)    (__os_align(_ms, _tick)/_tick)

#ifdef __APP__
#define __os_time_format    "1900-01-01 00:00:00"

#define USE_UTC_TIME    1

static inline struct tm *
os_localtime(time_t *c)
{
    if (c) {
        return localtime(c);
    } else {
        time_t t = time(NULL);
    
        return localtime(&t);
    }
}

static inline struct tm *
os_gmtime(time_t *c)
{
    if (c) {
        return gmtime(c);
    } else {
        time_t t = time(NULL);

        return gmtime(&t);
    }
}

#if USE_UTC_TIME
#define os_gettm(_time) os_gmtime(_time)
#else
#define os_gettm(_time) os_localtime(_time)
#endif

static inline char *
__get_now_date(struct tm *tm, int ifs)
{
    static char current[sizeof(__os_time_format)];
    
    os_saprintf(current, "%04d%c%02d%c%02d",
                1900 + tm->tm_year, ifs, 1 + tm->tm_mon, ifs, tm->tm_mday);
    
    return current;
}

static inline char *
get_now_date(time_t *t)
{
    return __get_now_date(os_gettm(t), '-');
}

static inline char *
__get_now_time(struct tm *tm, int ifs)
{
    static char current[sizeof(__os_time_format)];
    
    os_saprintf(current, "%02d%c%02d%c%02d",
                tm->tm_hour, ifs, tm->tm_min, ifs, (61==tm->tm_sec)?59:tm->tm_sec);
    
    return current;
}

static inline char *
get_now_time(time_t *t)
{
    return __get_now_time(os_gettm(t), ':');
}

static inline char *
__get_full_time(struct tm *tm, int ifs[3])
{
    static char current[sizeof(__os_time_format)];

    os_saprintf(current, "%s%c%s",
                __get_now_date(tm, ifs[0]),
                ifs[1],
                __get_now_time(tm, ifs[2]));

    return current;
}

static inline char *
get_full_time(time_t *t)
{
    int ifs[] = {'-', ' ', ':'};

    return __get_full_time(os_gettm(t), ifs);
}
#endif

/******************************************************************************/
#endif /* __TIME_H_1E5C2E2238BA2157D2C85F1285F8A579__ */
