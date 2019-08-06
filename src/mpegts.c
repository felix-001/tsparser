// Last Update:2019-08-06 23:12:48

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
    if ( stream->pat.pmt_list[0].program_number != 
         ( stream->pmt.hdr.program_number_hi<<8 | stream->pmt.hdr.program_number_lo ) ){
        LOGE("check program_number error\n");
    }

    return 0;

err:
    return -1;
}
