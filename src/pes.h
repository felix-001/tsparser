// Last Update:2019-08-07 18:48:38


#ifndef PES_H
#define PES_H

#define NALU_TYPE_IDR 5
#define NALU_TYPE_SEI 6
#define NALU_TYPE_SPS 7
#define NALU_TYPE_PPS 8
#define NALU_TYPE_NON_IDR 1
#define NALU_TYPE_AUD 9

typedef struct {
#ifdef WORDS_BIGENDIAN
    u8                                      :2;
    u8 PES_scrambling_control               :2;
    u8 PES_priority                         :1;
    u8 data_alignment_indicator             :1;
    u8 copyright                            :1;
    u8 original_or_copy                     :1;
    u8 PTS_DTS_flag                         :2;
    u8 ESCR_flag                            :1;
    u8 ES_rate_flag                         :1;
    u8 DSM_trick_mode_flag                  :1;
    u8 additional_copyright_info_flag       :1;
    u8 PES_crc_flag                         :1;
    u8 PES_extension_flag                   :1;
    u8 PES_header_data_length               :8;
#else
    u8 PES_scrambling_control               :2;
    u8 PES_priority                         :1;
    u8 data_alignment_indicator             :1;
    u8 copyright                            :1;
    u8 original_or_copy                     :1;
    u8                                      :2;
    u8 ESCR_flag                            :1;
    u8 ES_rate_flag                         :1;
    u8 DSM_trick_mode_flag                  :1;
    u8 additional_copyright_info_flag       :1;
    u8 PES_crc_flag                         :1;
    u8 PES_extension_flag                   :1;
    u8 PTS_DTS_flag                         :2;
    u8 PES_header_data_length               :8;
#endif
} optional_pes_header_t;


typedef struct {
    uint32_t packet_start_code_prefix;
    uint8_t stream_id;
    uint16_t PES_packet_length;
    int64_t pts;
    int64_t dts;
    uint32_t real_length;
    uint8_t nalu_type;
    optional_pes_header_t opt_hdr;
} pes_t;

#endif  /*PES_H*/
