/*
*************************************************
    START -> Includes and globals
*************************************************
*/

#include "vmm.h"

// SIG ERROR DEFINES
// SYS_WRTIE == 6
// SYS_READ == 8
// SYS_READONLY == 7

// given
enum policy_type policy = MM_FIFO;
void *vm = NULL;
int vm_size = 0;
int num_frames = 0;
int page_size = 0;

// maintain
int global_counter = 0;
Frame* frames = NULL;
bool freeFrames = true;
int victim_frame = 0; // tc

/*
*************************************************
    END -> Includes and globals
*************************************************
*/


/*
*************************************************
    START -> Init
*************************************************
*/

void initVirtualMemory(enum policy_type send_policy, void *send_vm, int send_vm_size, int send_num_frames, int send_page_size) {
    
    // set my bois up
    policy = send_policy;
    vm = send_vm;
    vm_size = send_vm_size;
    page_size = send_page_size;
    num_frames = send_num_frames;

    // initalize all pages to no access to cause a faulty faulty >:)
    if (mprotect(vm, vm_size, PROT_NONE) == -1) {
        printf("mprotect, inital failed\n");
    }

}

void createFrames() {

    frames = (Frame*)malloc(sizeof(Frame) * num_frames);

    for (int i = 0; i < num_frames; i++) {
        frames[i].occupied_when = -1;
        frames[i].page_occupied = -1;
        frames[i].write_back = false; // also m in TC
        frames[i].r = 0; // inital state for TC
        frames[i].given_third = false;
        frames[i].carryOverThird = false;
    }

}

/*
*************************************************
    END -> Init
*************************************************
*/


/*
*************************************************
    START -> Signals
*************************************************
*/

void signals_register() {
    struct sigaction sa;
    sa.sa_handler = &handle_SIGSEGV;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
}

void handle_SIGSEGV(int sig, siginfo_t* siginfo, void* context) {
    
    // get addr to get page
    int incident_addr = siginfo->si_addr - vm;
    EventData event; 
    event.incident_page = incident_addr / page_size;
    event.incident_diff = (incident_addr % page_size);

    // determine if fault was write(1), read(2) or readOnly(3)
    ucontext_t* const contextErr = (ucontext_t* const)context;

    if (contextErr->uc_mcontext.gregs[REG_ERR] == 6) {
        event.event_cause = WRITE;
    } else if (contextErr->uc_mcontext.gregs[REG_ERR] == 4) {
        event.event_cause = READ;
    } else {
        event.event_cause = READONLY;
    }

    handle_policy(event);

}

/*
*************************************************
    END -> Signals
*************************************************
*/


/*
*************************************************
    Start -> Eviction Policies
*************************************************
*/

void handle_policy(EventData send_event) {

    EventData event = send_event;
    bool write_back = false;
    int oldest_page = -1;
    int free_frame = 0;
    int tc_res = -1; // 0 == track read && 1 == track write

    // Check for free frames
    if (hasFreeFrames(event)) {
        free_frame = add_page(event);
    } else {
        int evicted = policy == MM_FIFO ? fifo_eviction(event, &oldest_page, &write_back, &free_frame) : tc_eviction(event, &tc_res, &oldest_page, &write_back, &free_frame);

        // Update frame
        if (evicted == -1) {
            oldest_page = -1;
        }

    }

    // update VM page to propert protect
    if (event.event_cause == WRITE || event.event_cause == READONLY) {
        mprotect(vm + (page_size * event.incident_page), page_size, (PROT_READ | PROT_WRITE));
    } else {
        mprotect(vm + (page_size * event.incident_page), page_size, PROT_READ);
    }
    
    unsigned int physical_addr = (page_size * free_frame) + event.incident_diff;

    if (tc_res != -1) {
        event.event_cause = tc_res;
    }

    mm_logger(event.incident_page, event.event_cause, oldest_page, write_back, physical_addr);
}

int add_page(EventData event) {
    
    for (int i = 0; i < num_frames; i++) {
        if (frames[i].occupied_when == -1) {
            frames[i].page_occupied = event.incident_page;
            frames[i].write_back = (event.event_cause == WRITE) ? true : (event.event_cause == READONLY ? true : false);
            frames[i].occupied_when = global_counter;
            frames[i].r = 1;
            global_counter += 1;
            return i;
        }
    }

}

