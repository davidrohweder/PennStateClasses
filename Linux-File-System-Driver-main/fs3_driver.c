/* 
    ********************************************************* 
	Start Region Init 
	File           : fs3_driver.c
    Description    : This is the implementation of the standardized IO functions
                     for used to access the FS3 storage system.
   Author        : *** David Rohweder ***
   Last Modified : *** October 7, 2021 ***
   *********************************************************
*/ 


// Includes
#include <string.h>
#include <cmpsc311_log.h>
#include <stdlib.h>

// Project Includes
#include <fs3_driver.h>
#include <fs3_cache.h>
#include <fs3_controller.h>
#include <fs3_network.h>

// Defines
#define SECTOR_INDEX_NUMBER(x) ((int)(x/FS3_SECTOR_SIZE))


/* 
    ********************************************************* 
	Start Region Global Defines
   *********************************************************
*/ 


//  Global Structures
typedef struct SysState {
	int currTrack;
	int currSector;
} sysState;

typedef struct FileDictionary {
	char filename[FS3_MAX_PATH_LENGTH]; 
	int open; // open or closed file, true or false respectivley
	int position; // current position the union of all sectors combined
	int length; // total length of all sectors unioned 
	int numOccupiedSectors; // used for realloc occupiedSectors
	sysState *occupiedSectors; // dynamic array of occupied sectors 
} FileDictionary; 

typedef struct SystemCall {
	FS3CmdBlk commandBlock;
	FS3CmdBlk opcode;
	FS3CmdBlk track;
	FS3CmdBlk sector;
	FS3CmdBlk returnVal;
} SystemCall;

// Global Variables
FileDictionary *dictionary = NULL; //  dynamically allocated array of all files
sysState currState; // keep track of current track and sector in system
SystemCall sysCall; // fields sent when commmand blocks are generated and deconstructed

int fileHandlerAssignment = 5; // give out unique identifiers for open files starts at 5 to avoid interfering with main fd's
int mounted = 0; // mount status
int totalFiles; // helps with bounds for dynamic array dictionary. keeps track of # of files
int valr = 1;

/* 
    ********************************************************* 
	Start Region Code
   *********************************************************
*/ 


/*
 Function     : generate_opcode
 Description  : given inputs encode a FS3CmdBlk value
 Inputs       : none
 Outputs      : 0 if successful
*/ 
 FS3CmdBlk fs3_generate_command_block(FS3CmdBlk opcode, FS3CmdBlk track, FS3CmdBlk sector, FS3CmdBlk returnVal) {

	FS3CmdBlk commandBlock; 

	sysCall.opcode = opcode;
	sysCall.track = track;
	sysCall.sector = sector;
	sysCall.returnVal = returnVal;
	
	commandBlock = sysCall.opcode << 60;
	commandBlock = commandBlock | (sysCall.sector << 44);
	commandBlock = commandBlock | (sysCall.track << 12);
	commandBlock = commandBlock | (sysCall.returnVal << 11);

	return(commandBlock); 
}


/*
 Function     : fs3_deconstruct_command_block
 Description  : given inputs decode a FS3CmdBlk value
 Inputs       : none
 Outputs      : 0 if successful
*/ 
 int fs3_deconstruct_command_block(void) {

	sysCall.opcode = sysCall.commandBlock >> 60;
	sysCall.track = sysCall.commandBlock >> 44;
	sysCall.sector = sysCall.commandBlock >> 12;
	sysCall.returnVal = sysCall.commandBlock >> 11;

	return(0); 
}


/*
 Function     : fs3_mount_disk
 Description  : FS3 interface, mount/initialize filesystem
 Inputs       : none
 Outputs      : 0 if successful, -1 if failure
*/
int32_t fs3_mount_disk(void) {
	
	if (mounted == 1) {
		logMessage(LOG_ERROR_LEVEL, " Path is too big or not fs3 mounted \n");
		return(-1);
	} // Return error already mounted
	
	//initialize fs3 filesystem		
	sysCall.commandBlock = fs3_generate_command_block(FS3_OP_MOUNT,0,0,0);
	int ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, NULL); // call to mount
	fs3_deconstruct_command_block();

	if (ret == -1) {
		logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
		return(-1);
	} else if (sysCall.returnVal == -1) {
		logMessage(LOG_ERROR_LEVEL, "Err: System Mounted Failed \n");
		return(-1);
	}
	
	// init internal data structures to ONE void file size system

	// update internal tracker of area in file system so that we are at sector 0, track 0 
	currState.currSector = 0; // we are at first sector
	currState.currTrack	= 0; // we are at first track

	//update to 1 file named NULL that is closed occupying sector 1 track 1
	dictionary = malloc(sizeof(FileDictionary)); // init to 1 file size
	strcpy(dictionary[0].filename, "first_unique"); // init file to name null
	dictionary[0].open = 0; // not open
	dictionary[0].length = 0; // file has 0 len
	dictionary[0].position = 0; // file is at pos 0
	dictionary[0].occupiedSectors = malloc(sizeof(sysState)); // init to occupy sector, track [0][0]
	dictionary[0].numOccupiedSectors = 1;
	dictionary[0].occupiedSectors[0].currSector = currState.currSector;
	dictionary[0].occupiedSectors[0].currTrack = currState.currTrack;

	totalFiles = 1; // we have one file, used for bounds checking and realloc dictionary

	// flag MOUNTED
	mounted = 1;

	return(0); // Return mount sucessful 
}


