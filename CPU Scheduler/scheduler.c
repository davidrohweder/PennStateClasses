 // Package Includes - verbose { scheduler contains data structs (PCB), (QUEUE) -> and functions associated (w prototypes)}
#include "scheduler.h"

/* 
-------------------------------------
    Start -> GLOBAL - And - INITS 
-------------------------------------
*/

// Volatile vars - Get / Read forAll threads in pool
Queue io_queue;
Queue cpu_queue;
int currentThreadCount = 0;
bool initalized = false;
int schedule_type = -5;
int globalTime = 0; // Needed for IO condition to signal it can complete
bool timeInitalized = false;

// Only master thread vars - ReadOnly for thread pool
int thread_count = 0;

// configure our schedule to serialize and maintain the thread pool
int configure_scheduler(int tCount, int schType) {

    if (tCount <= 0 || schType > 3 || schType < 0) {
        return ERR_SCHEDULER;
    } // basic data validation

    // set global params
    thread_count = tCount;
    schedule_type = schType;

    // configure queue streams
    initQueue(&io_queue, thread_count);
    initQueue(&cpu_queue, thread_count);

    return ALERT_SCH_SUCCESS;
}

// If all threads have been fired off - let the games begin
bool isReady() {

    currentThreadCount += 1;

    if (currentThreadCount == thread_count || initalized) {
        initalized = true;
        return true;
    }
    
    return false;
}


/* 
-------------------------------------
    END -> GLOBAL - And - INITS
-------------------------------------
*/ 


/* 
-------------------------------------
    Start -> Basic Functions
-------------------------------------
*/ 

void setSchedulerTime(int time) {
    globalTime = time;
}

int getSchedulerTime() {
    return globalTime;
} 

// intents: return lowest tid according to scheduule policy
int lowestCPUArrivalTime(bool syncIO) {

    switch(schedule_type) {
        case SCH_FCFS: return lowestFCFS(syncIO);
        case SCH_SRTF: return lowestSRFT(syncIO);
        case SCH_MLFQ: return lowestMLFQ(syncIO);
        default:
            return ERR_SCHEDULER;
    }

    return ERR_SCHEDULER;
}

int lowestFCFS(bool syncIO) {

    if (cpu_queue.isEmpty || !isReady()) {
        printf("ERROR -> IN lowestArrivaltime (queue is empty)\n");
        return ERR_SCHEDULER;
    } // will our query be valid?

    Node* tmp = cpu_queue.head_p;
    double lowestArrivalTime = INT_MAX;
    int currentThread = INT_MAX;
    

    // stage 1 - at start of scheduling, set lowest arrival time ...  
    if (!timeInitalized) {
        while(tmp != NULL) {
            if (tmp->threadInfo.arrival_time < lowestArrivalTime && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time and is not frozen
                lowestArrivalTime = tmp->threadInfo.arrival_time;
            }
            tmp = tmp->next;
        }
        setSchedulerTime(lowestArrivalTime);
        timeInitalized = true;
    }

    lowestArrivalTime = INT_MAX;
    tmp = cpu_queue.head_p;

    // stage 2 - find lowest thread 
    while(tmp != NULL) {
        if (tmp->threadInfo.arrival_time < lowestArrivalTime && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time and is not frozen
            if (tmp->threadInfo.arrival_time <= (double) getSchedulerTime() || !syncIO) {
                lowestArrivalTime = tmp->threadInfo.arrival_time;
                currentThread = tmp->threadInfo.tid;
            }
        }
        tmp = tmp->next;
    }

    tmp = cpu_queue.head_p;

    // stage 3 - find lowest tid where time is equal && stil not frozen or in IO
    while(tmp != NULL) {
        if (tmp->threadInfo.arrival_time <= lowestArrivalTime && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time
            if (tmp->threadInfo.tid < currentThread && tmp->threadInfo.arrival_time <= (double) getSchedulerTime()) {
                lowestArrivalTime = tmp->threadInfo.arrival_time;
                currentThread = tmp->threadInfo.tid;
            }
        }
        tmp = tmp->next;
    }

    return currentThread;
}