// intents: if current page exists return -1, if not return oldest page and wether it needs to be written back
int fifo_eviction(EventData event, int* oldest_page, bool* write_back, int* free_frame) {
    int lowest_counter = INT_MAX;

    for (int i = 0; i < num_frames; i++) {
        if (event.incident_page == frames[i].page_occupied) {
            // page is in frames no need to evict; however need to update
            frames[i].write_back = (event.event_cause == WRITE) ? true : (event.event_cause == READONLY ? true : false);
            *write_back = false;
            *free_frame = i;
            return -1; // not evicting so oldest page is -1
        }

        if (frames[i].occupied_when < lowest_counter) {
            *oldest_page = frames[i].page_occupied;
            lowest_counter = frames[i].occupied_when;
            *write_back = frames[i].write_back;
            *free_frame = i;
        }
    }

    evict_page(*oldest_page);

    frames[*free_frame].page_occupied = event.incident_page;
    frames[*free_frame].occupied_when = global_counter;
    frames[*free_frame].write_back = (event.event_cause == WRITE) ? true : (event.event_cause == READONLY ? true : false);
    global_counter += 1;

    return 1;
}

int tc_eviction(EventData event, int* tc_res, int* oldest_page, bool* write_back, int* free_frame) {

    int start = victim_frame; // todo maybe point another??

    // update if exists
    for (int i =0; i < num_frames; i++) {
        if (event.incident_page == frames[i].page_occupied) {
            // check for tracked
            if (frames[i].r == 0) {
                if (frames[i].write_back && (event.event_cause == WRITE || event.event_cause == READONLY)) {
                    *tc_res = 4;
                } else if (event.event_cause == READ) {
                    *tc_res = 3;
                }
            }

            if (!frames[i].write_back && (event.event_cause == WRITE || event.event_cause == READONLY)) {
                *tc_res = 2;
            }
            
            frames[i].write_back = (event.event_cause == WRITE) ? true : (event.event_cause == READONLY ? true : false);
            frames[i].r = 1;
            frames[i].carryOverThird = frames[i].given_third;
            frames[i].given_third = false;

            *write_back = 0;
            *free_frame = i;

            return -1;
        }
    }

    // we will loop until we fi./tnd a canidate
    while (true) {
        
        for (int i = start; i < num_frames; i++) {
            
            if (frames[i].r == 0 && frames[i].carryOverThird) {
                frames[i].carryOverThird = false;
                frames[i].given_third = true;
                continue;
            }

            // case A: evict NOW, (R,M) = (0,0) or (given_third == true)
            if ((frames[i].r == 0 && frames[i].write_back == false) || frames[i].given_third == true) {
                evict_page(frames[i].page_occupied);
                victim_frame = i + 1 < num_frames ? i + 1 : 0;

                *write_back = frames[i].given_third; 
                *oldest_page = frames[i].page_occupied;
                *free_frame = i;

                frames[i].carryOverThird = false;
                frames[i].page_occupied = event.incident_page;
                frames[i].write_back = (event.event_cause == WRITE) ? true : (event.event_cause == READONLY ? true : false);
                frames[i].r = 1;
                frames[i].given_third = false;
                
                return 1;
            }

            // case D: third chance -> set third chance to true
            if (frames[i].r == 0 && frames[i].write_back == true) {
                frames[i].given_third = true;
            }

            // case B&c: if r = 1 
            if (frames[i].r == 1) {
                frames[i].r = 0;
                evict_page(frames[i].page_occupied); // no protection to cause a fault
            }

        }

        start = 0;
    }

}

bool hasFreeFrames(EventData event) {

    // check if page is already within frames
    for (int i = 0; i < num_frames; i++) {
        if (frames[i].page_occupied == event.incident_page) {
            return false;
        }
    }

    // check if any page is free
    for (int i = 0; i < num_frames; i++) {
        if (frames[i].page_occupied == -1) {
            return true;
        }
    }

    return false;
}

void evict_page(int page) {
    mprotect(vm + (page_size * page), page_size, PROT_NONE);
}

/*
*************************************************
    End -> Eviction Policies
*************************************************
*/