/*
 Function     : fs3_open
 Description  : This function opens the file and returns a file handle
 Inputs       : path - filename of the file to open
 Outputs      : file handle if successful, -1 if failure
*/
int16_t fs3_open(char *path) {

	if (*path > FS3_MAX_PATH_LENGTH || mounted == 0) {
		logMessage(LOG_ERROR_LEVEL, "Path is too big or not fs3 mounted \n");
		return(-1);
	} // file path too long or not mounted

	int found = 0, i = 0;

	while (i < totalFiles && found == 0) {
		
		if (strcmp(dictionary[i].filename, path) == 0 || strcmp(dictionary[0].filename, "first_unique") == 0) {

			if (dictionary[i].open == 1) {
				logMessage(LOG_ERROR_LEVEL, "Err: File already opened \n");
				return(-1);
			} // file already opened 

			if (strcmp(dictionary[0].filename, "first_unique") == 0) {
				strcpy(dictionary[0].filename, path);
			} // special case for when FS3 is initalized
			
			found = 1;
			dictionary[i].open = 1;
		} // file exists already

		i += 1;
	} // go thru dictionary to see if file already exists

	if (found == 0) { 

		if (currState.currSector + 1 > FS3_TRACK_SIZE - 1) {
			
			if (currState.currTrack + 1 > FS3_MAX_TRACKS -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: File system has filled up \n");
				return(-1);
			} // Check to see if file system has filled up

			currState.currSector = 0;// update to new track first sector
			currState.currTrack += 1;
		} else { 
			currState.currSector += 1; // there is room in current track, move to next sector to store new file	
		}// update currState

		// internal maintance
		fileHandlerAssignment += 1; // increment global fd assigner to plus 1
		totalFiles += 1; // increment global total files to plus 1	
		
		FileDictionary *tryRealloc = realloc(dictionary, sizeof(FileDictionary) * totalFiles); // reallocate dictionary for one more file
	
		if (tryRealloc == NULL) {
			logMessage(LOG_ERROR_LEVEL, "Err: Sys busy could not handle request \n");
			return(-1);
		} // check return pointer

		dictionary = tryRealloc;
		
		strcpy(dictionary[totalFiles-1].filename, path); // set filename to path
		dictionary[totalFiles-1].open = 1; // set new file to open
		dictionary[totalFiles-1].length = 0; // file has 0 len
		dictionary[totalFiles-1].position = 0; // file is at pos 0
		dictionary[totalFiles-1].numOccupiedSectors = 1;
		dictionary[totalFiles-1].occupiedSectors = malloc(sizeof(sysState)); // init to occupy current sector, track in currState
		dictionary[totalFiles-1].occupiedSectors[0].currSector = currState.currSector;
		dictionary[totalFiles-1].occupiedSectors[0].currTrack = currState.currTrack;

	} // if the file was not found then, {if possible} add file to dictionary

	return(fileHandlerAssignment); 
}


/* 
 Function     : fs3_close
 Description  : This function closes the file
 Inputs       : fd - the file descriptor
 Outputs      : 0 if successful, -1 if failure
*/
int16_t fs3_close(int16_t fd) {

	if (fd > fileHandlerAssignment || fd < 5 || mounted == 0) {
		logMessage(LOG_ERROR_LEVEL, "Err: File at handler is not open, does not exist, or fs3 not mounted \n");
		return (-1);
	} // File at handler is not open, does not exist, or fs3 not mounted

	// Close active file 
	dictionary[fd - 5].open = 0;

	return (0);
}


