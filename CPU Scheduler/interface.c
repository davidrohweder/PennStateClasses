#include "scheduler.h"

/* 
-------------------------------------
    Start -> Global Defines and Initalizations 
-------------------------------------
*/

pthread_mutex_t serialLock;

pthread_cond_t shortestCPULock;
pthread_cond_t shortestIOLock;
pthread_cond_t threadsInitalized; 
pthread_cond_t staleState; 

bool cpu_syncIsReady = false;
bool io_syncIsReady = false;
bool syncIO = false; // does the cpu need to wait for IO at end
bool cpu_io_sync = false; // just make sure that io prints around same time as cpu
bool p_v_sync = false; // just make sure that p and v print around same time 
bool waitForSecondIOToStart = false;
bool nopBarrier = false;
int offset = 0;

pthread_barrier_t p_v_barrier;
pthread_barrier_t cpu_io_barrier;
pthread_barrier_t cpu_io_PrintBarrier;
pthread_barrier_t cpu_io_StatusBarrier;
pthread_barrier_t second_io_barrier;

Semaphores threadSems[MAX_NUM_SEM - 1]; // P / V sems not for my syncronization

int threads_remaining = 0;
int MAX_THREADS;
bool* threadBehavior;

// Only called by master thread
void init_scheduler(enum sch_type type, int thread_count)
{
    // initalize mutex locks and condition locks
    pthread_mutex_init(&serialLock, NULL); // make sure only one thread is running at a given instance
    pthread_cond_init(&shortestCPULock, NULL); // multiple threads in CPU function
    pthread_cond_init(&shortestIOLock, NULL); // multiple threads in IO function
    pthread_cond_init(&threadsInitalized, NULL);
    pthread_cond_init(&staleState, NULL);

    pthread_barrier_init(&cpu_io_barrier, NULL, 2);
    pthread_barrier_init(&p_v_barrier, NULL, 2);
    pthread_barrier_init(&cpu_io_PrintBarrier, NULL, 2);
    pthread_barrier_init(&cpu_io_StatusBarrier, NULL, 2);
    pthread_barrier_init(&second_io_barrier, NULL, 2);


    // Initalize Semaphores for P and V operations
    for (int i = 0; i < MAX_NUM_SEM; i++) {
        sem_init(&threadSems[i].sem, 0, 0);
        threadSems[i].value = 0;
    }

    // initalize scheduler
    int result = configure_scheduler(thread_count, type);

    if (result == ERR_SCHEDULER) {
        printf("ERROR IN init_scheduler: thread count - %d, sch_type - %d", thread_count, type);
    }

    threadBehavior = calloc(thread_count, sizeof(bool));

    for (int i = 0; i < thread_count; i++) {
        threadBehavior[i] = false; // false means behavior is undefined meaning the system should wait for it
    }

    MAX_THREADS = thread_count;
    threads_remaining = thread_count;
}

/* 
-------------------------------------
    End -> Global Defines and Initalizations 
-------------------------------------
*/


/* 
-------------------------------------
    Start -> Synconization Helper Functions
-------------------------------------
*/

// cpu pause so that IO can catch up
void CPU_IO_Barrier() {

    pthread_barrier_wait(&cpu_io_barrier);

    cpu_syncIsReady = false;
}

void CPU_IO_StatusBarrier() {
    pthread_barrier_wait(&cpu_io_StatusBarrier);
}

// program pause so if IO finished it can print along side appropiate time
void CPU_IO_PrintBarrier() {

    pthread_barrier_wait(&cpu_io_PrintBarrier);

    cpu_io_sync = false;
}

void P_V_Barrier() {

    pthread_barrier_wait(&p_v_barrier);
    
    p_v_sync = false;
}

void standardSync() {
    if (p_v_sync) {
        P_V_Barrier();
    }

    if (cpu_io_sync) {
        CPU_IO_PrintBarrier();
    }

}

void SecondIOBarrier() {
    pthread_barrier_wait(&second_io_barrier);

    waitForSecondIOToStart = false;
}

// how can we compare threads when we some are not ready after initialization? We have an array of states where false is undetermined and true is determined
bool knowAllThreadStates() {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threadBehavior[i] == false) {
            return false;
        }
    }
    return true;
}

