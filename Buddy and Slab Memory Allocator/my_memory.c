#include "my_memory.h"

/*
***************************
    Start -> GLOBAL & Inits
***************************
*/

enum malloc_type alloc_type = MALLOC_BUDDY;
int highestPower = 0;
int lowestPower = 0;
int powerRange = 0;
PowerList* powers = NULL;
SlabList slabs;

void mem_init(enum malloc_type type, int given_ram_block) {
    alloc_type = type;
    calcGlobalPowers();

    int try = initPowerList();

    if (try == MALLOC_ERROR) {
        printf("Error in mem_init\n");
    }

    initSlabList();
}

int initPowerList() {
    powers = (PowerList*)malloc(sizeof(PowerList) * (powerRange + 1));
    
    if (powers == NULL) {
        return MALLOC_ERROR;
    } 

    for (int i = 0; i < powerRange + 1; i++) {
        powers[i].head_p = NULL;
        int tmp_pow = i + lowestPower;
        powers[i].power_num = tmp_pow;
        powers[i].size = (int) pow(2, tmp_pow);
    }

    // Add hole in highest power 
    Buddy noFriend;
    noFriend.buddy_address = 10;
    noFriend.power = highestPower;
    int r = addHole(powerRange, 0, MEMORY_SIZE, noFriend);

    return MALLOC_SUCCESS;
}

void initSlabList() {
    slabs.head_p = NULL;
    slabs.tail_p = NULL;
    slabs.offset = HEADER_SIZE;
}

void calcGlobalPowers() {
    highestPower = (int) log2(MEMORY_SIZE);
    lowestPower = (int) log2(MIN_MEM_CHUNK_SIZE);
    powerRange = highestPower - lowestPower;
}

/*
***************************
    END -> GLOBAL & Inits
***************************
*/


/*
***************************
    Start -> General Functions
***************************
*/

int memory_request (int size) {
    int result = 0;

    switch (alloc_type) {
        case MALLOC_BUDDY: 
            result = buddy_request(size);
            break;
        case MALLOC_SLAB: 
            result = slab_request(size);
            break;
        default: exit(0);
    }

    return result;
}

int validate_memory_request(int size) {

    if (size > (int) pow(2, highestPower)) {
        return MALLOC_ERROR;
    }

    return MALLOC_SUCCESS;
}

// intents convert requested size to appropiate power of 2 block
int fitRequestSize(int size) {

    if (size <= MIN_MEM_CHUNK_SIZE) {
        return MIN_MEM_CHUNK_SIZE;
    }

    int power = (int) log2(size);
    int pow_res = (int) pow(2, power); 

    return pow(2, power + 1);
}

int free_request(int start_addr) {
    int result = 0;

    switch (alloc_type) {
        case MALLOC_BUDDY: 
            result = buddy_free(start_addr);
            break;
        case MALLOC_SLAB: 
            result = slab_free(start_addr);
            break;
    }

    return result;
}

/*
***************************
    End -> General Functions
***************************
*/


/*
***************************
    Start -> Slab Functions
***************************
*/

int slab_request(int size) {
    int block_size = N_OBJS_PER_SLAB * (size + HEADER_SIZE);
    int buddy_size = fitRequestSize(block_size);
    int addr = slab_of_size_request(size, block_size, buddy_size);

    if (addr == MALLOC_ERROR) {
        return MALLOC_ERROR;
    }

    return addr;
}

int slab_free(int start_addr) {
    SlabType* currentSlabType = slabs.head_p;

    while (currentSlabType != NULL) {
        Slab* currentSlab = currentSlabType->head_p;

        while (currentSlab != NULL) {

            int lower_bounded_range = currentSlab->buddy_addr_ptr;
            int upper_bounded_range = currentSlab->buddy_addr_ptr + currentSlabType->size;
            if (start_addr >= lower_bounded_range && start_addr <= upper_bounded_range) {
                int local_addr = lower_bounded_range + (2 * HEADER_SIZE);

                // get addr offset of free 
                for (int i = 0; i < N_OBJS_PER_SLAB; i++) {
                    if (local_addr == start_addr) {
                        currentSlab->entries[i] = EMPTY;
                        currentSlab->num_used -= 1;

                        if (currentSlab->num_used == 0) {
                            test_removeSlab(currentSlabType, currentSlab->buddy_addr_ptr);
                            test_removeSlabType(currentSlabType->type);
                        }

                        return MALLOC_SUCCESS; 
                    }
                    local_addr += (currentSlabType->type + slabs.offset);
                }

            }
            currentSlab = currentSlab->nextSlab;
        }
        
        currentSlabType = currentSlabType->nextSlabType;    
    }

    return MALLOC_ERROR; // no slabs contain this type
}

