/*
*************************************************
    START -> Includes
*************************************************
*/

#include "interface.h"
#include "vmm.h"

/*
*************************************************
    START -> Includes
*************************************************
*/


/*
*************************************************
    START -> Init
*************************************************
*/
void mm_init(enum policy_type policy, void *vm, int vm_size, int num_frames, int page_size) {

    // Initalize Internal VM
    initVirtualMemory(policy, vm, vm_size, num_frames, page_size);
    
    // Create Frames
    createFrames();
    
    // Register Signals
    signals_register();

}

/*
*************************************************
    End -> Init
*************************************************
*/