/* 
-------------------------------------
    End -> Synconization Helper Functions
-------------------------------------
*/


/* 
-------------------------------------
    Start -> API Interface Functions
    Each thread in thread pool calls these functions simutaneously ...  
-------------------------------------
*/

int cpu_me(float current_time, int tid, int remaining_time)
{

    standardSync();

    // serial add thread to CPU Queue if not already
    pthread_mutex_lock(&serialLock);
    pthread_cond_broadcast(&staleState);
    threadBehavior[tid] = true; // we know the behavior of this thread. { tid is in the CPU }

    // if thread is not in cpu queue, add to queue
    if (!inQueue(tid, true)) {
        int a_result = addToCPUQueue(current_time, tid, remaining_time);
        if (a_result == ERR_SCHEDULER) {
            printf("ERROR -> IN CPU-ME (Op add thread): TID - %d\n", tid);
            return ERR_SCHEDULER;
        }
    }
    
    // keep waiting until all threads have been initalized
    while (!isReady()) {
        pthread_cond_wait(&threadsInitalized, &serialLock); // wait until signal is made that all threads are initalized
    }
    pthread_cond_broadcast(&threadsInitalized);

    // if duration = 0 either thread has finished or it is starting a new burst
    if (!validBurst(tid)) {
        if (getThreadState(tid) == expired) {
            updateThread(current_time, tid, remaining_time); // starting new burst
        } else {
            changeSession(tid, expired); // change tid to expired state 
            threadBehavior[tid] = false;
            pthread_mutex_unlock(&serialLock);

            if (syncIO && io_syncIsReady) {
                CPU_IO_Barrier();
                CPU_IO_StatusBarrier();
            }

            return getSchedulerTime();
        }
    }

    pthread_cond_broadcast(&shortestCPULock); // signal that all threads have been initalized or new burst or that new burst op is occuring
    pthread_cond_broadcast(&shortestIOLock); 

    // keep waiting for lowest
    while (!(tid == lowestCPUArrivalTime(syncIO) && knowAllThreadStates())) {
        changeSession(tid, inWait);

        if (lowestCPUArrivalTime(syncIO) == INT_MAX) {
            // We are in IO and arrival has not been met

            while (!knowAllThreadStates()) {
                pthread_cond_wait(&staleState, &serialLock);
            }
            setSchedulerTime(getSchedulerTime() + 1);
            offset += 1;
            pthread_mutex_unlock(&serialLock);
            CPU_IO_Barrier();
            CPU_IO_StatusBarrier();
            standardSync();
            pthread_mutex_lock(&serialLock);
            pthread_cond_wait(&shortestCPULock, &serialLock);
        } else {
            pthread_cond_wait(&shortestCPULock, &serialLock); // if thread is not the lowest arrival time or if frozen .. wait
        }
    }   

    changeSession(tid, inBurst);
    decrementBurst(tid);

    if ((int) current_time > getSchedulerTime()) {
        setSchedulerTime(current_time + 1);
    } else {
        setSchedulerTime(getSchedulerTime() + 1);
    }

    cpu_syncIsReady = false;
    pthread_mutex_unlock(&serialLock);

    if (syncIO) {
        pthread_mutex_lock(&serialLock);
        cpu_syncIsReady = true;
        pthread_mutex_unlock(&serialLock);
        CPU_IO_Barrier();
        CPU_IO_StatusBarrier();
    }

    pthread_mutex_lock(&serialLock);
    cpu_syncIsReady = false;
    threadBehavior[tid] = false;
    pthread_mutex_unlock(&serialLock);

    return getSchedulerTime();
}