int lowestSRFT(bool syncIO) {

    if (cpu_queue.isEmpty || !isReady()) {
        printf("ERROR -> IN lowestArrivaltime (queue is empty)\n");
        return ERR_SCHEDULER;
    } // will our query be valid?

    Node* tmp = cpu_queue.head_p;
    int lowestAmountRemaining = INT_MAX;
    double lowestArrivalTime = INT_MAX;
    int currentThread = INT_MAX;

    // stage 1 - at start of scheduling, get lowest arrival time ...  
    if (!timeInitalized) {
        while(tmp != NULL) {
            if (tmp->threadInfo.arrival_time < lowestArrivalTime && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time and is not frozen
                lowestArrivalTime = tmp->threadInfo.arrival_time;
            }
            tmp = tmp->next;
        }
        setSchedulerTime(lowestArrivalTime);
        timeInitalized = true;
    }

    tmp = cpu_queue.head_p;
    
    // stage 1 - find lowest thread 
    while(tmp != NULL) {
        if (tmp->threadInfo.time_remaining < lowestAmountRemaining && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time and is not frozen
            if (tmp->threadInfo.arrival_time <= (double) getSchedulerTime()) {
                lowestAmountRemaining = tmp->threadInfo.time_remaining;
                currentThread = tmp->threadInfo.tid;
            }
        }
        tmp = tmp->next;
    }

    tmp = cpu_queue.head_p;

    // stage 2 - find lowest tid where time is equal && stil not frozen or in IO
    while(tmp != NULL) {
        if (tmp->threadInfo.time_remaining <= lowestAmountRemaining && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time
            if (tmp->threadInfo.tid < currentThread && tmp->threadInfo.arrival_time <= (double) getSchedulerTime()) {
                lowestAmountRemaining = tmp->threadInfo.time_remaining;
                currentThread = tmp->threadInfo.tid;
            }
        }
        tmp = tmp->next;
    }

    return currentThread;
}

int lowestMLFQ(bool syncIO) {

    if (cpu_queue.isEmpty || !isReady()) {
        printf("ERROR -> IN lowestArrivaltime (queue is empty)\n");
        return ERR_SCHEDULER;
    } // will our query be valid?

    Node* tmp = cpu_queue.head_p;
    double lowestArrivalTime = INT_MAX;
    int currentThread = INT_MAX;

    int max_priority = 5;
    int prioritySearch = 0;
    bool found = false;

    if (!timeInitalized) {
        while(tmp != NULL) {
            if (tmp->threadInfo.arrival_time < lowestArrivalTime && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time and is not frozen
                lowestArrivalTime = tmp->threadInfo.arrival_time;
            }
            tmp = tmp->next;
        }
        setSchedulerTime(lowestArrivalTime);
        timeInitalized = true;
    }    

    // loop through all priority levels if no thread is at a priority search at X :: Inner loop is FCFS
    while(!found && prioritySearch < 5) {
        tmp = cpu_queue.head_p;
        lowestArrivalTime = INT_MAX;

        while(tmp != NULL) {
            if (tmp->threadInfo.priority == prioritySearch && tmp->threadInfo.arrival_time < lowestArrivalTime && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time
                if (tmp->threadInfo.arrival_time <= (double) getSchedulerTime() || !syncIO) {
                    lowestArrivalTime = tmp->threadInfo.arrival_time;
                    currentThread = tmp->threadInfo.tid;
                    found = true;
                }
            }
            tmp = tmp->next;
        }

        tmp = cpu_queue.head_p;

        while(tmp != NULL) {
            if (tmp->threadInfo.priority == prioritySearch && tmp->threadInfo.arrival_time <= lowestArrivalTime && !tmp->threadInfo.frozen && !tmp->threadInfo.inIO && tmp->threadInfo.threadState != expired) { // does the thread have the lowest arrival time
                if (tmp->threadInfo.tid < currentThread && tmp->threadInfo.arrival_time <= (double) getSchedulerTime()) {
                    lowestArrivalTime = tmp->threadInfo.arrival_time;
                    currentThread = tmp->threadInfo.tid;
                }
            }
            tmp = tmp->next;
        }
        prioritySearch += 1;
    }

    return currentThread;

}

