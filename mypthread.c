
// List all group members' names: Vennela Chava(vc494) Sai Laxman Jagarlamudi(sj1018)
// iLab machine tested on:

#include "mypthread.h"

// INITIALIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
#define STACK_SIZE 6400
#define noOf_levels 3
static scheduler* sch;
int sched_started = 0; //schedule is not yet started
ucontext_t main_context;
tcb main_process;
struct itimerval timer_value;
int LOCKED=1;
int UNLOCKED=2;
int thread_init_id=0;
/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void (*function)(void), void * arg)
{
    void (*threadExit) (void *) = &mypthread_exit;
    ucontext_t *thread_exit = (ucontext_t *) malloc(sizeof(ucontext_t));
    getcontext(thread_exit);
    thread_exit->uc_link=0;
     thread_exit->uc_stack.ss_sp=malloc(STACK_SIZE);
     thread_exit->uc_stack.ss_size=STACK_SIZE;
     thread_exit->uc_stack.ss_flags=0;
     makecontext(thread_exit, (void*)threadExit, 1, NULL);
    if(sched_started==0){
        start_schedule();
        sched_started=1;
        start_timer();
    }
    tcb new_tcb_check=(tcb)malloc(sizeof(tcb));
    new_tcb_check->thr_id = *thread;
    if(getcontext(&(new_tcb_check->thr_context)) == -1) {
        printf("Error: getcontext returned -1\n");
        return -1;
    }
    *thread=thread_init_id;
    tcb new_tcb=(tcb)malloc(sizeof(tcb));
    //ucontext_t contextNew=(ucontext_t)malloc(sizeof(ucontext_t));
    new_tcb->thr_context.uc_stack.ss_size = STACK_SIZE;
    new_tcb->thr_context.uc_stack.ss_sp = malloc(STACK_SIZE);
    new_tcb->thr_context.uc_link = &main_context; //if thread is done it goes to main
    makecontext(&(new_tcb->thr_context), (void *)thread_execute, 3, new_tcb, function, arg);
    thread_init_id=thread_init_id+1;
    add_scheduler(new_tcb, 0);//we are adding the thread to the scheduler
    sch->thr_current = NULL;
    handle_schedule();
    // create a Thread Control Block
    // create and initialize the context of this thread
    // allocate heap space for this thread's stack
    // after everything is all set, push this thread into the ready queue
    return 0;
};

/* current thread voluntarily surrenders its remaining runtime for other threads to use */
int mypthread_yield()
{
    // YOUR CODE HERE
    sch->thr_current->thr_status = Yield; //changing the status to yield
    handle_schedule();
    // change current thread's state from Running to Ready
    // save context of this thread to its thread control block
    // switch from this thread's context to the scheduler's context
    return 0;
};

/* terminate a thread */
void mypthread_exit(void *value_ptr){
    // YOUR CODE HERE
    sch->thr_current->thr_status = Terminated;
    sch->thr_current->return_val = value_ptr;
    swapcontext(&sch->thr_current->thr_context, &main_process.thr_context);
    // preserve the return value pointer if not NULL
    // deallocate any dynamic memory allocated when starting this thread
    return;
};


/* Wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr){
    // YOUR CODE HERE
    if(sch->thr_current->thr_id == thread){
        while(sch->thr_current->thr_status != Terminated){
            mypthread_yield();
        }
        sch->thr_current->return_val = value_ptr;
        free(sch->thr_current->thr_context.uc_stack.ss_sp);
    }
     // supposed to free the stack (look at line 78 in pthread_create)
    // wait for a specific thread to terminate
    // deallocate any dynamic memory created by the joining thread

    return 0;
};

/* initialize the mutex lock */
int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr){
    // YOUR CODE HERE
    if(mutex == NULL){
        printf("invalid mutex");
        exit(EXIT_FAILURE);
    }
    mutex->lock = UNLOCKED;
    mutex->wait_queue = (queue*) malloc(sizeof(queue));
    initialize_queue(mutex->wait_queue);
    //initialize data structures for this mutex
    return 0;
};