int io_me(float current_time, int tid, int duration)
{
    bool needCPUSync = false;
    bool stillActive = false;

    standardSync();
    
    // stage 1 - add to io queue, update cpu queue that io is happening (if possible), and wait until IO is lowest
    pthread_mutex_lock(&serialLock);
    pthread_cond_broadcast(&staleState);
    threadBehavior[tid] = true; // We know the behavior of this thread. { it is in the IO }

    if (!inQueue(tid, false)) {
        int a_result = addToIOQueue(current_time, tid, duration);
        if (a_result == ERR_SCHEDULER) {
            printf("ERROR -> IN IO-ME (Op add thread): TID - %d\n", tid);
        }
    }

    // update cpu queue; if tid is in cpu queue
    int i_result = setIOStatus(tid, true);
    if (i_result == ERR_SCHEDULER) {
        printf("WARNING -> IN IO-ME (tid not in CPU queue): TID - %d\n", tid);
    }

    while (!isReady()) {
        pthread_cond_wait(&threadsInitalized, &serialLock); // wait until signal is made that all threads are initalized
    }
    pthread_cond_signal(&threadsInitalized);
    
    syncIO = true;

    // keep waiting for lowest _ fcfs scheduling
    while ( (!(tid == lowestIOArrivalTime() && knowAllThreadStates())) && !(tid == lowestIOArrivalTime() && waitForSecondIOToStart) ) {
        pthread_cond_broadcast(&shortestIOLock);
        pthread_cond_broadcast(&shortestCPULock);
        pthread_cond_wait(&shortestIOLock, &serialLock); // if IO is not the lowest arrival time wait
    }

    // make sure that our global time reflects the arrival of IO
    if ((int) current_time > getSchedulerTime()) {
        setSchedulerTime( (int) current_time);
    } else if ( (int) current_time < getSchedulerTime() - 1 ) {
        current_time = getSchedulerTime();
    }

    int targetEnd = (int) current_time + duration + offset;
    offset = 0;
    pthread_cond_broadcast(&shortestCPULock);
    pthread_mutex_unlock(&serialLock);
    
    if (waitForSecondIOToStart) {
        SecondIOBarrier(); // sync with io about to leave function
    }

    // stage 2 - keep waiting or call and resposne until global time is arrival + duration
    while (targetEnd != getSchedulerTime()) {
        pthread_mutex_lock(&serialLock);
        pthread_cond_broadcast(&shortestCPULock);
        while (!knowAllThreadStates()) {
            pthread_cond_wait(&staleState, &serialLock) ;
        }
        pthread_mutex_unlock(&serialLock);

        if (anyThreadsInCPU()) {
            needCPUSync = true;
            io_syncIsReady = true;
            CPU_IO_Barrier();
            io_syncIsReady = false;

            pthread_mutex_lock(&serialLock);
            if (targetEnd != getSchedulerTime()) {
                stillActive = true;
            } else {
                stillActive = false;
            }
            pthread_mutex_unlock(&serialLock);

            if (stillActive) {
                CPU_IO_StatusBarrier();
            }
        } else {
            // special case - no cpu so increment global time
            needCPUSync = false;
            setSchedulerTime(getSchedulerTime() + 1);
        }
    }

    // stage 3 - update everything
    if (cpu_syncIsReady) {
        CPU_IO_Barrier();
    }

    pthread_mutex_lock(&serialLock);
    removeFromIOQueue(tid);

    if (isIOEmpty()) {
        syncIO = false;
    }

    setIOStatus(tid, false);
    pthread_cond_broadcast(&shortestIOLock);
    pthread_cond_broadcast(&shortestCPULock);
    threadBehavior[tid] = false;

    if (needCPUSync) {
        cpu_io_sync = true;
    }

    if (!isIOEmpty()) {
        waitForSecondIOToStart = true;
    }
    pthread_mutex_unlock(&serialLock);

    if (needCPUSync) {
        CPU_IO_StatusBarrier();
    }

    if (waitForSecondIOToStart) {
        SecondIOBarrier(); 
    }
    
    return getSchedulerTime();
}

