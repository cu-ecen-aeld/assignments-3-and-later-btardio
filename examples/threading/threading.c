#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)


/* msleep(): Sleep for the requested number of milliseconds. */
// https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
int msleep(long msec)
{
    struct timespec ts;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    return nanosleep(&ts, &ts);

}


void* threadfunc(void* thread_param)
{
    // todo: does this get deallocated
    struct thread_data* thread_p = (struct thread_data *) thread_param;

    //printf("%lu\n", thread_p->thread_id);
    if (msleep(thread_p->wait_to_obtain_ms) != 0) {
        perror("sleep failed\n");
    }
    
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
//    free(thread_p);
//    return NULL;
    return thread_param;
    
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{

    struct thread_data *data;
    void* t;
    data = malloc(sizeof(struct thread_data));
    if (data == NULL) {
        perror("calloc error\n");
    }

    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->thread_complete_success = false;
    data->thread_id = thread;
    pthread_create(
                data->thread_id,
                NULL,
                threadfunc,
                (void*) data
            );
    pthread_join(*data->thread_id, &t); //&data->thread_id, &t);
    
    free(t);

    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    //assert (rval == (void*)&data->thread_id);
//    if(data != NULL){
//        free(data);
//    }
    return false;
}

