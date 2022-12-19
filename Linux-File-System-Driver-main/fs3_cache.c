/* 
    ********************************************************* 
	Start Region Init 
	File           : fs3_cache.c
    Description    : This is the implementation of the cache for the 
                     FS3 filesystem interface.
   Author        : *** David Rohweder ***
   Last Modified : *** October 7, 2021 ***
   *********************************************************
*/ 
  
// Includes
#include <cmpsc311_log.h>
#include <stdlib.h>
#include <string.h>

// Project Includes
#include <fs3_cache.h>
#include <cmpsc311_log.h>

// Support Macros/Data


/* 
    ********************************************************* 
	Start Region Global Defines
   *********************************************************
*/ 


//  Global Structures
typedef struct LRU_Cache {
    char cacheBuf[FS3_SECTOR_SIZE]; 
    int sector;
    int track;
    uint64_t lastAccessed;
    int ready;  
} LRU_Cache;


// Global Variables
LRU_Cache *cache = NULL;
int sizeCache = 0;
int cacheReady = 0;
uint64_t hits = 0;
uint64_t misses = 0; 
uint64_t eventTimer = 0; 


/* 
    ********************************************************* 
	Start Region Code
   *********************************************************
*/ 


/*
 Function     : fs3_init_cache
 Description  : Initialize the cache with a fixed number of cache lines
 Inputs       : sizeCache - the number of cache lines to include in cache
 Outputs      : 0 if successful, -1 if failure
*/
int fs3_init_cache(uint16_t cachelines) {
    
    if (cachelines <= 0) {
        logMessage(LOG_ERROR_LEVEL, "Err: Cannot have 0 or neg cache lines length \n");
        return(-1);
    } // validate input

    // init global vars
    sizeCache = cachelines;
    cache = malloc(sizeof(LRU_Cache) * cachelines);

    // init cache to blank data: for bc each element must be exhausted
    for (int i = 0; i < sizeCache; i++) {
        cache[i].ready = 0;
        cache[i].sector = 0;
        cache[i].track = 0;
        cache[i].lastAccessed = 0;
    }

    // cache has been initalized
    cacheReady = 1;

    return(0);
}


/*
 Function     : fs3_close_cache
 Description  : Close the cache, freeing any buffers held in it
 Inputs       : none
 Outputs      : 0 if successful, -1 if failure
*/
int fs3_close_cache(void)  {

    if (cacheReady == 0) {
        logMessage(LOG_ERROR_LEVEL, "Err: Cache has not be initalized yet \n");
        return(-1);
    }

    free(cache);
    cacheReady = 0;

    return(0);
}


/*
 Function     : fs3_put_cache
 Description  : Put an element in the cache
 Inputs       : trk - the track number of the sector to put in cache
                sct - the sector number of the sector to put in cache
 Outputs      : 0 if inserted, -1 if not inserted
*/
int fs3_put_cache(FS3TrackIndex trk, FS3SectorIndex sct, void *buf) {
    
    if (cacheReady == 0) {
        logMessage(LOG_ERROR_LEVEL, "Err: Cache has not be initalized yet \n");
        return(-1);
    } // Err: cache not initalized

    int i = 0, lruElement, lruTime = eventTimer, inserted = -1;

    while (i < sizeCache && inserted == -1) {

        if (cache[i].ready == 0) {
            memcpy(cache[i].cacheBuf, buf, FS3_SECTOR_SIZE);
            cache[i].lastAccessed = eventTimer;
            cache[i].ready = 1;
            cache[i].sector = sct;
            cache[i].track = trk;
            inserted = 0;
        } // empty element in cache: highest priority, will end search

        if (cache[i].sector == sct && cache[i].track == trk && cache[i].ready == 1 && inserted == -1) {
            memcpy(cache[i].cacheBuf, buf, FS3_SECTOR_SIZE);
            cache[i].lastAccessed = eventTimer;
            inserted = 0;
        } // element already in cache, update time accessed and buffer data, 2nd priority, will end search

        if (cache[i].lastAccessed < lruTime && inserted == -1) {
            lruElement = i;
            lruTime = cache[i].lastAccessed;
        } // if no empty space keep track of smallest last accessed time, lowest priority, ends when elements exhausted

        i += 1;
    }
 
    if (inserted == -1) {
        memcpy(cache[lruElement].cacheBuf, buf, FS3_SECTOR_SIZE);
        cache[lruElement].ready = 1;
        cache[lruElement].lastAccessed = eventTimer;
        cache[lruElement].sector = sct;
        cache[lruElement].track = trk;
        inserted = 0;
    }
     // if no insertions then insert into LRU element into cache
    eventTimer += 1;
    
    return(inserted);
}


/*
 Function     : fs3_get_cache
 Description  : Get an element from the cache (

 Inputs       : trk - the track number of the sector to find
                sct - the sector number of the sector to find
 Outputs      : returns NULL if not found or failed, pointer to buffer if found
*/
void * fs3_get_cache(FS3TrackIndex trk, FS3SectorIndex sct)  {
    
    if (cacheReady == 0) {
        logMessage(LOG_ERROR_LEVEL, "Err: Cache has not be initalized yet \n");
        return(NULL);
    }

    int i = 0, found = 0;

    // O(n) => linear search algorithm
    while (i < sizeCache && found == 0) {

        if (cache[i].sector == sct && cache[i].track == trk && cache[i].ready == 1) {
            cache[i].lastAccessed = eventTimer;
            hits += 1;
            eventTimer += 1;

            return(cache[i].cacheBuf);
        } // check skr and trk to see if the combination is in cache already

        i += 1;
    }

    misses += 1;

    return(NULL);
}


/*
 Function     : fs3_log_cache_metrics
 Description  : Log the metrics for the cache 
 Inputs       : none
 Outputs      : 0 if successful, -1 if failure
*/
int fs3_log_cache_metrics(void) {

    if (cacheReady == 0) {
        logMessage(LOG_ERROR_LEVEL, "Err: Cache has not be initalized yet \n");
        return(-1);
    }
    
    float ratio = (float)hits / ((float)misses + (float)hits);
    logMessage(LOG_ERROR_LEVEL, "Cache Info: hits: %d, misses %d, hit ratio: %.2f\% \n", hits, misses, ratio * 100);

    return(0);
}