/*
 Function     : fs3_read
 Description  : Reads "count" bytes from the file handle "fh" into the 
                buffer "buf"
 Inputs       : fd - filename of the file to read from
                buf - pointer to buffer to read into
                count - number of bytes to read
 Outputs      : bytes read if successful, -1 if failure
*/
int32_t fs3_read(int16_t fd, void *buf, int32_t count) {
	
	if (dictionary[fd - 5].open == 0 || fd > fileHandlerAssignment || fd < 0 || mounted == 0) {
		logMessage(LOG_ERROR_LEVEL, "Err: File at handler is not open, does not exist, or fs3 not mounted \n");
		return(-1);
	} // File at handler is not open, does not exist, or is not mounted

	if (count + dictionary[fd - 5].position > dictionary[fd - 5].length) {
		count = dictionary[fd - 5].length - dictionary[fd - 5].position;
	} // Modified bytes to read {if count + pos > length then }

	int returnCount = count;
	int currSector = dictionary[fd - 5].position / FS3_SECTOR_SIZE;
	char sectorBuffer[FS3_SECTOR_SIZE];
	int filled = 0;
	int sectorPos; // get pos in current sector
	int secCpyAmount; // used to show how much of buff to put into specific sector
	int bufferOffest = 0; 

	while (filled == 0) {

		// read in sector from {True: cache}, {False: FS3}
		if (valr == 0 && fs3_get_cache(dictionary[fd - 5].occupiedSectors[currSector].currTrack, dictionary[fd - 5].occupiedSectors[currSector].currSector) != NULL) {
			memcpy(sectorBuffer, fs3_get_cache(dictionary[fd - 5].occupiedSectors[currSector].currTrack, dictionary[fd - 5].occupiedSectors[currSector].currSector), FS3_SECTOR_SIZE);
		} else {			
			// Seek to track
			sysCall.commandBlock = fs3_generate_command_block(FS3_OP_TSEEK,dictionary[fd - 5].occupiedSectors[currSector].currTrack,0,0);
			int ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, NULL); // call to seek track
			fs3_deconstruct_command_block();

			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
				return(-1);
			} else if (sysCall.returnVal == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Seek Track[%d] FAILED \n", sysCall.track);
				return(-1);
			}

			// Read sector from track
			sysCall.commandBlock = fs3_generate_command_block(FS3_OP_RDSECT,0,dictionary[fd - 5].occupiedSectors[currSector].currSector,0);
			ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, sectorBuffer); // call read sector
			fs3_deconstruct_command_block();

			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
				return(-1);
			} else if (sysCall.returnVal == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Read Sector Failed %d in track %d FAILED \n", sysCall.sector, sysCall.track);
				return(-1);
			}
			
			// update cache with modified buffer
			ret = fs3_put_cache(dictionary[fd - 5].occupiedSectors[currSector].currTrack, dictionary[fd - 5].occupiedSectors[currSector].currSector, &sectorBuffer);

			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Unable to update sector in cache \n", sysCall.sector, sysCall.track);
				return(-1);				
			}
			
		} // check if sector is in cache. If not read from FS3 then store in cache

		// IF - {true} -> write op exceeds currSector	::: IF {false} -> write op does not fill currSector
		if (dictionary[fd - 5].position + count >= (currSector + 1) * FS3_SECTOR_SIZE) {
			sectorPos = dictionary[fd - 5].position - (currSector * FS3_SECTOR_SIZE); // offset of pos and last sector
			secCpyAmount = FS3_SECTOR_SIZE - sectorPos; // amount in currSector to copy using offset sectorPos

			memcpy(&buf[bufferOffest], &sectorBuffer[sectorPos], secCpyAmount);

			bufferOffest += secCpyAmount;
			count -= secCpyAmount;
			dictionary[fd - 5].position += secCpyAmount; 
			currSector += 1;
		} else {
			sectorPos = dictionary[fd - 5].position - (currSector * FS3_SECTOR_SIZE); // offset of pos and last sector
			
			memcpy(&buf[bufferOffest], &sectorBuffer[sectorPos], count);

			dictionary[fd -5].position += count; 
			filled = 1;
		}

	}

	return (returnCount);
}


