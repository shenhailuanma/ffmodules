
#ifndef _FFM_FIFO_H_
#define _FFM_FIFO_H_
#include <pthread.h>

/*
 * Error codes used by fifo. Negative values are errors,
 * while positive values indicate success.
 */
#define FIFO_EINVAL     -5   /**< Invalid input arguments (failure). */
#define FIFO_ENOMEM     -4   /**< No memory available (failure). */
#define FIFO_EIO        -3   /**< An IO error occured (failure). */
#define FIFO_ENOTIMPL   -2   /**< Functionality not implemented (failure). */
#define FIFO_EFAIL      -1   /**< General failure code (failure). */
#define FIFO_EOK         0   /**< General success code (success). */
#define FIFO_EFLUSH      1   /**< The command was flushed (success). */
#define FIFO_EPRIME      2   /**< The command was primed (success). */
#define FIFO_EFIRSTFIELD 3   /**< Only the first field was processed (success)*/
#define FIFO_EBITERROR   4   /**< There was a non fatal bit error (success). */
#define FIFO_ETIMEOUT    5   /**< The operation was timed out (success). */
#define FIFO_EEOF        6   /**< The operation reached end of file */
#define FIFO_EAGAIN      7   /**< The command needs to be rerun (success). */


typedef struct Fifo_Object {
    pthread_mutex_t mutex;
    int             numBufs;
    int             flush;
    int             pipes[2];
    int             maxsize;
    int             inModId;
    int             outModId;
    int             inused;
    int             outused;
    int             switchflag;
} Fifo_Object;


/**
 * @brief       Handle through which to reference a Fifo.
 */
typedef struct Fifo_Object *Fifo_Handle;

/**
 * @brief       Attributes used to create a Fifo.
 * @see         Fifo_Attrs_DEFAULT.
 */
typedef struct Fifo_Attrs {
    /** 
     * @brief      Maximum elements that can be put on the Fifo at once
     * @remarks    For Bios only, Linux ignores this attribute
     */     
    int maxElems;
} Fifo_Attrs;

/**
 * @brief       Default attributes for a Fifo.
 * @code
 * numElems     = 20
 * @endcode
 */
extern  Fifo_Attrs Fifo_Attrs_DEFAULT;



/**
 * @brief       Creates a fifo.
 *
 * @param[in]   attrs       #Fifo_Attrs to use for creating the Fifo.
 *
 * @retval      Handle for use in subsequent operations (see #Fifo_Handle).
 * @retval      NULL for failure.
 */
extern Fifo_Handle Fifo_create(Fifo_Attrs *attrs);

/**
 * @brief       Blocking call to receive a buffer pointer from a fifo.
 *
 * @param[in]   hFifo       #Fifo_Handle from which to receive a buffer.
 * @param[out]  ptrPtr      A pointer to the pointer to be set.
 *
 * @retval      Dmai_EOK if a buffer was successfully received.
 * @retval      Dmai_EFLUSH if the fifo was flushed.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Fifo_create must be called before this function.
 */
extern int Fifo_get(Fifo_Handle hFifo, char * ptrPtr);
extern int Fifo_get2(Fifo_Handle hFifo, char * ptrPtr, int size);

/**
 * @brief       Flushes a fifo. The other end will unblock and return the
 *              (non-negative) #Dmai_EFLUSH error code.
 *
 * @param[in]   hFifo       #Fifo_Handle which to flush.
 *
 * @retval      Dmai_EOK for success.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Fifo_create must be called before this function.
 */
extern int Fifo_flush(Fifo_Handle hFifo);

/**
 * @brief       Put a buffer pointer on the fifo.
 *
 * @param[in]   hFifo       #Fifo_Handle to which to send a buffer.
 * @param[in]   ptr         The pointer to put to the fifo.
 *
 * @retval      Dmai_EOK for success.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Fifo_create must be called before this function.
 */
extern int Fifo_put(Fifo_Handle hFifo, char *  ptr);
extern int Fifo_put2(Fifo_Handle hFifo, char *  ptr, int size);

/**
 * @brief       Determine number of entries (pointers) currently in fifo.
 *
 * @param[in]   hFifo       #Fifo_Handle which to investigate.
 *
 * @retval      Number of entries in the fifo on success.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Fifo_create must be called before this function.
 */
extern int Fifo_getNumEntries(Fifo_Handle hFifo);

/**
 * @brief       Deletes a previously created fifo.
 *
 * @param[in]   hFifo       #Fifo_Handle for the fifo to delete.
 *
 * @retval      Dmai_EOK for success.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Fifo_create must be called before this function.
 */
extern int Fifo_delete(Fifo_Handle hFifo);

#endif 
