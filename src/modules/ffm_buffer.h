#ifndef _FFM_BUFFER_H_
#define _FFM_BUFFER_H_

#include <pthread.h>
#include <sys/types.h>

typedef enum{
    BUFFER_TYPE_NONE,
    BUFFER_TYPE_VIDEO,
    BUFFER_TYPE_AUDIO,
    BUFFER_TYPE_RAW_VIDEO,
    BUFFER_TYPE_RAW_AUDIO,
    BUFFER_TYPE_AV_PKTS,
    BUFFER_TYPE_BUFFERS,
    BUFFER_TYPE_CODEC_CONTEXT,
}ffm_buffer_type_t;

typedef struct ffm_buffer_object {

    unsigned int buftype;
    unsigned int bufsize;
    unsigned int used_size;
    unsigned int useMask;
    int          usecnt;
    unsigned int buf_id;
    unsigned int refrence;
    pthread_mutex_t mutex;
    void * ptr;
    void * useptr;
    int64_t ts;
    int     timebase;
    
    int     have_video_stream;
    int     have_audio_stream;
    int     stream_type;  // live, vod ...
    int     stream_start; // 0: not the stream start, 1: is the stream start
    
    //// video
    int     vcodec_id;
    int     vbit_rate;
    int     width;
    int     height;
    int     pix_fmt;
    int     frame_rate;
    int     vextradata_size;
    //// audio
    int     acodec_id;
    int     abit_rate;
    int     channels;
    int     frame_size;
    int     sample_rate;
    int     sample_fmt;
    int64_t channel_layout;
    int     aextradata_size;
    // extradata
    char    aextradata[128];
    char    vextradata[256];
    
}ffm_buffer_object;

typedef struct ffm_buffer_object *ffm_buf_Handle;

typedef struct ffm_buftab_object {
    ffm_buf_Handle * hbufs;
    unsigned int buf_nums;
    void * ptr;
}ffm_buftab_object;




typedef struct ffm_buftab_object *ffm_buftab_Handle;


typedef struct ffm_buffer_attr { 
    unsigned int  bufsize;
    unsigned int  align;
    unsigned int  refrence;
 }ffm_buffer_attr;


typedef struct ffm_buftab_attr { 
    unsigned int  bufsize;
    unsigned int  align;
    unsigned int  buf_nums;
 }ffm_buftab_attr;

ffm_buf_Handle ffm_buffer_create(ffm_buffer_attr * attr);
int ffm_buffer_delete(ffm_buf_Handle hBuf);
void ffm_buffer_setTs(ffm_buf_Handle hBuf, int64_t ts);
int64_t ffm_buffer_getTs(ffm_buf_Handle hBuf);

void ffm_buffer_setId(ffm_buf_Handle hBuf, unsigned int id);
unsigned int ffm_buffer_getSize(ffm_buf_Handle hBuf);

void * ffm_buffer_getPtr(ffm_buf_Handle hBuf);
unsigned int ffm_buffer_getUsedSize(ffm_buf_Handle hBuf);
void ffm_buffer_setUsedSize(ffm_buf_Handle hBuf, unsigned int size);
unsigned int ffm_buffer_getId(ffm_buf_Handle hBuf);
void ffm_buffer_free(ffm_buf_Handle hBuf);
void ffm_buffer_setUsed(ffm_buf_Handle hBuf);
void ffm_buffer_setUsePtr(ffm_buf_Handle hBuf, void * ptr);
unsigned int ffm_buffer_getType(ffm_buf_Handle hBuf);
void ffm_buffer_setType(ffm_buf_Handle hBuf, unsigned int buftype);
int ffm_buffer_getUseCnt(ffm_buf_Handle hBuf);
void ffm_buffer_setUseCnt(ffm_buf_Handle hBuf, int usecnt);
void ffm_buffer_print(ffm_buf_Handle hBuf);

void ffm_buffer_dup(ffm_buf_Handle hBuf);


ffm_buftab_Handle ffm_tab_create(ffm_buftab_attr *attr);
void ffm_tab_delete(ffm_buftab_Handle hBufTab);
ffm_buf_Handle ffm_tab_get_by_id(ffm_buftab_Handle hBufTab,unsigned int id);
ffm_buf_Handle ffm_tab_getFreeBuf(ffm_buftab_Handle hBufTab);
int ffm_tab_get_nums(ffm_buftab_Handle hBufTab);


extern const ffm_buffer_attr ffm_buf_Attrs_DEFAULT;
extern const ffm_buftab_attr ffm_tab_Attrs_DEFAULT;
#endif