/*
 Function     : fs3_write
 Description  : Writes "count" bytes to the file handle "fh" from the 
                buffer  "buf"
 Inputs       : fd - filename of the file to write to
                buf - pointer to buffer to write from
                count - number of bytes to write
 Outputs      : bytes written if successful, -1 if failure
*/
int32_t fs3_write(int16_t fd, void *buf, int32_t count) {

	if (dictionary[fd - 5].open == 0 || fd > fileHandlerAssignment || fd < 5 || mounted == 0) {
		logMessage(LOG_ERROR_LEVEL, "Err: File at handler is not open, does not exist, or not mounted \n");
		return(-1);
	} // File at handler is not open, does not exist, or not mounted

	int returnCount = count; // count is modified so store origional for return 
	int currSector = dictionary[fd - 5].position / FS3_SECTOR_SIZE; // file -> [1,2,CURR,3,.., n]
	char sectorBuffer[FS3_SECTOR_SIZE]; // ops occur based on whole sector / op
	int filled = 0; // when data is fully written to file then exit loop
	int sectorPos; // get pos in current sector
	int secCpyAmount; // used to show how much of buff to put into currSector
	int buffOffset = 0; // offset written amount of param buff if multiple sectors are needed
	int ret = 0; // ret for network syscall
	
	while (filled == 0) {

		// read in sector from {True: cache}, {False: FS3}
		if (valr == 0 && fs3_get_cache(dictionary[fd - 5].occupiedSectors[currSector].currTrack, dictionary[fd - 5].occupiedSectors[currSector].currSector) != NULL) {
			memcpy(sectorBuffer, fs3_get_cache(dictionary[fd - 5].occupiedSectors[currSector].currTrack, dictionary[fd - 5].occupiedSectors[currSector].currSector), FS3_SECTOR_SIZE);
		} else {	
			// Seek to track
			sysCall.commandBlock = fs3_generate_command_block(FS3_OP_TSEEK,dictionary[fd - 5].occupiedSectors[currSector].currTrack,0,0);
			ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, NULL); // call to seek track
			fs3_deconstruct_command_block();

			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
				return(-1);
			} else if (sysCall.returnVal == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Seek Track[%d] FAILED \n", sysCall.track);
				return(-1);
			}			
			
			// read in sector
			sysCall.commandBlock = fs3_generate_command_block(FS3_OP_RDSECT,0,dictionary[fd - 5].occupiedSectors[currSector].currSector,0);
			ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, sectorBuffer); // call read sector
			fs3_deconstruct_command_block();
			
			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
				return(-1);
			} else if (sysCall.returnVal == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Read Sector Failed %d in track %d FAILED \n", sysCall.sector, sysCall.track);
				return(-1);
			}

		}

		if (dictionary[fd - 5].position + count >= (currSector + 1) * FS3_SECTOR_SIZE) {
			sectorPos = dictionary[fd - 5].position - (currSector * FS3_SECTOR_SIZE); // offset of pos and last sector
			secCpyAmount = FS3_SECTOR_SIZE - sectorPos; 

			memcpy(&sectorBuffer[sectorPos], &buf[buffOffset], secCpyAmount);
			count -= secCpyAmount;
			buffOffset += secCpyAmount;
			dictionary[fd - 5].position += secCpyAmount; 

			// write buff block to sector in disk
			sysCall.commandBlock = fs3_generate_command_block(FS3_OP_WRSECT,0,dictionary[fd - 5].occupiedSectors[currSector].currSector,0);
			ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, sectorBuffer); // call write sector
			fs3_deconstruct_command_block();

			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
				return(-1);
			} else if (sysCall.returnVal == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Write Sector[%d] Failed in Track[%d] \n", sysCall.sector, sysCall.track);
				return(-1);
			}
			
			// update cache with modified buffer
			ret = fs3_put_cache(dictionary[fd - 5].occupiedSectors[currSector].currTrack, dictionary[fd - 5].occupiedSectors[currSector].currSector, &sectorBuffer);

			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Unable to update sector in cache \n", sysCall.sector, sysCall.track);
				return(-1);				
			}
			
			currSector += 1; // move to next occupied sector in file
		} else {
			sectorPos = dictionary[fd - 5].position - (currSector * FS3_SECTOR_SIZE); // offset of pos and last sector
			memcpy(&sectorBuffer[sectorPos], &buf[buffOffset], count);

			// write buff block to sector on disk
			sysCall.commandBlock = fs3_generate_command_block(FS3_OP_WRSECT,0,dictionary[fd - 5].occupiedSectors[currSector].currSector,0);
			ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, sectorBuffer); // call write sector
			fs3_deconstruct_command_block();
			
			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
				return(-1);
			} else if (sysCall.returnVal == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Write Sector[%d] Failed in Track[%d] \n", sysCall.sector, sysCall.track);
				return(-1);
			}
			
			// update cache with modified buffer
			ret = fs3_put_cache(dictionary[fd - 5].occupiedSectors[currSector].currTrack, dictionary[fd - 5].occupiedSectors[currSector].currSector, &sectorBuffer);

			if (ret == -1) {
				logMessage(LOG_ERROR_LEVEL, "Err: Unable to update sector in cache \n", sysCall.sector, sysCall.track);
				return(-1);				
			}
			
			// maintaince
			dictionary[fd -5].position += count; 
			filled = 1;

		} // IF - {true} -> write op exceeds currSector	::: IF {false} -> write op does not fill currSector

		if (dictionary[fd - 5].position >= dictionary[fd - 5].numOccupiedSectors * FS3_SECTOR_SIZE) {

			if (currState.currSector + 1 > FS3_TRACK_SIZE - 1) {
				
				if (currState.currTrack + 1 > FS3_MAX_TRACKS -1) {
					logMessage(LOG_ERROR_LEVEL, "Err: File system has filled up \n");
					return(-1);
				} // Check to see if file system has filled up

				currState.currSector = 0;// update to new track first sector
				currState.currTrack += 1;
			} else {
				currState.currSector += 1; // there is room in current track, move to next sector to store new file
			} // update currState

			dictionary[fd - 5].numOccupiedSectors += 1;

			sysState *tryRealloc = realloc(dictionary[fd - 5].occupiedSectors, sizeof(FileDictionary) * dictionary[fd - 5].numOccupiedSectors); // reallocate sectors occupied by file by 1
	
			if (tryRealloc == NULL) {
				logMessage(LOG_ERROR_LEVEL, "Err: Sys busy could not handle request \n");
				return(-1);
			} // check return pointer

			dictionary[fd - 5].occupiedSectors = tryRealloc;
			dictionary[fd - 5].occupiedSectors[dictionary[fd - 5].numOccupiedSectors - 1].currSector = currState.currSector;
			dictionary[fd - 5].occupiedSectors[dictionary[fd - 5].numOccupiedSectors - 1].currTrack = currState.currTrack;
		} // add sector since write exceeds current sector limitations: if pos + count > max mem

		if (dictionary[fd -5].position > dictionary[fd -5].length){
			dictionary[fd -5].length = 	dictionary[fd -5].position; 	
		} // update file leng only if new pos > curr leng

	}

	return (returnCount);

}