/* aquire a mutex lock */
int mypthread_mutex_lock(mypthread_mutex_t *mutex)
{
        // YOUR CODE HERE
        if (mutex==NULL){
            printf("invalid mutex");
            exit(EXIT_FAILURE);
        }
        while(__sync_lock_test_and_set(&(mutex->lock), LOCKED)){
            sch->thr_current->thr_status = Wait;
            enqueue(mutex->wait_queue, sch->thr_current);
            handle_schedule();
        }
        // use the built-in test-and-set atomic function to test the mutex
        // if the mutex is acquired successfully, return
        // if acquiring mutex fails, put the current thread on the blocked/waiting list and context switch to the scheduler thread
        return 0;
};

/* release the mutex lock */
int mypthread_mutex_unlock(mypthread_mutex_t * mutex)
{
    // YOUR CODE HERE
    if (mutex==NULL){
        printf("invalid mutex");
        exit(EXIT_FAILURE);
    }
    if(mutex->wait_queue->size != 0){
        tcb * holder = dequeue(mutex->wait_queue);
        add_scheduler(holder, holder->thr_priority);
    }
    __sync_lock_test_and_set(&(mutex->lock), UNLOCKED);
    // update the mutex's metadata to indicate it is unlocked
    // put the thread at the front of this mutex's blocked/waiting queue in to the run queue
    return 0;
};


/* destroy the mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex)
{
    // YOUR CODE HERE
    mutex->lock = UNLOCKED;
    if(mutex == NULL){
        printf("invalid mutex");
        exit(EXIT_FAILURE);
    }
    free(mutex->wait_queue);
    // deallocate dynamic memory allocated during mypthread_mutex_init
    return 0;
};

/* scheduler */
static void schedule()
{
    // YOUR CODE HERE
    
    // each time a timer signal occurs your library should switch in to this context
    
    // be sure to check the SCHED definition to determine which scheduling algorithm you should run
    //   i.e. RR, PSJF or MLFQ

    return;
}

/* Round Robin scheduling algorithm */
static void sched_RR()
{
    // YOUR CODE HERE
    
    // Your own implementation of RR
    // (feel free to modify arguments and return types)
    
    return;
}

/* Preemptive PSJF (STCF) scheduling algorithm */
static void sched_PSJF()
{
    // YOUR CODE HERE

    // Your own implementation of PSJF (STCF)
    // (feel free to modify arguments and return types)

    return;
}

/* Preemptive MLFQ scheduling algorithm */
/* Graduate Students Only */
static void sched_MLFQ() {
    // YOUR CODE HERE
    
    // Your own implementation of MLFQ
    // (feel free to modify arguments and return types)

    return;
}

// Feel free to add any other functions you need

// YOUR CODE HERE
void find_and_halt (){};