// note - will always be fcfs
int lowestIOArrivalTime() {
    
    if (io_queue.isEmpty || !isReady()) {
        printf("ERROR -> IN lowestArrivaltime (queue is empty)\n");
        return ERR_SCHEDULER;
    } // will our query be valid?

    Node* tmp = io_queue.head_p;
    double lowestArrivalTime = INT_MAX;
    int currentThread = INT_MAX;
    
    // stage 1 - find lowest thread 
    while(tmp != NULL) {
        if (tmp->threadInfo.arrival_time < lowestArrivalTime) { // does the thread have the lowest arrival time
            lowestArrivalTime = tmp->threadInfo.arrival_time;
            currentThread = tmp->threadInfo.tid;
        }
        tmp = tmp->next;
    }
    
    tmp = io_queue.head_p;

    // stage 2 - find lowest tid where time is equal
    while(tmp != NULL) {
        if (tmp->threadInfo.arrival_time <= lowestArrivalTime) { 
            if (tmp->threadInfo.tid < currentThread) {
                lowestArrivalTime = tmp->threadInfo.arrival_time;
                currentThread = tmp->threadInfo.tid;
            }
        }
        tmp = tmp->next;
    }

    return currentThread;
}

// intents: a burst has occured in the cpu decrement remaining time 
// notes : err checking done several times before : function is certain tid exists
void decrementBurst(int tid) {

    Node* tmp = cpu_queue.head_p;

    while(tmp != NULL) {
        if (tmp->threadInfo.tid == tid) {
            tmp->threadInfo.time_remaining -= 1;

            // mlfq work
            if (schedule_type == SCH_MLFQ) {
                tmp->threadInfo.quantum += 1;
                if (tmp->threadInfo.quantum >= MLFQ_TIME_QUANTUM[tmp->threadInfo.priority]) {
                    tmp->threadInfo.quantum = 0;
                    tmp->threadInfo.priority += 1;
                }
            }

            return;
        }
        tmp = tmp->next;
    }
}

threadState getThreadState(int tid) {
    
    Node* currentNode = cpu_queue.head_p;
    
    while(currentNode != NULL) {
        if (currentNode->threadInfo.tid == tid) {
            return currentNode->threadInfo.threadState;
        }
        currentNode = currentNode->next;
    }
    
    return inBurst; // just a default return
}

void updateThread(double arrival_time, int tid, int duration) {

    Node* currentNode = cpu_queue.head_p;

    while(currentNode != NULL) {
        if (currentNode->threadInfo.tid == tid) {
            currentNode->threadInfo.threadState = inBurst;
            currentNode->threadInfo.arrival_time = arrival_time;
            currentNode->threadInfo.time_remaining = duration;

            // mlfq work
            currentNode->threadInfo.priority = 0;
            currentNode->threadInfo.quantum = 0;
        }
        currentNode = currentNode->next;
    }
}

// the main.c calls while duration >= so check for nop
bool validBurst(int tid) {

    Node* tmp = cpu_queue.head_p;

    while(tmp != NULL) {
        if (tmp->threadInfo.tid == tid) {
            if (tmp->threadInfo.time_remaining != 0) {
                return ALERT_QUE_SUCCESS;
            } else {
                return ERR_QUEUE;
            }
        }
        tmp = tmp->next;
    }
    
    return ERR_QUEUE;
}

// Update Thread State
void changeSession(int tid, threadState state) {

    Node* tmp = cpu_queue.head_p;

    while(tmp != NULL) {
        if (tmp->threadInfo.tid == tid) {
            tmp->threadInfo.threadState = state;
            return;
        }
        tmp = tmp->next;
    }   
}

// note: expired is not an accurate thread in cpu
bool anyThreadsInCPU() {

    Node* currentNode = cpu_queue.head_p;

    while(currentNode != NULL) {
        if (currentNode->threadInfo.threadState == inBurst || currentNode->threadInfo.threadState == inWait) {
            return ALERT_QUE_SUCCESS;
        }
        currentNode = currentNode->next;
    }

    return ERR_QUEUE;
}

// intents: reflect IO status of thread in cpu queue
int setIOStatus(int thread_id, bool setStatus) {

    if (cpu_queue.isEmpty || !isReady()) {
        return ERR_SCHEDULER;
    }

    Node* tmp = cpu_queue.head_p;

    while(tmp != NULL) {
        if (tmp->threadInfo.tid == thread_id) {
            tmp->threadInfo.inIO = setStatus;
            return ALERT_SCH_SUCCESS;
        } else {
            tmp = tmp->next;
        }
    }

    return ERR_SCHEDULER; //  tid not found in io queue to be put in IO state ....
}