int P(float current_time, int tid, int sem_id)
{

    // data validation
    if (sem_id > MAX_NUM_SEM || sem_id < 0) {
        return ERR_SCHEDULER;
    }
    
    standardSync();

    // increment sem value at sem_id
    pthread_mutex_lock(&serialLock);
    pthread_cond_broadcast(&staleState);
    threadBehavior[tid] = true; // We know this threads behavior. { its frozen }
    
    // todo add that V shit
    if ((int) current_time > getSchedulerTime()) {
        setSchedulerTime((int) current_time + 1);
    }

    threadSems[sem_id - 1].value += 1;
    
    // wait for sem to be unlocked from V() op
    if (threadSems[sem_id - 1].value > 0) {
        
        int p_result = setFrozenStatus(tid, sem_id, threadSems[sem_id].value); // set thread in queue to frozen
        if (p_result == ERR_SCHEDULER) {
            printf("TID %d, WARNING in P ... thread not in CPU queue\n\n", tid);
        }

        pthread_cond_broadcast(&shortestCPULock);
        pthread_cond_broadcast(&shortestIOLock);
        pthread_mutex_unlock(&serialLock);
        sem_wait(&threadSems[sem_id -1].sem);
        pthread_mutex_lock(&serialLock);
        p_v_sync = true;
    }

    threadBehavior[tid] = false;
    pthread_mutex_unlock(&serialLock);

    return getSchedulerTime();
}

int V(float current_time, int tid, int sem_id)
{    
    // data validation
    if (sem_id > MAX_NUM_SEM || sem_id < 0) {
        return ERR_SCHEDULER;
    }

    standardSync();

    // stage 1 - update sems at sem_id
    pthread_mutex_lock(&serialLock);
    pthread_cond_broadcast(&staleState);
    threadBehavior[tid] = true; // We know this threads behavior. { its unfreezing all tid at sem val }

    while (!isReady()) {
        pthread_cond_wait(&threadsInitalized, &serialLock);
    }
    pthread_cond_broadcast(&threadsInitalized);

    // correct time validation. { we want false or !{false and false} = !T = F to proceed if time is greater and nothig is in io or cpu  }
    while ( (int) current_time > getSchedulerTime() && anyThreadsInCPU() ) {
        pthread_cond_wait(&staleState, &serialLock);
    }

    if ((int) current_time > getSchedulerTime()) {
        setSchedulerTime((int) current_time);
    }

    int u_result = unfreezeThreads(sem_id);
    if (u_result == ERR_SCHEDULER) {
        printf("TID %d, WARNING in V ... thread not in CPU queue\n", tid);
    }

    if (threadSems[sem_id - 1].value > 0) {
        p_v_sync = true;
        sem_post(&threadSems[sem_id - 1].sem);
    }
    
    threadSems[sem_id - 1].value -= 1;

    threadBehavior[tid] = false;
    pthread_mutex_unlock(&serialLock);

    return getSchedulerTime();  
}

// serial remove thread from all queues (if exists)
void end_me(int tid)
{
    bool lastThread = false;

    standardSync();

    pthread_mutex_lock(&serialLock);

    pthread_cond_broadcast(&staleState);
    threadBehavior[tid] = true; // We know the behavior of this thread. { its dead }

    int result_C = removeFromCPUQueue(tid);
    if (result_C == ERR_SCHEDULER) {
        printf("Warning IN END-ME (Remove Thread): TID - %d -> result cpu: %d\n", tid, result_C);
    }

    threads_remaining -= 1;

    // if all queues are empty perform data cleanup
    if (threads_remaining == 0) {
        lastThread = true;
    }

    pthread_mutex_unlock(&serialLock);

    pthread_cond_broadcast(&shortestCPULock); // signal that all threads have been initalized or new burst or that new burst op is occuring
    pthread_cond_broadcast(&shortestIOLock);

    // Cleanup data
    if (lastThread) {
        pthread_mutex_destroy(&serialLock);
        pthread_cond_destroy(&shortestCPULock);
        pthread_cond_destroy(&shortestIOLock);
        pthread_cond_destroy(&staleState);
        
        pthread_barrier_destroy(&second_io_barrier);
        pthread_barrier_destroy(&cpu_io_barrier);
        pthread_barrier_destroy(&p_v_barrier);
        pthread_barrier_destroy(&cpu_io_PrintBarrier);
        pthread_barrier_destroy(&cpu_io_StatusBarrier);

        for (int i = 1; i < MAX_NUM_SEM; i++) {
            sem_destroy(&threadSems[i].sem);
        }

        free(threadBehavior);
    }

}

/* 
-------------------------------------
    End -> API Interface Functions
-------------------------------------
*/