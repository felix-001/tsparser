// Last Update:2019-08-05 17:42:46

#include "include.h"
#include "pmt.h"

int find_pmt( const char *buf_ptr, int len, uint16_t pmt_pid, pmt_t *pmt )
{
    const char *buf_end = buf_ptr + len, *_buf_ptr = buf_ptr;
    uint16_t pid;
    ts_packet_hdr_t *pkt_hdr;

    CHECK_PARAM( !buf_ptr || !pmt );

    while( buf_ptr < buf_end ) {
        if ( *_buf_ptr != 0x47 ) {
            LOGE("check sync code : 0x47 error\n");
            goto err;
        }
        pkt_hdr = (ts_packet_hdr_t*)_buf_ptr;
        pid = pkt_hdr->pid_hi * 256 + pkt_hdr->pid_lo; 
        if ( pid == pmt_pid ) {
            LOGI("found pmt pid\n");
            if ( pkt_hdr->adaptation_field_control != 0x01 ) {
                LOGE("currently not support pat with adaption field\n");
                goto err;
            }
            if ( pkt_hdr->payload_unit_start_indicator ) {
                _buf_ptr++;// pointer field
            }
            _buf_ptr += sizeof(ts_packet_hdr_t);
            break;
        }
    }

    return 0;
err:
    return -1;
}
