// Last Update:2019-08-05 17:28:11

#include "include.h"
#include "mpegts.h"
#include "pat.h"

int find_pat( const char *buf_ptr, int len, pat_t *pat )
{
    uint16_t pid,section_length;
    const char *buf_end = buf_ptr+len, *_buf_ptr = buf_ptr;
    program_t *program;
    int i = 0;
    ts_packet_hdr_t *pkt_hdr;

    while( buf_ptr <= buf_end ) {
        if ( *_buf_ptr != 0x47 ) {
            LOGE("check sync code : 0x47 error\n");
            goto err;
        }
        pkt_hdr = (ts_packet_hdr_t*)_buf_ptr;
        pid = pkt_hdr->pid_hi * 256 + pkt_hdr->pid_lo; 
        if ( pid == 0 ) {
            if ( pkt_hdr->adaptation_field_control != 0x01 ) {
                LOGE("currently not support pat with adaption field\n");
                goto err;
            }
            if ( pkt_hdr->payload_unit_start_indicator ) {
                _buf_ptr++;// pointer field
            }
            _buf_ptr  += sizeof(ts_packet_hdr_t);
            pat->hdr = *(pat_hdr_t *)_buf_ptr;
            if ( pat->hdr.table_id != 0x00 ) {
                LOGE("check pat table id error\n");
                goto err;
            }
            if ( !pat->hdr.section_syntax_indicator ) {
                LOGE("check pat section_syntax_indicator error\n");
                goto err;
            }
            section_length = pat->hdr.section_length_hi << 8 | pat->hdr.section_length_lo;
            LOGI("section_length = %d\n", section_length );
            if ( section_length <= 0 ) {
                LOGE("check pat section length error\n");
                goto err;
            }
            pat->pmt_count =(section_length - 4 - 5)/4; 
            LOGI("pat->pmt_count = %d\n", pat->pmt_count );
            pat->pmt_list = (program_t *)malloc( sizeof(program_t)*(pat->pmt_count));
            if ( !(pat->pmt_list) ) {
                LOGE("malloc error, %s\n", strerror(errno) );
                goto err;
            }
            _buf_ptr += sizeof(pat_hdr_t);
            for ( i=0; i<pat->pmt_count; i++ ) {
                program = &pat->pmt_list[i];
                program->program_number = _buf_ptr[0]<<8 | _buf_ptr[1];
                _buf_ptr += 2;
                program->pmt_pid = (_buf_ptr[0] & 0x1f) << 8 | _buf_ptr[1];
                _buf_ptr += 2;
            }
            break;
        }
        buf_ptr += TS_PACKET_LEN;
    }

    return 0;
err:
    return -1;
}

void dump_pat( pat_t *pat )
{
    int i = 0;
    program_t *program;

    CHECK_PARAM( !pat || !(pat->pmt_list) );

    for ( i=0; i<pat->pmt_count; i++ ) {
        program = &(pat->pmt_list[i]);
        LOGI("stream: %d program number: %d pmt pid: 0x%x\n", i, program->program_number, program->pmt_pid );
    }

err:
    return;
}
