// Last Update:2019-08-06 10:51:04

#include "include.h"
#include "pat.h"
#include "pmt.h"
#include "mpegts.h"

int parse_ts( char *buf, int len )
{
    ts_stream_t *stream;

    MALLOC_STRUCT( stream, ts_stream_t );    
    CALL( find_pat( buf, len, &stream->pat ) );
    dump_pat( &stream->pat );
    CALL( find_pmt( buf, len, stream->pat.pmt_list[0].pmt_pid, &stream->pmt ) );
    dump_pmt( &stream->pmt );

    return 0;

err:
    return -1;
}
