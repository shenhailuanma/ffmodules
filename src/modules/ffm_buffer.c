
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <inttypes.h> 

#include "ffm_buffer.h"





#define MODULE_NAME     "ffm_buffer"

enum{ true=1,   false=0 };


const ffm_buffer_attr ffm_buf_Attrs_DEFAULT = {
    188,
    4,
    0
};

const ffm_buftab_attr ffm_tab_Attrs_DEFAULT = {
    188,
    4,
    2400
};


ffm_buf_Handle ffm_buffer_create(ffm_buffer_attr * attr)
{
    ffm_buf_Handle handle = NULL;
    unsigned int bufsize;

    if(attr == NULL)
    {
        printf("ffm_buffer_create attr NULL pointer\n");
        goto cleanup;
    }
        
    handle = calloc(1, sizeof(ffm_buffer_object));
    if(handle == NULL)
    {
        printf("ffm_buffer_create ffm_buf_Handle create failed\n");
        goto cleanup;
    }

    if(attr->align == 0)
    {
        printf("align can not be 0\n");
        goto cleanup;
    }
    
    //bzero(handle,sizeof(ffm_buffer_object));
    memset((void *)handle, 0, sizeof(ffm_buffer_object));

    bufsize = ((attr->bufsize + attr->align - 1) / attr->align) * attr->align;
    handle->refrence = attr->refrence;
    
    if(handle->refrence == 0)
    {
        handle->ptr = calloc(1,bufsize);
        if(handle->ptr == NULL)
        {
            printf("ffm_buffer_create handle->ptr create failed\n");
            goto cleanup;
        }
        handle->useptr = handle->ptr;
    }

    handle->bufsize = attr->bufsize; 
    
    pthread_mutex_init(&handle->mutex, NULL);


    return handle;
        
    cleanup:
        
        if(handle != NULL)
        {
            free(handle);
        }

        return NULL;

}

int ffm_buffer_delete(ffm_buf_Handle hBuf)
{
        if(hBuf != NULL)
        {
            if(hBuf->usecnt <= 0){
                if(hBuf->ptr != NULL)
                {
                    free(hBuf->ptr);
                    pthread_mutex_destroy(&hBuf->mutex);
                }
                free(hBuf);
            }else{
                pthread_mutex_lock(&hBuf->mutex);
                hBuf->usecnt--;
                pthread_mutex_unlock(&hBuf->mutex);
            }
        }

        return 0;
}

int ffm_buffer_getUseCnt(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getUseCnt NULL pointer\n");
         return 0;       
    }

    return hBuf->usecnt;

}

/*
ffm_buffer_setUseCnt
if buffer used by 5, set "ffm_buffer_setUseCnt(buf,5);

*/
void ffm_buffer_setUseCnt(ffm_buf_Handle hBuf, int usecnt)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getUseCnt NULL pointer\n");
        return;       
    }
    pthread_mutex_lock(&hBuf->mutex);
    hBuf->usecnt = (usecnt-1);
    pthread_mutex_unlock(&hBuf->mutex);

}

void ffm_buffer_dup(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getUseCnt NULL pointer\n");
        return;       
    }
    pthread_mutex_lock(&hBuf->mutex);
    hBuf->usecnt++;
    pthread_mutex_unlock(&hBuf->mutex);
}


int64_t ffm_buffer_getTs(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getTs NULL pointer\n");
         return 0;       
    }

    return hBuf->ts;
}

/******************************************************************************
 * _Buffer_setTs (INTERNAL)
 ******************************************************************************/
void ffm_buffer_setTs(ffm_buf_Handle hBuf, int64_t ts)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_setTs NULL pointer\n");
        return;      
    }
    pthread_mutex_lock(&hBuf->mutex);
    hBuf->ts = ts;
    pthread_mutex_unlock(&hBuf->mutex);
}

/******************************************************************************
 * _Buffer_setType(INTERNAL)
 ******************************************************************************/
void ffm_buffer_setType(ffm_buf_Handle hBuf, unsigned int buftype)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_setTs NULL pointer\n");
        return;      
    }
    pthread_mutex_lock(&hBuf->mutex);
    hBuf->buftype = buftype;
    pthread_mutex_unlock(&hBuf->mutex);
}


