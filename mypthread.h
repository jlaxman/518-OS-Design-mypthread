// File:    mypthread_t.h

// List all group members' names: 
// iLab machine tested on:

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE

/* in order to use the built-in Linux pthread library as a control for benchmarking, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>

typedef uint mypthread_t;
typedef enum status{
    New,
    Ready, //ready to be scheduled
    Running, //is currently running
    Wait, //waiting to be scheduled
    Yield, //yielding for another thread
    Terminated //no longer running
} status;
    /* add important states in a thread control block */
typedef struct threadControlBlock
{
    // YOUR CODE HERE
    mypthread_t thr_id;
    status thr_status;
    ucontext_t thr_context;
    void * stack;
    int thr_priority;
    void * return_val;
    // thread Id
    // thread status
    // thread context
    // thread stack
    // thread priority
    // And more ...
} tcb;

typedef struct queue_node{
    tcb * thread_tcb;
    struct queue_node * next;
}queue_node;

typedef struct queue{
    queue_node * head;
    queue_node * tail;
    int size;
}queue;
/* mutex struct definition */
typedef struct mypthread_mutex_t
{
    int lock;
    queue *wait_queue;
    // YOUR CODE HERE
    
} mypthread_mutex_t;


// Feel free to add your own auxiliary data structures (linked list or queue etc...)


typedef struct scheduler{
    queue * prio_queue; //priority queue
    tcb * thr_main; //main context
    tcb * thr_current; //current context
    int pri[16]; //priority list
    int sched_count;
}scheduler;


/* Function Declarations: */

/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void (*function)(void), void * arg);

/* current thread voluntarily surrenders its remaining runtime for other threads to use */
int mypthread_yield();

/* terminate a thread */
void mypthread_exit(void *value_ptr);

/* wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr);

/* initialize a mutex */
int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

/* aquire a mutex (lock) */
int mypthread_mutex_lock(mypthread_mutex_t *mutex);

/* release a mutex (unlock) */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex);

/* destroy a mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex);
tcb * dequeue(queue *que);
void initialize_queue(queue *que);
bool IsEmpty(queue * que);
tcb * peekOfQueue(queue * que);
queue_node * initialize_queue_node();
void enqueue(queue *que, tcb * thread_node);
void find_and_halt ();
void signal_handler(int signal);
void thread_execute(tcb * new_thr_node, void *(*f)(void *), void * arg);
void start_timer();
void start_schedule();
void sched_add(tcb * new_thr_node, int node_priority);
tcb * select_schedule();
void handle_schedule();


#ifdef USE_MYTHREAD
#define pthread_t mypthread_t
#define pthread_mutex_t mypthread_mutex_t
#define pthread_create mypthread_create
#define pthread_exit mypthread_exit
#define pthread_join mypthread_join
#define pthread_mutex_init mypthread_mutex_init
#define pthread_mutex_lock mypthread_mutex_lock
#define pthread_mutex_unlock mypthread_mutex_unlock
#define pthread_mutex_destroy mypthread_mutex_destroy
#endif

#endif