int slab_of_size_request(int slab_size, int slab_block, int buddy_size) {

    // search for slab of size -- search is slab is free < if none then we need to create a new slab >
    SlabType* currentSlabType = slabs.head_p;

    while (currentSlabType != NULL) {
        if (currentSlabType->type == slab_size) {
            Slab* currentSlab = currentSlabType->head_p;

            while (currentSlab != NULL) {;
                if (currentSlab->num_used < N_OBJS_PER_SLAB) {
                    int local_addr = HEADER_SIZE;

                    // get addr offset of free 
                    for (int i = 0; i < N_OBJS_PER_SLAB; i++) {
                        if (currentSlab->entries[i] == EMPTY) {
                            currentSlab->entries[i] = OCCUPIED;
                            currentSlab->num_used += 1;
                            break;    
                        }
                        local_addr += (currentSlabType->type + slabs.offset);
                    }

                    return currentSlab->buddy_addr_ptr + local_addr;
                }
                currentSlab = currentSlab->nextSlab;
            }

            // Did not find a free slab within current slab type < add new slab >
            int buddy_addr = buddy_request(slab_block);
        
            if (buddy_addr == MALLOC_ERROR) {
                return MALLOC_ERROR;
            }
            addSlab(currentSlabType, buddy_addr);

            return slab_of_size_request(slab_size, slab_block, buddy_size);
        }
        currentSlabType = currentSlabType->nextSlabType;  
    }

    // If we make it here we need to create a new slab
    addSlabType(slab_size, buddy_size);

    return slab_of_size_request(slab_size, slab_block, buddy_size);
}

/*
***************************
    End -> Slab Functions
***************************
*/


/*
***************************
    Start -> Buddy Functions
***************************
*/

int buddy_request(int size) {
    // Format Data
    int block_size = fitRequestSize(size);
    
    if (!validate_memory_request(block_size)) {
        return MALLOC_ERROR;
    }

    // Add Data to Structures
    int start_addr = find_smallest_free_block(block_size);

    if (start_addr == MALLOC_ERROR) {
        return MALLOC_ERROR;
    }

    return start_addr;
}

int buddy_free(int start_addr) {

    int abs_addr = start_addr - HEADER_SIZE;
    
    int found_addr = tryCoalescing(abs_addr);
    
    if (found_addr == MALLOC_ERROR) {
        return MALLOC_ERROR;
    }

    return MALLOC_SUCCESS;
}

// defn: block_size is already given in pow of 2
int find_smallest_free_block(int block_size) {

    // look thru powers for hole smallest hole
    int opening_address = 0;
    int idx = 0;
    int try = 0;
    Hole* selectedHole = NULL;

    while (idx + lowestPower <= highestPower) {
        if (hasHoles(idx)) {
            if (powers[idx].size > block_size) {
                // Loop through holes for first EMPTY
                selectedHole = powers[idx].head_p;
                while (selectedHole != NULL) {
                    if (selectedHole->state == EMPTY) {
                        break;
                    }
                    selectedHole = selectedHole->nextHole;
                }
                
                // Add 2 holes to the next power down
                int halfway_factor = (selectedHole->end_address - selectedHole->start_address) / 2;

                Buddy left; 
                Buddy right;
                left.buddy_address = selectedHole->start_address;
                left.power = powers[idx].power_num - 1;
                right.buddy_address = selectedHole->start_address + halfway_factor;
                right.power = powers[idx].power_num - 1;

                try = addHole(idx - 1, selectedHole->start_address, selectedHole->start_address + halfway_factor, right);
                try = addHole(idx - 1, selectedHole->start_address + halfway_factor, selectedHole->end_address, left);
                selectedHole->state = OCCUPIED;

                idx -= 1;
                continue;
            } 
            if (powers[idx].size == block_size) { 
                // Reached lowest buddy level - Loop through holes for first EMPTY
                selectedHole = powers[idx].head_p;
                while (selectedHole != NULL) {
                    if (selectedHole->state == EMPTY) {
                        break;
                    }
                    selectedHole = selectedHole->nextHole;
                }

                opening_address = selectedHole->start_address;
                selectedHole->state = OCCUPIED;

                return opening_address;
            }
        }
        idx += 1;
    }

    return MALLOC_ERROR;
}