int setFrozenStatus(int thread_id, int sem_id, int sem_value) {

    if (cpu_queue.isEmpty || !isReady()) {
        return ERR_SCHEDULER;
    }

    Node* tmp = cpu_queue.head_p;

    while(tmp != NULL) {
        if (tmp->threadInfo.tid == thread_id) {
            tmp->threadInfo.frozen = true;
            tmp->threadInfo.frozenBy = sem_id;
            tmp->threadInfo.frozenAt = sem_value;
            return ALERT_SCH_SUCCESS;
        } else {
            tmp = tmp->next;
        }
    }

    return ERR_SCHEDULER; //  tid not found in cpu queue to be frozen ....
}

// unfreeze all threads that are frozen on sem_id
int unfreezeThreads(int sem_id) {

    if (cpu_queue.isEmpty || !isReady()) {
        return ERR_SCHEDULER;
    }

    Node* tmp = cpu_queue.head_p;

    while(tmp != NULL) {
        if (tmp->threadInfo.frozenBy == sem_id) {
            tmp->threadInfo.frozen = false;
        } 
        tmp = tmp->next;
    }

    return ALERT_SCH_SUCCESS;
}

int addToCPUQueue(double arrivalTime, int tid, int timeRemaining) {

    if (cpu_queue.isFull) {
        printf("ERROR -> IN addToCPUQueue (CPU QUEUE IS FULL) -- Cant Add [tid] = %d\n", tid);
        return ERR_SCHEDULER;
    }

    PCB threadInfo; // create thread information to store in cpu queue

    threadInfo.arrival_time = arrivalTime;
    threadInfo.frozen = false;
    threadInfo.frozenBy = 0;
    threadInfo.inIO = false;
    threadInfo.threadState = inBurst;
    threadInfo.time_remaining = timeRemaining;
    threadInfo.tid = tid;
    threadInfo.priority = 0;
    threadInfo.quantum = 0;

    if (enqueue(&cpu_queue, threadInfo)) {
        ALERT_SCH_SUCCESS;
    } else {
        printf("ERROR -> IN addToCPUQueue (Enqueue failed) -- Cant Add [tid] = %d\n", tid);
    }

    return ALERT_SCH_SUCCESS;
}

int addToIOQueue(double arrivalTime , int tid, int timeRemaining) {

    if (io_queue.isFull) {
        printf("ERROR -> IN addToIOQueue (IO QUEUE IS FULL) -- Cant Add [tid] = %d\n", tid);
        return ERR_SCHEDULER;
    }

    PCB threadInfo; // create thread information to store in io queue

    threadInfo.arrival_time = arrivalTime;
    threadInfo.frozen = false; // DNC IO
    threadInfo.frozenBy = 0; // DNC IO
    threadInfo.inIO = false; // DNC IO
    threadInfo.threadState = inBurst; // DNC IO
    threadInfo.time_remaining = timeRemaining;
    threadInfo.tid = tid;
    threadInfo.priority = 0; // DNC IO
    threadInfo.quantum = 0; // DNC IO

    if (enqueue(&io_queue, threadInfo)) {
        return ALERT_SCH_SUCCESS;
    } else {
        printf("ERROR -> IN addToCPUQueue (Enqueue failed) -- Cant Add [tid] = %d\n", tid);
    }
    return ALERT_SCH_SUCCESS;
}

int removeFromCPUQueue(int tid) {
    
    if (!canDequeue(&cpu_queue, tid) || !isReady()) {
        return ERR_SCHEDULER;
    }

    dequeue(&cpu_queue, tid);

    return ALERT_SCH_SUCCESS;
}

int removeFromIOQueue(int tid) {
    
    if (!canDequeue(&io_queue, tid) || !isReady()) {
        return ERR_SCHEDULER;
    }

    dequeue(&io_queue, tid);

    return ALERT_SCH_SUCCESS;
}


/* 
-------------------------------------
    End -> Basic Functions
-------------------------------------
*/ 


/* 
-------------------------------------
    Start -> Queue Functions *************************************************************************************************************************************
-------------------------------------
*/

