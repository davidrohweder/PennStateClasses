#include "interface.h"
#include "my_memory.h"

// Interface implementation
// Implement APIs here...

void* global_memory_addr;
void* start_mem;
int total_memory = 0;

void my_setup(enum malloc_type type, int mem_size, void *start_of_memory)
{
    mem_init(type, mem_size);
    total_memory = mem_size;
    start_mem = start_of_memory;
}

void *my_malloc(int size)
{
    int addr = memory_request(size);

    if (addr == MALLOC_ERROR) {
        global_memory_addr = (void*) MALLOC_ERROR;
        return global_memory_addr;
    }

    int abs_addr = addr + HEADER_SIZE;
    global_memory_addr = start_mem + abs_addr;

    return global_memory_addr;
}

void my_free(void *ptr)
{
    int mem_addr = ptr - start_mem;
    int try = free_request(mem_addr);

    if (try == MALLOC_ERROR) {
        printf("Error in free\n");
    }
}
