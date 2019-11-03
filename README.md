#include <stdlib.h>   
#include <pthread.h>   
#include <assert.h>   
  
#ifdef _WIN32   
#   include <windows.h>   
#   define SLEEP(ms) Sleep(ms)   
#elif defined(LINUX)   
#   include <unistd.h>   
#   define SLEEP(ms) sleep(ms)   
#endif   
  
#define QUEUE_SIZE  10   
#define LOOP        20   
  
void* producer (void *args);  
void* consumer (void *args);  
  
typedef struct  
{  
    int buf[QUEUE_SIZE];  
    long head, tail;  
    bool full, empty;  
    pthread_mutex_t *mutex;  
    pthread_cond_t *notFull, *notEmpty;  
} Queue;  
  
Queue*  queueInit (void);  
void    queueDelete (Queue *q);  
void    queueAdd (Queue *q, int in);  
void    queueDel (Queue *q, int *out);  
  
int main(int argc, char* argv[])  
{  
    Queue* fifo = queueInit ();  
    assert(fifo !=  NULL);  
  
    pthread_t pro, con;  
    pthread_create (&pro, NULL, &producer, fifo);  
    pthread_create (&con, NULL, &consumer, fifo);  
    pthread_join (pro, NULL);  
    pthread_join (con, NULL);  
    queueDelete (fifo);  
    return 0;  
}  
  
void* producer (void *q)  
{  
    Queue* fifo = (Queue *)q;  
  
    for (int i = 0; i < LOOP; i++)  
    {  
        // 临界区操作：若队列未满，添加新数据   
        pthread_mutex_lock (fifo->mutex);  
        while (fifo->full)  
        {  
            printf ("producer: Queue FULL.\n");  
            pthread_cond_wait (fifo->notFull, fifo->mutex);  
        }  
        queueAdd (fifo, i);  
        pthread_mutex_unlock (fifo->mutex);  
  
        // 数据添加结束，发“队列有数据”信号   
        pthread_cond_signal (fifo->notEmpty);  
        SLEEP (100);  
    }  
  
    // 与上面类似   
    for (int i = 0; i < LOOP; i++)  
    {  
        pthread_mutex_lock (fifo->mutex);  
        while (fifo->full)  
        {  
            printf ("producer: Queue FULL.\n");  
            pthread_cond_wait (fifo->notFull, fifo->mutex);  
        }  
        queueAdd (fifo, i);  
        pthread_mutex_unlock (fifo->mutex);  
        pthread_cond_signal (fifo->notEmpty);  
        SLEEP (200);  
    }  
    return (NULL);  
}  
  
void* consumer (void *q)  
{  
    int d;  
  
    Queue *fifo = (Queue *)q;  
  
    for (int i = 0; i < LOOP; i++)  
    {  
        // 临界区操作：若队列不空，则取出数据   
        pthread_mutex_lock (fifo->mutex);  
        while (fifo->empty)  
        {  
            printf ("consumer: Queue EMPTY.\n");  
            pthread_cond_wait (fifo->notEmpty, fifo->mutex);  
        }  
        queueDel (fifo, &d);  
        pthread_mutex_unlock (fifo->mutex);  
  
        // 取完数据，发“队列不满”信号   
        pthread_cond_signal (fifo->notFull);  
        printf ("consumer: recieved %d.\n", d);  
        SLEEP(200);  
    }  
  
    // 与上面类似   
    for (int i = 0; i < LOOP; i++)  
    {  
        pthread_mutex_lock (fifo->mutex);  
        while (fifo->empty)  
        {  
            printf ("consumer: Queue EMPTY.\n");  
            pthread_cond_wait (fifo->notEmpty, fifo->mutex);  
        }  
        queueDel (fifo, &d);  
        pthread_mutex_unlock (fifo->mutex);  
        pthread_cond_signal (fifo->notFull);  
        printf ("consumer: recieved %d.\n", d);  
        SLEEP (50);  
    }  
    return (NULL);  
}  
  
Queue *queueInit (void)  
{  
    Queue *q = (Queue *)malloc (sizeof (Queue));  
    if (q == NULL) return (NULL);  
  
    q->empty = true;  
    q->full = false;  
    q->head = 0;  
    q->tail = 0;  
  
    q->mutex = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));  
    pthread_mutex_init (q->mutex, NULL);  
    q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));  
    pthread_cond_init (q->notFull, NULL);  
    q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));  
    pthread_cond_init (q->notEmpty, NULL);  
      
    return (q);  
}  
  
void queueDelete (Queue *q)  
{  
    pthread_mutex_destroy (q->mutex);  
    free (q->mutex);   
  
    pthread_cond_destroy (q->notFull);  
    free (q->notFull);  
  
    pthread_cond_destroy (q->notEmpty);  
    free (q->notEmpty);  
  
    free (q);  
}  
  
void queueAdd (Queue *q, int in)  
{  
    q->buf[q->tail] = in;  
    q->tail++;  
    if (q->tail == QUEUE_SIZE)   // 循环队列   
        q->tail = 0;  
  
    if (q->tail == q->head)       // 添加数据时“触顶”   
        q->full = true;  
    q->empty = false;  
  
    return;  
}  
  
void queueDel (Queue *q, int *out)  
{  
    *out = q->buf[q->head];  
    q->head++;  
    if (q->head == QUEUE_SIZE)   // 循环队列   
        q->head = 0;  
  
    if (q->head == q->tail)       // 取出数据时“触底”   
        q->empty = true;  
    q->full = false;  
  
    return;  
}
