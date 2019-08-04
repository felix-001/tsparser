// Last Update:2019-08-04 22:21:06

#ifndef PAT_H
#define PAT_H

typedef struct {
#ifdef WORDS_BIGENDIAN
    u8 table_id                             :8;
    u8 section_syntax_indicator             :1;
    u8                                      :3;
    u8 section_length_hi                    :4;
    u8 section_length_lo                    :8;
    u8 transport_stream_id_hi               :8;
    u8 transport_stream_id_lo               :8;
    u8                                      :2;
    u8 version_number                       :5;
    u8 current_next_indicator               :1;
    u8 section_number                       :8;
    u8 last_section_number                  :8;
#else
    u8 table_id                             :8;
    u8 section_length_hi                    :4;
    u8                                      :3;
    u8 section_syntax_indicator             :1;
    u8 section_length_lo                    :8;
    u8 transport_stream_id_hi               :8;
    u8 transport_stream_id_lo               :8;
    u8 current_next_indicator               :1;
    u8 version_number                       :5;
    u8                                      :2;
    u8 section_number                       :8;
    u8 last_section_number                  :8;
#endif
} pat_hdr_t;

typedef struct {
    uint16_t program_number;
    uint16_t pmt_pid;
} program_t;

typedef struct {
    pat_hdr_t hdr;
    int pmt_count;
    program_t *pmt_list;
} pat_t;

extern int find_pat( const char *buf, int len, pat_t *pat );


#endif  /*PAT_H*/
