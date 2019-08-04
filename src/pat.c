// Last Update:2019-08-04 22:41:59

#include "include.h"
#include "mpegts.h"
#include "pat.h"

int find_pat( const char *buf_ptr, int len, pat_t *pat )
{
    uint16_t pid,section_length;
    const char *buf_end = buf_ptr+len;
    program_t *program;
    int i = 0;

    while( buf_ptr <= buf_end ) {
        if ( *buf_ptr != 0x47 ) {
            LOGE("check sync code : 0x47 error\n");
            goto err;
        }
        pid = ((ts_packet_hdr_t*)buf_ptr)->pid_hi * 256 + ((ts_packet_hdr_t*)buf_ptr)->pid_lo; 
        if ( pid == 0 ) {
            pat->hdr = *(pat_hdr_t *)buf_ptr;
            section_length = pat->hdr.section_length_hi << 8 | pat->hdr.section_length_lo;
            pat->pmt_count =(section_length - 4 - 5)/4; 
            pat->pmt_list = (program_t *)malloc( sizeof(program_t)*pat->pmt_count);
            if ( !pat->pmt_list ) {
                LOGE("malloc error\n");
                goto err;
            }
            buf_ptr += sizeof(ts_packet_hdr_t);
            for ( i=0; i<pat->pmt_count; i++ ) {
                program = (program_t *)(pat->pmt_list + sizeof(program_t)*i);
                program->program_number = buf_ptr[0]<<8 | buf_ptr[1];
                buf_ptr += 2;
                program->pmt_pid = (buf_ptr[0] & 0x1f) << 8 | buf_ptr[1];
                buf_ptr += 2;
            }
            break;
        }
        buf_ptr += TS_PACKET_LEN;
    }

    return 0;
err:
    return -1;
}