/*
 Function     : fs3_seek
 Description  : Seek to specific point in the file
 Inputs       : fd - filename of the file to write to
                loc - offfset of file in relation to beginning of file
 Outputs      : 0 if successful, -1 if failure
*/
int32_t fs3_seek(int16_t fd, uint32_t loc) {

	if (dictionary[fd - 5].open == 0 || fd > fileHandlerAssignment || fd < 5 || mounted == 0) {
		logMessage(LOG_ERROR_LEVEL, "Err: File at handler is not open, does not exist, or not mounted \n");
		return(-1);
	} // Err: File at handler is not open, does not exist, or not mounted

	if (loc > (dictionary[fd - 5]).length) {
		logMessage(LOG_ERROR_LEVEL, "Err: position seeked too far ahead \n");
 		return(-1);
	} // Err: position seeked too far ahead

	dictionary[fd - 5].position = loc;

	return (0);
}


/*
 Function     : fs3_unmount_disk
 Description  : FS3 interface, unmount the disk, close all files
 Inputs       : none
 Outputs      : 0 if successful, -1 if failure
*/
int32_t fs3_unmount_disk(void) {

	if (mounted == 0) {
		logMessage(LOG_ERROR_LEVEL, "Err: File at handler is not open, does not exist, or not mounted \n");
		return (-1);
	} // Err: cannot unmount and already unmounted disk

	// Call controller and turn off FS3
	sysCall.commandBlock = fs3_generate_command_block(FS3_OP_UMOUNT,0,0,0);
	int ret = network_fs3_syscall(sysCall.commandBlock, &sysCall.commandBlock, NULL); // call write sector
	fs3_deconstruct_command_block();

	if (ret == -1) {
		logMessage(LOG_ERROR_LEVEL, "Err: Network Connection Failed \n");
		return(-1);
	} else if (sysCall.returnVal == -1) {
		logMessage(LOG_ERROR_LEVEL, "Err: UnMount Operation Failed \n");
		return(-1);
	}

	for (int i = 0; i < totalFiles; i++) {
		
		ret = fs3_close(i+5); // double ensure all files are closed

		if (ret == -1) {
			logMessage(LOG_ERROR_LEVEL, "Err: Error closing file on unmount op");
			return(-1);
		}

		free(dictionary[i].occupiedSectors);
	} //Close all active files and free all data

	// maintance
	free(dictionary);
	mounted = 0;

	return(0);
}
