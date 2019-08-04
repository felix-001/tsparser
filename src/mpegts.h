// Last Update:2019-08-04 18:31:17


#ifndef MPEGTS_H
#define MPEGTS_H

#define TS_PACKET_LEN (188)

typedef struct{
#ifdef WORDS_BIGENDIAN
    u8 sync_byte                            :8;   /* always be 0x47 */
    u8 transport_error_indicator            :1;
    u8 payload_unit_start_indicator         :1;   /* if a PES-packet starts in the TS-packet */
    u8 transport_priority                   :1;   /* meanless to IRD, can be ignored */
    u8 pid_hi                               :5;
    u8 pid_lo                               :8;
    u8 transport_scrambling_control         :2;   /* 00: no scramble, 01: reserved,
                                                           10: even key scrambled, 11: odd key scrambled */
    u8 adaptation_field_control             :2;   /* 00: reserved
                                                           01: no adaptation field, payload only
                                                           10: adaptation field only, no payload
                                                           11: adaptation field followed by payload */
    u8 continuity_counter                   :4;
#else
    u8 sync_byte                            :8;
    u8 pid_hi                               :5;
    u8 transport_priority                   :1;
    u8 payload_unit_start_indicator         :1;
    u8 transport_error_indicator            :1;
    u8 pid_lo                               :8;
    u8 continuity_counter                   :4;
    u8 adaptation_field_control             :2;
    u8 transport_scrambling_control         :2;
#endif
}ts_packet_hdr_t;

typedef struct {
    uint16_t pmt_pid;
    uint16_t video_pid;
    uint16_t audio_pid;
} ts_stream_t;


extern int parse_ts( char *buf, int len );

#endif  /*MPEGTS_H*/