// defn: opType = true -> thread in cpu queue; opType = false -> thread in io queue
bool inQueue(int tid, bool opType) {

    Queue* testingQueue = opType ? &cpu_queue : &io_queue; // which queue are we finding the thread with the lowest arrival time?

    if (canDequeue(testingQueue, tid)) {
        return ALERT_QUE_SUCCESS;
    } // canDequeue checks if queues are valid and if tid is present

    return ERR_QUEUE;
}

bool isIOEmpty() {
    if (io_queue.tail_p == NULL) {
        return ALERT_QUE_SUCCESS;
    }
    return ERR_QUEUE;
}

bool isQueuesEmpty() {
    if (cpu_queue.tail_p == NULL && io_queue.tail_p == NULL) {
        return ALERT_QUE_SUCCESS;
    }
    return ERR_QUEUE;
}

void initQueue(Queue *queue, int maxThreads) {
    queue->head_p = NULL;
    queue->tail_p = NULL;
    queue->maxLength = maxThreads;
    queue->isEmpty = true;
    queue->isFull = false;
    queue->length = 0;
}

// add thread to end of queue
bool enqueue(Queue* queue, PCB incommingThread) {

    if (queue->isFull) {
        printf("ERROR -> IN enqueue (Queue at max capacity) -- Cant Add [tid] = %d\n", incommingThread.tid);
        return ERR_QUEUE; // queue already at max capacity
    }

    Node* nextNode = malloc(sizeof(Node));
    nextNode->threadInfo = incommingThread;
    nextNode->next = NULL;

    // if tail exists, connect new node to tail
    if (queue->tail_p != NULL) {
        queue->tail_p->next = nextNode; // set current tail pointer's next node from the new node
    }
    queue->tail_p = nextNode; // set tail to new node 

    // if queue is empty then set the new node to the head
    if (queue->head_p == NULL) {
        queue->head_p = nextNode;
    }
    
    // queue managment
    queue->isEmpty = false;
    queue->length += 1;

    if (queue->maxLength == queue->length) {
        queue->isFull = true; // set queue to max capacity
    }

    return ALERT_QUE_SUCCESS;
}

// check empty or if tid is in queue first, then call dequeue
bool canDequeue(Queue* queue, int tid) {
    if (queue->head_p == NULL || queue->length == 0 || queue->isEmpty) {
        return ERR_QUEUE;
    }

    // if tid exists, because cant dequeue if tid is not in queue
    Node* currentNode = queue->head_p;
    bool found = false; 
    int idx = 0;

    while(!found && idx != queue->maxLength) {
        if (currentNode->threadInfo.tid == tid) {
            return ALERT_QUE_SUCCESS;
        }
        if (currentNode->next == NULL) {
            found = true;
        } else {
            currentNode = currentNode->next;
            idx += 1;
        }
    }

    return ERR_QUEUE; // cant remove from queue as it D.N.E
}

// intents -> dequeue where tid
void dequeue(Queue* queue, int tid) {

    Node* currentNode = queue->head_p;
    Node* previousNode = queue->head_p;
    bool found = false; 
    int idx = 0;

    while(!found && idx != queue->maxLength) {
        if (currentNode->threadInfo.tid == tid) {

            previousNode->next = currentNode->next; // connect the previous node's next node to the current node's next node

            if (currentNode->threadInfo.tid == queue->head_p->threadInfo.tid) {
                queue->head_p = currentNode->next; // if head update
            }
            if (currentNode->next == NULL) {
                queue->tail_p = previousNode; // if removed node is a tail set tail to previous node
                previousNode->next = NULL;
            } 

            free(currentNode);
            found = true;
        } else {
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
        idx += 1;
    }

    if (queue->head_p == NULL) {
        queue->isEmpty = true;
        queue->tail_p = NULL;
    }

    // queue managment
    if (queue->maxLength == queue->length) {
        queue->isFull = false; // set queue to max capacity
    }

    queue->length -= 1;
}

// intents - helper function bc i get confused easily
void printQueue(Queue* queue) {

    Node* currentNode = queue->head_p;
    printf("\n");
    while(currentNode != NULL) {
        printf("%d - ", currentNode->threadInfo.tid);
        currentNode = currentNode->next;
    }
    printf("\n");

}

/* 
-------------------------------------
    End -> Queue Functions *************************************************************************************************************************************
-------------------------------------
*/