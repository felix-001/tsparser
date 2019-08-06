// Last Update:2019-08-06 15:37:20

#include "include.h"
#include "pmt.h"
#include "mpegts.h"

int find_pmt( const char *buf_ptr, int len, uint16_t pmt_pid, pmt_t *pmt )
{
    const char *buf_end = buf_ptr + len, *_buf_ptr = buf_ptr, *start;
    uint16_t pid, section_length, program_info_length, es_loop_length;
    uint16_t elementary_pid, es_info_length;
    uint8_t stream_type;
    ts_packet_hdr_t *pkt_hdr;

    CHECK_PARAM( !buf_ptr || !pmt );

    while( buf_ptr < buf_end ) {
        _buf_ptr = buf_ptr;
        if ( *_buf_ptr != 0x47 ) {
            LOGE("check sync code : 0x47 error\n");
            goto err;
        }
        pkt_hdr = (ts_packet_hdr_t*)_buf_ptr;
        pid = pkt_hdr->pid_hi * 256 + pkt_hdr->pid_lo; 
        LOGI("pid = 0x%x\n", pid );
        if ( pid == pmt_pid ) {
            LOGI("found pmt pid\n");
            if ( pkt_hdr->adaptation_field_control != 0x01 ) {
                LOGE("currently not support pat with adaption field\n");
                goto err;
            }
            _buf_ptr += sizeof(ts_packet_hdr_t);
            if ( pkt_hdr->payload_unit_start_indicator ) {
                _buf_ptr++;// pointer field
            }
            pmt->hdr = *(pmt_hdr_t *)_buf_ptr;
            if( !pmt->hdr.section_syntax_indicator ) {
                LOGE("check pmt section_syntax_indicator error\n");
                goto err;
            }
            if ( pmt->hdr.table_id != 0x02 ) {
                LOGE("pmt check table_id error\n");
                goto err;
            }
            section_length = pmt->hdr.section_length_hi << 8 | pmt->hdr.section_length_lo;
            program_info_length = pmt->hdr.program_info_length_hi << 8 | pmt->hdr.program_info_length_lo;
            _buf_ptr += sizeof(pmt_hdr_t) + program_info_length;
            start = _buf_ptr;
            es_loop_length = section_length - 9 - 5 - program_info_length;
            while ( (_buf_ptr - start) < es_loop_length ) {
                stream_type = *_buf_ptr++;
                elementary_pid = (_buf_ptr[0] & 0x1f)<<8 | _buf_ptr[1];
                if ( stream_type == 0x1b ) {
                    pmt->video_pid = elementary_pid;
                } else if ( stream_type == 0x0f ) {
                    pmt->audio_pid = elementary_pid;
                } else if ( stream_type == 0x06 ) {
                    pmt->audio_pid = elementary_pid;
                } else {
                    // TODO 
                    // suuport more stream_type
                    LOGE("unspported stream_type: 0x%x\n", stream_type );
                    exit(0);
                }
                _buf_ptr += 2;
                es_info_length = (_buf_ptr[0] & 0x0f)<<8 | _buf_ptr[1];
                _buf_ptr += 2 + es_info_length;
            }
            return 0;
        }
        buf_ptr += TS_PACKET_LEN;
    }

    LOGE("pmt pid not found\n");
err:
    return -1;
}

void dump_pmt( pmt_t *pmt )
{
    CHECK_PARAM( !pmt );

    dbg_arr_dump( (uint8_t*)&pmt->hdr, 32 );
    LOGI("pmt table_id : 0x%x\n", pmt->hdr.table_id );
    LOGI("pmt->hdr.program_info_length_lo = 0x%x\n", pmt->hdr.program_info_length_lo);
    LOGI("pmt->hdr.program_info_length_hi = 0x%x\n", pmt->hdr.program_info_length_hi);
    LOGI("pmt program number : 0x%x\n", ((pmt->hdr.program_number_hi)<<8 | pmt->hdr.program_info_length_lo) );
    LOGI("video pid : 0x%x\n", pmt->video_pid);
    LOGI("audio pid : 0x%x\n", pmt->audio_pid );

err:
    return;
}

