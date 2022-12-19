#ifndef SCHEDULER_H
#define SCHEDULER_H


/* 
-------------------------------------
    Start -> Includes 
-------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include "interface.h"

/* 
-------------------------------------
    End -> Includes 
-------------------------------------
*/


/* 
-------------------------------------
    Start -> Defines & Struct Defs. 
-------------------------------------
*/

// Scheduler
#define ALERT_SCH_SUCCESS 5
#define ERR_SCHEDULER -5

// Queue
#define ERR_QUEUE false
#define ALERT_QUE_SUCCESS true

typedef enum {
    inBurst, 
    inWait,
    expired
} threadState;

typedef struct Semaphore {
    int value;
    sem_t sem;
} Semaphores;

typedef struct PCB {
    int tid; // thread id
    double arrival_time; // global time thread started CPU Burst
    int time_remaining; // how many bursts in current burst session
    threadState threadState; // in cpu burst or waiting for new C|X| command
    bool inIO; // indicate wether thread is in io to reflect true lowest
    bool frozen; // is the thread frozen i.e. P()
    int frozenBy; // what sem_id froze thread
    int frozenAt; // what sem_id value froze this thread
    int priority; // mlfq
    int quantum; // mflq
} PCB;

typedef struct Node {
    PCB threadInfo; 
    struct Node *next;
} Node;

typedef struct Queue {
    Node* head_p;
    Node* tail_p;
    int length;
    int maxLength;
    bool isFull; 
    bool isEmpty;
} Queue;

/* 
-------------------------------------
    End -> Defines & Struct Defs. 
-------------------------------------
*/


/* 
-------------------------------------
    Start -> Function Prototypes 
-------------------------------------
*/

// APIS
void* syncronization();

// Queue
void initQueue(Queue*, int);
bool inQueue(int, bool);
bool enqueue(Queue*, PCB);
bool canDequeue(Queue*, int);
bool isIOEmpty();
void dequeue(Queue*, int);
void printQueue(Queue*); // testing purposes

// Scheduler
bool isQueuesEmpty();
int configure_scheduler(int, int);
bool isReady();
int lowestCPUArrivalTime(bool);
int lowestFCFS(bool);
int lowestSRFT(bool);
int lowestMLFQ(bool);
int lowestIOArrivalTime();
bool validBurst(int);
void decrementBurst(int);
threadState getThreadState(int);
bool anyThreadsInCPU();
void updateThread(double, int, int);
void changeSession(int, threadState);
int setIOStatus(int, bool);
int setFrozenStatus(int, int, int);
int unfreezeThreads(int);
bool isFrozen(int);
int addToCPUQueue(double, int, int);
int addToIOQueue(double, int, int);
int removeFromCPUQueue(int);
int removeFromIOQueue(int);
void setSchedulerTime(int);
int getSchedulerTime();

/* 
-------------------------------------
    End -> Function Prototypes 
-------------------------------------
*/


#endif