int tryCoalescing(int abs_addr) {

    int idx = 0;
    Hole* currentHole = NULL; 
    bool stillCoalescing = true;
    int try = 0;
    bool found_addr = false;
    
    // Find the abs_addr in list
    while (idx < powerRange) {
        // Find addr
        currentHole = powers[idx].head_p;
        while (currentHole != NULL) {
            
            if (currentHole->start_address == abs_addr) {
                currentHole->state = EMPTY;
                found_addr = true;
                break;
            }
            currentHole = currentHole->nextHole;
        }
        if (found_addr) {
            break;
        }
        idx += 1;
    }
    
    if (!found_addr) {
        return MALLOC_ERROR; // addr can not be freed
    }

    while (stillCoalescing) {
        // if next hole in free list is the partner then coalescing is possible
        int buddy_power = currentHole->buddy_information.power - lowestPower;
        int buddy_addr = currentHole->buddy_information.buddy_address;
        int found_buddy = false;

        // Find buddy
        Hole* buddy_hole = powers[buddy_power].head_p; 

        while (buddy_hole != NULL) {
            if (buddy_hole->start_address == buddy_addr) {
                found_buddy = true;
                break;
            }
            buddy_hole = buddy_hole->nextHole;
        }

        if (!found_buddy) {
            return MALLOC_ERROR; // will throw error on top level coalescing as it has no buddy
        }

        if (buddy_hole->state == EMPTY) {
            // find probable buddy of combined block
            int lower_block_bound = currentHole->start_address < buddy_hole->start_address ? currentHole->start_address : buddy_hole->start_address;
            int upper_block_bound = currentHole->end_address > buddy_hole->end_address ? currentHole->end_address : buddy_hole->end_address;
            Buddy buddy;
            buddy.buddy_address = upper_block_bound;
            buddy.power = currentHole->power + 1;

            // add new combined buddy to next power 
            Hole* occupiedNext = powers[buddy_power + 1].head_p;
            while (occupiedNext != NULL) {
                if (occupiedNext->start_address == lower_block_bound) {
                    break;
                }
                occupiedNext = occupiedNext->nextHole;
            }
            occupiedNext->state = EMPTY;
            
            // remove buddy 1 and 2 from list
            try = removeHole(buddy_power, currentHole->start_address);
            try = removeHole(buddy_power, buddy_hole->start_address);

            // Update currentHole to reflect the combined holes in next level
            currentHole = powers[buddy_power + 1].head_p;
            while (currentHole != NULL) {
                
                if (currentHole->start_address == lower_block_bound) {
                    break;
                }
                currentHole = currentHole->nextHole;
            }
        } else {
            break;
        }
    }
    
    return MALLOC_SUCCESS;
}

/*
***************************
    End -> Buddy Functions
***************************
*/


/*
***************************
    Start -> List Functions
***************************
*/

int addSlabType(int type, int size) {

    SlabType* newSlab = (SlabType*)malloc(sizeof(SlabType));

    if (newSlab == NULL) {
        return MALLOC_ERROR;
    }
    
    newSlab->head_p = NULL;
    newSlab->tail_p = NULL;
    newSlab->type = type;
    newSlab->size = size; 
    newSlab->nextSlabType = NULL;

    if (slabs.head_p == NULL) {
        slabs.head_p = newSlab;
        slabs.tail_p = newSlab;
        return MALLOC_SUCCESS;
    }

    if (slabs.tail_p != NULL) {
        slabs.tail_p->nextSlabType = newSlab;
    }
    slabs.tail_p = newSlab;

    return MALLOC_SUCCESS;
}

int addSlab(SlabType* slab_t, int buddy_addr) {

    Slab* newSlab = (Slab*)malloc(sizeof(Slab));

    if (newSlab == NULL) {
        return MALLOC_ERROR;
    }

    newSlab->num_used = 0;
    newSlab->buddy_addr_ptr = buddy_addr;
    newSlab->nextSlab = NULL;

    // init the ints 
    for (int i = 0; i < N_OBJS_PER_SLAB; i++) {
        newSlab->entries[i] = EMPTY;
    }

    if (slab_t->head_p == NULL) {
        slab_t->head_p = newSlab;
        slab_t->tail_p = newSlab;
        return MALLOC_SUCCESS;
    }

    if (slab_t->tail_p != NULL) {
        slab_t->tail_p->nextSlab = newSlab;
    }
    slab_t->tail_p = newSlab;

    return MALLOC_SUCCESS;
}

