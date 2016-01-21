#ifndef __TYPE_H_6BA367A7D4B17C3588DAABDD4B9B396D__
#define __TYPE_H_6BA367A7D4B17C3588DAABDD4B9B396D__
/******************************************************************************/
#ifdef BOOL
#undef BOOL
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#endif

#ifdef false
#undef false
#endif

#define BOOL            int
#define TRUE            1
#define FALSE           0

#define bool            BOOL
#define true            TRUE
#define false           FALSE

#ifndef BYTE
#define BYTE            unsigned char
#endif

#ifndef byte
#define byte            BYTE
#endif

#define __BOOL(_x)      ((BOOL)!!(_x))
#define __bool(_x)      __BOOL(_x)

#if defined(LP64) || defined(ILP64) || defined(LLP64)
#define OS64            1
#else
#define OS64            0
#endif

#define OS_ENABLE       1
#define OS_DISABLE      0

#define OS_ON           1
#define OS_OFF          0

#define __stdin         0
#define __stdout        1
#define __stderr        2

#define OS_MV_GO        0
#define OS_MV_BREAK     1

typedef int multi_value_t;

typedef union {
    multi_value_t value;
    
    struct {
        int result:24;
        int control:8;
    } v2;
    
    struct {
        int result:16;
        int control:8;
        int private:8;
    } v3;
    
    struct {
        int result:8;
        int control:8;
        int private:8;
        int value:8;
    } v4;
}
multi_value_u;

#define MV_INITER               { .value = 0 }

#define mv2_result(_mv)         (_mv).v2.result
#define mv2_control(_mv)        (_mv).v2.control
#define mv2_INITER(_control, _result) { \
    .v2 = {                             \
        .result = _result,              \
        .control= _control,             \
    }                                   \
}

static inline multi_value_t 
__mv2_return(int control, int result)
{
    multi_value_u mv = mv2_INITER(control, result);

    return mv.value;
}

#define mv2_BREAK(_result)      __mv2_return(OS_MV_BREAK, _result)
#define mv2_GO(_result)         __mv2_return(OS_MV_GO, _result)
#define mv2_OK                  0

#define mv2_is_break(_mv)       (OS_MV_BREAK==mv2_control(_mv))
#define mv2_is_go(_mv)          (OS_MV_GO==mv2_control(_mv))

#define mv3_result(_mv)         (_mv).v3.result
#define mv3_control(_mv)        (_mv).v3.control
#define mv3_private(_mv)        (_mv).v3.private

#define mv4_result(_mv)         (_mv).v4.result
#define mv4_control(_mv)        (_mv).v4.control
#define mv4_private(_mv)        (_mv).v4.private
#define mv4_value(_mv)          (_mv).v4.value
/******************************************************************************/
#endif /* __TYPE_H_6BA367A7D4B17C3588DAABDD4B9B396D__ */
