// Last Update:2019-08-05 14:35:20

#include "include.h"
#include "pat.h"
#include "mpegts.h"

int parse_ts( char *buf, int len )
{
    ts_stream_t *stream;

    MALLOC_STRUCT( stream, ts_stream_t );    
    CALL( find_pat( buf, len, &stream->pat ) );
    dump_pat( &stream->pat );

    return 0;

err:
    return -1;
}
