
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

//#include "ffm_module_common.h"
#include "ffm_fifo.h"

#define MODULE_NAME     "ffm_fifo"

/* Misc. constants. */

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE  1

#ifdef NULL
#undef NULL
#endif
#define NULL  0

#define ERR(format, arg...)    printf(MODULE_NAME " L: %u " format, __LINE__,  ## arg )




Fifo_Attrs Fifo_Attrs_DEFAULT = {
    0
};

/******************************************************************************
 * Fifo_create
 ******************************************************************************/
Fifo_Handle Fifo_create(Fifo_Attrs *attrs)
{
    Fifo_Handle hFifo;

    if (attrs == NULL) {
        ERR("NULL attrs not supported\n");
        return NULL;
    }

    hFifo = calloc(1, sizeof(Fifo_Object));

    if (hFifo == NULL) {
        ERR("Failed to allocate space for Fifo Object\n");
        return NULL;
    }

    if (pipe(hFifo->pipes)) {
        free(hFifo);
        return NULL;
    }

    pthread_mutex_init(&hFifo->mutex, NULL);
    hFifo->maxsize = 64*4096; // linux: 64*4096
        
    return hFifo;
}

/******************************************************************************
 * Fifo_delete
 ******************************************************************************/
int Fifo_delete(Fifo_Handle hFifo)
{
    int ret = FIFO_EOK;

    if (hFifo) {
        if (close(hFifo->pipes[0])) {
            ret = FIFO_EIO;
        }

        if (close(hFifo->pipes[1])) {
            ret = FIFO_EIO;
        }

        pthread_mutex_destroy(&hFifo->mutex);

        free(hFifo);
    }

    return ret;
}

/******************************************************************************
 * Fifo_get
 ******************************************************************************/
int Fifo_get(Fifo_Handle hFifo, char * ptrPtr)
{
    int flush;
    int numBytes;

    assert(hFifo);
    assert(ptrPtr);

    pthread_mutex_lock(&hFifo->mutex);
    flush = hFifo->flush;
    pthread_mutex_unlock(&hFifo->mutex);

    if (flush) {
        return FIFO_EFLUSH;
    }

    numBytes = read(hFifo->pipes[0], ptrPtr, sizeof(char *));

    if (numBytes != sizeof(char *)) {
        pthread_mutex_lock(&hFifo->mutex);
        flush = hFifo->flush;
        if (flush) {
            hFifo->flush = FALSE;
        }
        pthread_mutex_unlock(&hFifo->mutex);

        if (flush) {
            return FIFO_EFLUSH;
        }
        return FIFO_EIO;
    }

    pthread_mutex_lock(&hFifo->mutex);
    hFifo->numBufs--;
    pthread_mutex_unlock(&hFifo->mutex);

    return FIFO_EOK;
}

int Fifo_get2(Fifo_Handle hFifo, char * ptrPtr, int size)
{
    int flush;
    int numBytes;

    assert(hFifo);
    assert(ptrPtr);

    pthread_mutex_lock(&hFifo->mutex);
    flush = hFifo->flush;
    pthread_mutex_unlock(&hFifo->mutex);

    if (flush) {
        return FIFO_EFLUSH;
    }

    numBytes = read(hFifo->pipes[0], ptrPtr, size);

    if (numBytes != size) {
        pthread_mutex_lock(&hFifo->mutex);
        flush = hFifo->flush;
        if (flush) {
            hFifo->flush = FALSE;
        }
        pthread_mutex_unlock(&hFifo->mutex);

        if (flush) {
            return FIFO_EFLUSH;
        }
        return FIFO_EIO;
    }

    pthread_mutex_lock(&hFifo->mutex);
    hFifo->numBufs-=size;
    pthread_mutex_unlock(&hFifo->mutex);

    return FIFO_EOK;
}

/******************************************************************************
 * Fifo_flush
 ******************************************************************************/
int Fifo_flush(Fifo_Handle hFifo)
{
    char ch = 0xff;

    assert(hFifo);

    pthread_mutex_lock(&hFifo->mutex);
    hFifo->flush = TRUE;
    pthread_mutex_unlock(&hFifo->mutex);

    /* Make sure any Fifo_get() calls are unblocked */
    if (write(hFifo->pipes[1], &ch, 1) != 1) {
        return FIFO_EIO;
    }

    return FIFO_EOK;
}

/******************************************************************************
 * Fifo_put
 ******************************************************************************/
int Fifo_put(Fifo_Handle hFifo, char *  ptr)
{
    assert(hFifo);
    assert(ptr);

    pthread_mutex_lock(&hFifo->mutex);
    hFifo->numBufs++;
    pthread_mutex_unlock(&hFifo->mutex);

    if (write(hFifo->pipes[1], &ptr, sizeof(char *)) != sizeof(char *)) {
        return FIFO_EIO;
    }

    return FIFO_EOK;
}

int Fifo_put2(Fifo_Handle hFifo, char *  ptr, int size)
{
    assert(hFifo);
    assert(ptr);

    if((hFifo->numBufs+size) > hFifo->maxsize)
        return FIFO_EINVAL;
    pthread_mutex_lock(&hFifo->mutex);
    hFifo->numBufs+=size;
    pthread_mutex_unlock(&hFifo->mutex);

    if (write(hFifo->pipes[1], ptr, size) != size) {
        return FIFO_EIO;
    }

    return FIFO_EOK;
}

/******************************************************************************
 * Fifo_getNumEntries
 ******************************************************************************/
int Fifo_getNumEntries(Fifo_Handle hFifo)
{
    int numEntries;

    assert(hFifo);

    pthread_mutex_lock(&hFifo->mutex);
    numEntries = hFifo->numBufs;
    pthread_mutex_unlock(&hFifo->mutex);

    return numEntries;
}

