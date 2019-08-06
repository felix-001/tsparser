// Last Update:2019-08-06 15:28:02

#include "include.h"

void dbg_arr_dump( uint8_t *buf, int len )
{
    int i = 0;

    for ( i=0; i<len; i++ ) {
        printf("0x%x, ", buf[i] );
    }
    printf("\n");
}