int addHole(int idx, int start_addr, int end_addr, Buddy buddy) {

    Hole* nextHole = malloc(sizeof(Hole));

    if (nextHole == NULL) {
        return MALLOC_ERROR;
    }

    nextHole->nextHole = NULL;
    nextHole->power = powers[idx].power_num;
    nextHole->buddy_information = buddy;
    nextHole->state = EMPTY;
    nextHole->start_address = start_addr;
    nextHole->end_address = end_addr;
    nextHole->size = powers[idx].size;

    if (powers[idx].head_p == NULL) {
        powers[idx].head_p = nextHole;
        return MALLOC_SUCCESS;
    }

    Hole* currentHole = powers[idx].head_p;
    Hole* previousHole = powers[idx].head_p;

    while (currentHole != NULL) {
        if (currentHole->start_address > nextHole->start_address) {
            
            if (powers[idx].head_p->start_address == currentHole->start_address) {
                powers[idx].head_p = nextHole;
                nextHole->nextHole = currentHole;
                return MALLOC_SUCCESS;
            }
            
            previousHole->nextHole = nextHole;
            nextHole->nextHole = currentHole;
            return MALLOC_SUCCESS;
        } else {
            
            if (currentHole->nextHole == NULL) {
                currentHole->nextHole = nextHole;
                return MALLOC_SUCCESS;
            }

            previousHole = currentHole;
            currentHole = currentHole->nextHole;
        }
    }

    return MALLOC_ERROR;
}

void test_removeSlab(SlabType* currentSlabType, int buddy_addr) {

    Slab* currentSlab = currentSlabType->head_p;
    Slab* previousSlab = currentSlabType->head_p;

    while (currentSlab != NULL) {
        if (currentSlab->buddy_addr_ptr == buddy_addr) {
            previousSlab->nextSlab = currentSlab->nextSlab;

            if (currentSlab->buddy_addr_ptr == currentSlabType->head_p->buddy_addr_ptr) {
                currentSlabType->head_p = currentSlab->nextSlab;
            } // if rem head then update head

            if (currentSlab->nextSlab == NULL) {
                currentSlabType->tail_p = previousSlab;
                previousSlab->nextSlab = NULL;
            }

            if (currentSlabType->head_p == NULL) {
                currentSlabType->tail_p = NULL;
            }

            free(currentSlab);
            buddy_free(buddy_addr + HEADER_SIZE);
            return;
        } else {
            previousSlab = currentSlab;
            currentSlab = currentSlab->nextSlab;
        }
    }
}

void test_removeSlabType(int type) {
    SlabType* currentSlabType = slabs.head_p;
    SlabType* previousSlabType = slabs.head_p;

    while (currentSlabType != NULL) {
        if (currentSlabType->type == type) {

            // Test removal 
            if (currentSlabType->head_p != NULL) {
                return;
            }

            previousSlabType->nextSlabType = currentSlabType->nextSlabType;

            if (currentSlabType->type == slabs.head_p->type) {
                slabs.head_p = currentSlabType->nextSlabType;
            } // if rem head then update head

            if (currentSlabType->nextSlabType == NULL) {
                slabs.tail_p = previousSlabType;
                previousSlabType->nextSlabType = NULL;
            }

            free(currentSlabType);

            return;
        } else {
            previousSlabType = currentSlabType;
            currentSlabType = currentSlabType->nextSlabType;
        }
    }
}


int removeHole(int idx, int start_addr) {

    Hole* currentHole = powers[idx].head_p;
    Hole* previousHole = powers[idx].head_p;

    while (currentHole != NULL) {
        if (currentHole->start_address == start_addr) {
            previousHole->nextHole = currentHole->nextHole;

            if (currentHole->start_address == powers[idx].head_p->start_address) {
                powers[idx].head_p = currentHole->nextHole;
            } // if rem head then update head

            free(currentHole);
            return MALLOC_SUCCESS;
        } else {
            previousHole = currentHole;
            currentHole = currentHole->nextHole;
        }
    }

    return MALLOC_ERROR;
}

bool hasHoles(int idx) {

    Hole* currentHole = powers[idx].head_p;

    while (currentHole != NULL) {
        
        if (currentHole->state == EMPTY) {
            return true;
        }

        currentHole = currentHole->nextHole;
    }

    return false;
}

void printList(int idx) {

    Hole* currentHole = powers[idx].head_p;

    printf("Powers[%d] : head_p -> ", idx);
    while (currentHole != NULL) {
        printf(" %d(%d) ", currentHole->start_address, currentHole->state);
        currentHole = currentHole->nextHole;
    }
    printf("\n");

}

void printSlabAll() {

    SlabType* currentType = slabs.head_p;

    while (currentType != NULL) {
        printf("Type %d : head_p -> ", currentType->type);
        Slab* currentSlab = currentType->head_p;
        while (currentSlab != NULL) {
            printf(" %d(%d) ",  currentType->type, currentSlab->buddy_addr_ptr);
            currentSlab = currentSlab->nextSlab;
        }
        printf("\n");
        currentType = currentType->nextSlabType;
    }
}

/*
***************************
    End -> List Functions
***************************
*/
