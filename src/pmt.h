// Last Update:2019-08-06 10:15:42


#ifndef PMT_H
#define PMT_H

typedef struct{
#ifdef WORDS_BIGENDIAN
    u8 table_id                             :8;
    u8 section_syntax_indicator             :1;
    u8                                      :3;
    u8 section_length_hi                    :4;
    u8 section_length_lo                    :8;
    u8 program_number_hi                    :8;
    u8 program_number_lo                    :8;
    u8                                      :2;
    u8 version_number                       :5;
    u8 current_next_indicator               :1;
    u8 section_number                       :8;
    u8 last_section_number                  :8;
    u8                                      :3;
    u8 pcr_pid_hi                           :5;
    u8 pcr_pid_lo                           :8;
    u8                                      :4;
    u8 program_info_length_hi               :4;
    u8 program_info_length_lo               :8;
#else   
    u8 table_id                             :8;
    u8 section_length_hi                    :4;
    u8                                      :3;
    u8 section_syntax_indicator             :1;
    u8 section_length_lo                    :8;
    u8 program_number_hi                    :8;
    u8 program_number_lo                    :8;
    u8 current_next_indicator               :1;
    u8 version_number                       :5;
    u8                                      :2;
    u8 section_number                       :8;
    u8 last_section_number                  :8;
    u8 pcr_pid_hi                           :5;
    u8                                      :3;
    u8 pcr_pid_lo                           :8;
    u8 program_info_length_hi               :4;
    u8                                      :4;
    u8 program_info_length_lo               :8;
#endif
} pmt_hdr_t;

typedef struct {
    pmt_hdr_t hdr;
    int video_pid;
    int audio_pid;
} pmt_t;

extern int find_pmt( const char *buf_ptr, int len, uint16_t pmt_pid, pmt_t *pmt );
extern void dump_pmt( pmt_t *pmt );

#endif  /*PMT_H*/
