/*
*************************************************
    START -> Includes and macros
*************************************************
*/

#ifndef VMM_H
#define VMM_H
#include "interface.h"

/*
*************************************************
    END -> Includes and macros
*************************************************
*/


/*
*************************************************
    START -> Function Prototypes
*************************************************
*/

typedef enum {
    READ,
    WRITE,
    READONLY
} SegErr;

typedef struct Frame {
    int page_occupied;
    bool write_back; // also m bit
    int occupied_when;
    int r;
    bool given_third;
    bool carryOverThird;
} Frame;

typedef struct EventData {
    int incident_page;
    int incident_diff;
    SegErr event_cause;
    
} EventData;

/*
*************************************************
    START -> Function Prototypes
*************************************************
*/


/*
*************************************************
    START -> Function Prototypes
*************************************************
*/

// init
void initVirtualMemory(enum policy_type, void*, int, int, int);
void createFrames();

// signals
void signals_register();
void handle_SIGSEGV(int sig, siginfo_t*, void*);

// policies
void handle_policy(EventData);
int add_page(EventData);
int fifo_eviction(EventData, int*, bool*, int*);
int tc_eviction(EventData, int*, int*, bool*, int*);
bool hasFreeFrames();
void evict_page(int);


// linked list management
bool maxFrames();
void addFrame();
void removeFrame();

/*
*************************************************
    End -> Function Prototypes
*************************************************
*/



#endif
