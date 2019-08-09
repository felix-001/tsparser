// Last Update:2019-08-08 12:35:39

#include "include.h"

#define PES_FIX_HEADER_LEN (3+1+2)// packet_start_code_prefix: 3, stream id : 1, PES_packet_length : 2
#define PES_OPT_HEADER_LEN (3)

static inline int64_t ff_parse_pes_pts(const uint8_t *buf) 
{
    return (int64_t)(*buf & 0x0e) << 29 |
            (AV_RB16(buf+1) >> 1) << 15 |
             AV_RB16(buf+3) >> 1;
}

int parse_h264(const char *buf_ptr, pes_t *pes )
{
    const char *startcode[] = { 0x00, 0x00, 0x00, 0x01 };

    CHECK_PARAM( !buf_ptr || !pes );

    if ( memcmp( buf_ptr, startcode, sizeof(startcode)) != 0 ) {
        LOGE("check nalu start code error\n");
        goto err;
    }
    buf_ptr += 4;
    pes->nalu_type = (*buf_ptr) & 0x1f;

    return 0;
err:
    return -1;
}

int parse_pes_packet(const char *_buf_ptr, int len, int is_start, uint16_t pid, ts_stream_t *stream)
{
    pes_t *pes = NULL;

    CHECK_PARAM( !buf_ptr || !stream );

    if ( pid == stream->pmt.video_pid ) {
        pes = stream->videos[stream->video_index];
        if ( is_start ) {
            stream->video_index++;
        }
    } else if ( pid == stream->pmt.audio_pid ) {
        pes = stream->audios[stream->audio_index];
        if ( is_start ) {
            stream->audio_index++;
        }
    } else {
        LOGE("check pid error");
        goto err;
    }

    if ( is_start ) {
        pes->packet_start_code_prefix = ( buf_ptr[0] << 16 | buf_ptr[1] << 8 | buf_ptr[2] );
        if ( pes->packet_start_code_prefix != 0x01 ) {
            LOGE("check pes start code error\n");
            goto err;
        }
        pes->stream_id = buf_ptr[3];
        if ( pes->stream_id != 0xE0 || pes->stream_id != 0xC0 || pes->stream_id != 0xBD ) {
            LOGE("check stream id error\n");
            goto err;
        }
        pes->PES_packet_length = buf_ptr[4] << 8 | buf_ptr[5];
        buf_ptr += 6;
        pes->opt_hdr = (optional_pes_header_t *)buf_ptr;
        if ( PTS_DTS_flag != 0x2 && PTS_DTS_flag != 0x3 ) {
            LOGE("pts dts flags not set\n");
            goto err;
        }
        buf_ptr += sizeof(optional_pes_header_t);
        pes->pts = ff_parse_pes_pts(buf_ptr);
        if ( !pes->pts ) {
            LOGE("check pts error, pts is 0\n");
            goto err;
        }
        buf_ptr += pes->opt_hdr.PES_header_data_length;
        CALL( parse_h264(buf_ptr, pes) );
        pes->real_length += (TS_PACKET_LEN - sizeof(ts_packet_hdr_t) 
            - PES_FIX_HEADER_LEN - PES_OPT_HEADER_LEN - pes->opt_hdr.PES_header_data_length);
    } else {
        pes->real_length += (TS_PACKET_LEN - sizeof(ts_packet_hdr_t) );
    }

    return 0;
err:
    return -1;
}

int check_continuity_counter( ts_packet_hdr_t *hdr, ts_stream_t *stream )
{
    uint16_t pid;

    CHECK_PARAM( !hdr || !stream );

    pid = hdr->pid_hi * 256 + hdr->pid_lo; 

    if ( pid == stream->pmt.video_pid ) {
        if ( stream->last_video_counter < 0 ) {
            stream->last_video_counter = hdr->continuity_counter;
        } else if ( stream->last_video_counter >= hdr->continuity_counter ) {
            LOGE("check video continuity_counter error\n");
            goto err;
        }
    } else if ( pid == stream->pmt.audio_pid ) {
        if ( stream->last_audio_counter < 0 ) {
            stream->last_audio_counter = hdr->continuity_counter;
        } else if ( stream->last_audio_counter >= hdr->continuity_counter ) {
            LOGE("check audio continuity_counter error\n");
            goto err;
        }
    } else {
        LOGE("check pid error\n");
        goto err;
    }

    return 0;
err:
    return -1;
}

int parse_pes( const char *_buf_ptr, int len, ts_stream_t *stream )
{
    const char *buf_ptr = _buf_ptr, *buf_end = _buf_ptr + len;
    ts_packet_hdr_t *hdr;
    uint16_t pid;

    CHECK_PARAM( !buf_ptr || !stream );

    while( buf_ptr < buf_end ) {
        hdr = (ts_packet_hdr_t *)buf_ptr;
        pid = hdr->pid_hi * 256 + hdr->pid_lo; 
        if ( pid == stream->pmt.video_pid || pid == stream->pmt.audio_pid ) {
            CALL( check_continuity_counter( hdr, stream ) );
            if ( hdr->adaptation_field_control == 0x02 || hdr->adaptation_field_control == 0x03 ) {
                LOGE("pes packet has adaption filed, skip\n");
                continue;
            }
            CALL( parse_pes_packet( buf_ptr + sizeof(ts_packet_hdr_t),
                                    TS_PACKET_LEN - sizeof(ts_packet_hdr_t), (hdr->payload_unit_start_indicator == 1), pid, stream ) );
        }
        buf_ptr += TS_PACKET_LEN;
    }

    return 0;
err:
    return -1;
}
