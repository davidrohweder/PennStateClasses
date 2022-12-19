#ifndef MY_MEMORY_H
#define MY_MEMORY_H

/* 
***************************
    Start -> Includes
***************************
*/

#include "interface.h"
#include <math.h>

/* 
***************************
    End -> Includes
***************************
*/

/* 
***************************
    Start -> Data Structures
***************************
*/

#define MALLOC_SUCCESS 1
#define MALLOC_ERROR -1

typedef struct Buddy {
    int buddy_address;
    int power;
} Buddy;

typedef enum {
    OCCUPIED, 
    EMPTY
} HoleState;

typedef struct Slab {
    int buddy_addr_ptr;
    int entries[N_OBJS_PER_SLAB];
    int num_used;
    struct Slab* nextSlab;
} Slab;

typedef struct SlabType {
    int type; 
    int size;
    Slab* head_p;
    Slab* tail_p;
    struct SlabType* nextSlabType;
} SlabType;

typedef struct SlabList {
    SlabType* head_p;
    SlabType* tail_p;
    int offset;
} SlabList;

typedef struct Hole {
    int power;
    int start_address;
    int end_address;
    int size;
    Buddy buddy_information;
    HoleState state;
    struct Hole* nextHole;
} Hole;

typedef struct PowerList {
    Hole* head_p; 
    int power_num;
    int size;
} PowerList;

/* 
***************************
    End -> Data Structures
***************************
*/


/* 
***************************
    Start -> Function Prototypes
***************************
*/

// inits
void mem_init(enum malloc_type, int);
int initPowerList ();
void initSlabList();
void calcGlobalPowers();

// All use func
int memory_request (int);
int fitRequestSize(int);
int free_request(int);
int validate_memory_request(int);

// slab functions
int slab_request(int);
int slab_of_size_request(int, int, int);
int slab_free(int);


// buddy functions
int buddy_request(int);
int tryCoalescing(int);
int find_smallest_free_block(int);
int buddy_free(int);

// list functions
int addSlabType(int, int);
int addSlab(SlabType*, int);
int removeHole(int, int);
void test_removeSlab(SlabType*, int);
void test_removeSlabType(int);
int addHole(int, int, int, Buddy);
bool hasHoles(int);
void printList(int);
void printSlabAll();

/* 
***************************
    End -> Function Prototypes
***************************
*/


#endif