void signal_handler(int signal){
    handle_schedule(); //hopefully this will handle the thread swapping
}
void thread_execute(tcb * new_thr_node, void *(*function)(void *), void * arg){
    new_thr_node->thr_status = Running;
    sch->thr_current = new_thr_node;
    new_thr_node = function(arg);
    new_thr_node->thr_status = Terminated;
    handle_schedule();
}
void start_timer(){
    signal(SIGALRM, signal_handler);
    timer_value.it_interval.tv_sec = 0;
    timer_value.it_interval.tv_usec = 5000;
    timer_value.it_value.tv_sec = 0; //it_value if set to zero disables the alarm
    timer_value.it_value.tv_usec = 5000; //it_value if not zero enables the alarm
    setitimer(ITIMER_REAL, &timer_value, NULL);//starting the itimer
}
void initialize_queue(queue *que) {
    que->head=NULL;
    que->tail=NULL;
    que->size=0;
}
bool IsEmpty(queue * que){
    if (que->size == 0) {
        return true;
    }
    return false;

}
tcb * peekOfQueue(queue * que){
    tcb *thr_peek = NULL;
    if (IsEmpty(que)){ //checking if the queue is empty
        printf("queue is empty");
        return thr_peek;
    }
    thr_peek = (que -> head) -> thread_tcb;
    return thr_peek; //returns the head of the queue
    
}
queue_node * initialize_queue_node() {
    queue_node *new_node = (queue_node *)malloc(sizeof(queue_node));
    if (new_node == NULL){ //checking if the malloc allocated properly
        printf("Error in initializing queue node");
        exit(EXIT_FAILURE);
    }
    if (new_node != NULL){
        new_node-> thread_tcb = NULL;
        new_node-> next = NULL;
        return new_node;
    }
    return NULL;
}
void enqueue(queue *que, tcb * thread_node) {
    if (IsEmpty(que)){ //checking if the queue is empty
        queue_node * queue_head = initialize_queue_node(); //the given node becomes the head of the queue
        que -> head = queue_head;
        que -> tail = queue_head;
        queue_head -> thread_tcb = thread_node;
        (que -> size)++;
    }
    else {
        queue_node * queue_end = que -> tail; //adding the node at the end of the queue
        queue_node * new_tail = initialize_queue_node();
        new_tail -> thread_tcb = thread_node;
        new_tail -> next = NULL;
        queue_end->next = new_tail;
        que -> tail = new_tail -> next; //making the new node to be the tail
        (que -> size)++;
    }
}
tcb * dequeue(queue *que) {
    tcb * thr_dequeue = NULL;
    if (IsEmpty(que)){ //checking if the queue is empty
        fprintf(stderr, "Error : the list is empty, nothing to dequeue\n");
        return thr_dequeue;
    }
    thr_dequeue = (que -> head) -> thread_tcb; //removing the node from front of the queue
    queue_node *node_q = (que -> head) -> next;
    free(que -> head);
    que -> head = node_q;
    (que -> size)--;
    return thr_dequeue;
}
void start_schedule(){
        if(sched_started != 0){
            return;
        }
        sch = malloc(sizeof(sch));
        getcontext(&main_context); //has main, getting the main context for reference
        main_process.thr_context = main_context;
        main_process.thr_context.uc_link = NULL;
        main_process.thr_status = Ready;
        sch->thr_main = &main_process;
        sch->thr_main->thr_status = Ready;
        sch->thr_main->thr_id = 0;
        sch->thr_current = NULL;
        sch->prio_queue = (queue *)malloc(sizeof(queue)*noOf_levels);
        for(int i = 0; i < noOf_levels; i++){
          initialize_queue(&(sch->prio_queue[i])); //we are initializing
        }
}
void add_scheduler(tcb * new_thr_node, int node_priority){
    new_thr_node->thr_status = Ready;
    new_thr_node->thr_priority = node_priority;
    enqueue(&(sch->prio_queue[node_priority]), new_thr_node);
}
tcb * select_schedule(){
    for(int p = 0; p<noOf_levels; p++){
        if(sch->prio_queue[p].head != NULL){
            return dequeue(&(sch->prio_queue[p]));
        }
    }
    return NULL;
}
void handle_schedule() {
    struct sigaction action_setup;
    sigset_t blocker;
    sigemptyset (&blocker);
    sigaddset (&blocker, SIGALRM);
    sigaddset (&blocker, SIGQUIT);
    action_setup.sa_handler = find_and_halt;
    action_setup.sa_mask = blocker;
    action_setup.sa_flags = 0;
    sigaction (SIGALRM, &action_setup, NULL);
    tcb * curr_tcb = sch->thr_current;
    if(curr_tcb != NULL){
        if(curr_tcb->thr_status == Yield){
            add_scheduler(curr_tcb, curr_tcb->thr_priority); //placing in scheduler
        }
        else if(curr_tcb->thr_status == Terminated){}
        else if(curr_tcb->thr_status == Wait){}
        else{
            int new_priority; //thread is running setting new priority
            if((curr_tcb->thr_priority)+1 > noOf_levels-1){
                new_priority = noOf_levels-1;
            }
            else{
                new_priority = (curr_tcb->thr_priority)+1;
            }
            add_scheduler(curr_tcb, new_priority);
        }
    }
    sch->thr_current = select_schedule(); //select another thread to run
    if(sch->thr_current == NULL){
        thread_execute(&main_process, NULL, NULL);
    }
    sch->thr_current->thr_status = Running; //swap the thread with selected thread
    if(curr_tcb != NULL){
        swapcontext(&(curr_tcb->thr_context), &(sch->thr_current->thr_context));
    }
    else {
        swapcontext(&main_context, &(sch->thr_current->thr_context));
    }
    sigemptyset(&blocker);
}
