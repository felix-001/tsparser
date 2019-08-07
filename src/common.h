// Last Update:2019-08-05 17:01:31


#ifndef COMMON_H
#define COMMON_H

#define CALL( func )  \
    if ( (func) < 0 ) { \
        LOGE("error\n"); \
        goto err;\
    }

#define MALLOC_STRUCT( ptr, type )  \
do { \
    ptr = (type *)malloc( sizeof(type) ); \
    if ( !ptr ) { \
        LOGE("malloc error\n"); \
        goto err; \
    } \
    memset( ptr, 0, sizeof(type)  ); \
} while(0)

#define CHECK_PARAM( param )  if ( (param) ) { LOGE("check param error\n"); goto err; }

#define ARRSZ(arr) ((sizeof(arr))/(arr[0]))

#   define AV_RB32(x)                           \
    ((((const uint8_t*)(x))[0] << 24) |         \
     (((const uint8_t*)(x))[1] << 16) |         \
     (((const uint8_t*)(x))[2] <<  8) |         \
     ((const uint8_t*)(x))[3])

#   define AV_RB16(x)                           \
    ((((const uint8_t*)(x))[0] << 8) |         \
     (((const uint8_t*)(x))[1] ))

typedef uint8_t u8;

#endif  /*COMMON_H*/