/******************************************************************************
 * _Buffer_setId (INTERNAL)
 ******************************************************************************/
void ffm_buffer_setId(ffm_buf_Handle hBuf, unsigned int id)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_setId NULL pointer\n");
        return;      
    }
    
    pthread_mutex_lock(&hBuf->mutex);
    hBuf->buf_id = id;
    pthread_mutex_unlock(&hBuf->mutex);
}

unsigned int ffm_buffer_getId(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getId NULL pointer\n");
        return 0;      
    }

    return hBuf->buf_id;
}


/******************************************************************************
 * Buffer_getSize
 ******************************************************************************/
unsigned int ffm_buffer_getSize(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getSize NULL pointer\n");
        return 0;      
    }

    return hBuf->bufsize;
}

/******************************************************************************
 * Buffer_getSize
 ******************************************************************************/
unsigned int ffm_buffer_getuseMask(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getuseMask NULL pointer\n");
        return 0;      
    }

    return hBuf->useMask;
}

void ffm_buffer_setUsed(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_setUsed NULL pointer\n");
        return;      
    }
    pthread_mutex_lock(&hBuf->mutex);
    hBuf->useMask = 1;
    pthread_mutex_unlock(&hBuf->mutex);
}


unsigned int ffm_buffer_getType(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getTs NULL pointer\n");
         return 0;       
    }

    return hBuf->buftype;
}


void * ffm_buffer_getPtr(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getPtr NULL pointer\n");
        return NULL;        
    }

    return hBuf->useptr;
}

void ffm_buffer_setUsePtr(ffm_buf_Handle hBuf, void * ptr)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_setUsePtr NULL pointer\n");
        return;      
    }
    pthread_mutex_lock(&hBuf->mutex);

    hBuf->useptr = ptr;
    pthread_mutex_unlock(&hBuf->mutex);
}


unsigned int ffm_buffer_getUsedSize(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_getUsedSize NULL pointer\n");
        return 0;        
    }

    return hBuf->used_size;
}

void ffm_buffer_setUsedSize(ffm_buf_Handle hBuf, unsigned int size)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_setUsedSize NULL pointer\n");
        return;      
    }
    pthread_mutex_lock(&hBuf->mutex);

    hBuf->used_size = size;
    pthread_mutex_unlock(&hBuf->mutex);
}



void ffm_buffer_free(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_free NULL pointer\n");
        return;      
    }

    hBuf->useMask = 0;
}

void ffm_buffer_print(ffm_buf_Handle hBuf)
{
    if(hBuf == NULL)
    {
        printf("ffm_buffer_free NULL pointer\n");
        return;      
    }

    printf("buffer: buftype=%d, bufsize=%d, used_size=%d, useMask=%d, usecnt=%d,buf_id=%d, ts=%"PRId64", timebase=%d\n",
        hBuf->buftype,hBuf->bufsize,hBuf->used_size,hBuf->useMask,hBuf->usecnt,
        hBuf->buf_id,hBuf->ts,hBuf->timebase);
    
    printf("vcodec_id=%d, width=%d, height=%d, pix_fmt=%d, frame_rate=%d, vextradata_size=%d\n"\
        "acodec_id=%d, channels=%d, sample_rate=%d, sample_fmt=%d, aextradata_size=%d\n", 
        hBuf->vcodec_id,hBuf->width,hBuf->height,hBuf->pix_fmt,hBuf->frame_rate,hBuf->vextradata_size,
        hBuf->acodec_id,hBuf->channels,hBuf->sample_rate,hBuf->sample_fmt,hBuf->aextradata_size);
        
}



/////////////////////////////
//buffer table


ffm_buftab_Handle ffm_tab_create(ffm_buftab_attr *attr)
{
    ffm_buftab_Handle hBufTab = NULL;
    ffm_buffer_attr  buf_attr;
    int nums = 0;
    int i = 0;
    unsigned int bufsize;

    if(attr == NULL)
    {
        printf("ffm_buftab_create attr NULL pointer\n");
        goto cleanup;
    }
        
    hBufTab = calloc(1, sizeof(ffm_buftab_object));
    if(hBufTab == NULL)
    {
        printf("ffm_buftab_create hBufTab create failed\n");
        goto cleanup;
    }
    
    //bzero(hBufTab,sizeof(ffm_buftab_object));
    memset((void *)hBufTab, 0, sizeof(ffm_buftab_object));
    hBufTab->buf_nums = nums = attr->buf_nums;
    buf_attr.align = attr->align;
    buf_attr.bufsize = attr->bufsize;
    buf_attr.refrence = true;

    if(attr->align == 0)
    {
        printf("align can not be 0\n");
        goto cleanup;
    }

    bufsize = ((attr->bufsize + attr->align - 1) / attr->align) * attr->align;
    

    hBufTab->hbufs = calloc(1, sizeof(ffm_buffer_object) * nums);
    if(hBufTab->hbufs  == NULL)
    {
        printf("ffm_buftab_create hBufTab->hbufs  create failed\n");
        goto cleanup;
    }

    hBufTab->ptr = calloc(1, bufsize * nums);
    if(hBufTab->ptr == NULL)
    {
        printf("ffm_buftab_create hBufTab->ptr  create failed\n");
        goto cleanup;
    }
    
    for(i = 0; i < nums;i++)
    {
        hBufTab->hbufs[i] = ffm_buffer_create(&buf_attr); 
        if(hBufTab->hbufs[i] == NULL)
        {
            goto cleanup;
        }
        ffm_buffer_setUsePtr(hBufTab->hbufs[i], hBufTab->ptr + i * bufsize);
        ffm_buffer_setId(hBufTab->hbufs[i], i);
    }
    
    return hBufTab;

    cleanup:
        if(hBufTab != NULL)
        {
            if(hBufTab->hbufs)
            {
                for(i = 0; i < nums;i++)
                {
                    if(hBufTab->hbufs[i])
                    {
                        ffm_buffer_delete(hBufTab->hbufs[i]);
                        hBufTab->hbufs[i] = NULL;
                    }
                }
                free(hBufTab->hbufs);
            }
            
            if(hBufTab->ptr)
            {
                free(hBufTab->ptr);
            }

         

            free(hBufTab);
        }

        return NULL;
}

void ffm_tab_delete(ffm_buftab_Handle hBufTab)
{
    int i;
    
    if(hBufTab != NULL)
    {   if(hBufTab->hbufs)
         {
            for(i = 0; i < hBufTab->buf_nums;i++)
            {
                if(hBufTab->hbufs[i])
                {
                    ffm_buffer_delete(hBufTab->hbufs[i]);
                    hBufTab->hbufs[i] = NULL;
                }
            }
            free(hBufTab->hbufs);
         }
         
    if(hBufTab->ptr)
    {
        free(hBufTab->ptr);
    }
    
    free(hBufTab);

    }    
}

ffm_buf_Handle ffm_tab_get_by_id(ffm_buftab_Handle hBufTab,unsigned int id)
{

    if(hBufTab == NULL)
    {
        printf("ffm_tab_get_by_id pointer NULL\n");
        return NULL;
    }
    
    if(id < hBufTab->buf_nums)
    {
        return hBufTab->hbufs[id];
    }

    return NULL;
}

/******************************************************************************
 * BufTab_getFreeBuf
 ******************************************************************************/
ffm_buf_Handle ffm_tab_getFreeBuf(ffm_buftab_Handle hBufTab)
{
    int bufIdx;
    unsigned int useMask;

    if(hBufTab == NULL)
    {
        printf("ffm_tab_get_by_id pointer NULL\n");
        return NULL;
    }

    for (bufIdx = 0; bufIdx < hBufTab->buf_nums; bufIdx++) {
        useMask = ffm_buffer_getuseMask(hBufTab->hbufs[bufIdx]);
        if (!useMask) {
            ffm_buffer_setUsed(hBufTab->hbufs[bufIdx]);
            break;
        }
    }

    if (bufIdx == hBufTab->buf_nums) {
        return NULL;
    }

    return ffm_tab_get_by_id(hBufTab, bufIdx);
}

int ffm_tab_get_nums(ffm_buftab_Handle hBufTab)
{
    return hBufTab->buf_nums;
}

