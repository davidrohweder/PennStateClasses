/* 
    ********************************************************* 
	Start Region Init 
	File           : fs3_netowork.c
    Description    : This is the network implementation for the FS3 system.
                     
   Author        : *** David Rohweder ***
   Last Modified : *** November 19, 2021 ***
   *********************************************************
*/ 


// Includes
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cmpsc311_log.h>

// Project Includes
#include <fs3_network.h>
#include <fs3_controller.h>


/* 
    ********************************************************* 
	Start Region Global Defines
   *********************************************************
*/ 


//  Global Variables
unsigned char     *fs3_network_address = NULL; // Address of FS3 server
unsigned short     fs3_network_port = 0;       // Port of FS3 server
struct sockaddr_in caddr;
int socket_fd;


/* 
    ********************************************************* 
	Start Region Code
   *********************************************************
*/ 


int fs3_network_init(void) {

    socket_fd = socket(PF_INET, SOCK_STREAM, 0); 

    if (socket_fd == -1) {
        printf( "Error on socket creation [%s]\n", strerror(errno) );
        return(-1);
    }  

    if (fs3_network_address == NULL) {
            fs3_network_address = FS3_DEFAULT_IP;
        } // set ip to default local host

    if (fs3_network_port == 0) {
        fs3_network_port = FS3_DEFAULT_PORT;
    } // set port to default

    caddr.sin_family = AF_INET; // set protocol
    caddr.sin_port = htons(fs3_network_port); // put in network byte order

    if (inet_aton(fs3_network_address, &caddr.sin_addr) == 0) { 
        return(-1);
    } // attempt to put ip addr into sockaddr_in.sin_addr_s._adder  

    if (connect(socket_fd, (const struct sockaddr *) &caddr, sizeof(caddr)) == -1) { 
        logMessage(LOG_ERROR_LEVEL, "Conn bad \n");
        //printf("Error on socket connect [%s]\n", strerror(errno));
        return(-1);
    } 

    return(0);
}


/*
 Function     : network_fs3_syscall
 Description  : Perform a system call over the network

 Inputs       : cmd - the command block to send
                ret - the returned command block
                buf - the buffer to place received data in
 Outputs      : 0 if successful, -1 if failure
*/
int network_fs3_syscall(FS3CmdBlk cmd, FS3CmdBlk *ret, void *buf) {

    if (sizeof(cmd) != FS3_NET_HEADER_SIZE) {
        logMessage(LOG_INFO_LEVEL, "Size bad \n");
        return(-1);
    } // check size of cmd

	int opcode = cmd >> 60; // deconstruct cmd for opcode
    FS3CmdBlk networkCMD; // to send cmd over network
    uint32_t firsthalf = cmd;
    uint32_t secondhalf = cmd >> 32;

    firsthalf = htonl(firsthalf);
    secondhalf = htonl(secondhalf);

    if (opcode == FS3_OP_MOUNT) {

        if (fs3_network_init() == -1) {
            return(-1);
        }
    
    }
    
    if (write(socket_fd, &secondhalf, sizeof(secondhalf)) != sizeof(secondhalf)) { 
        printf("Error writing network data [%s]\n", strerror(errno));
        return(-1);
    } // write cmd to controller 

    if (write(socket_fd, &firsthalf, sizeof(firsthalf)) != sizeof(firsthalf)) { 
        printf("Error writing network data [%s]\n", strerror(errno));
        return(-1);
    } // write cmd to controller 
    
    if (opcode == FS3_OP_MOUNT || opcode == FS3_OP_TSEEK || opcode == FS3_OP_UMOUNT) {
        if (read(socket_fd, &networkCMD, sizeof(networkCMD)) != sizeof(networkCMD)) { 
            printf( "Error reading network data [%s]\n", strerror(errno) );
            return(-1);
        } // read cmd

        ret = ntohll64(networkCMD); // store returned cmd in return cmd

        if (ret == -1) {
            return(-1);
        }
        
    } else {
       
        if (opcode == FS3_OP_WRSECT) {

            char tempBuf[FS3_SECTOR_SIZE];

            memcpy(tempBuf, buf, FS3_SECTOR_SIZE); // smashing err w no temp

            if (write(socket_fd, &tempBuf, FS3_SECTOR_SIZE) != FS3_SECTOR_SIZE) { 
                printf("Error writing network data [%s]\n", strerror(errno));
                return(-1);
            } // send buff to controller right after cmd if non null
        }
        
        if (read(socket_fd, &networkCMD, sizeof(networkCMD)) != sizeof(networkCMD)) { 
            printf("Error reading network data [%s]\n", strerror(errno));
            return(-1);
        } // read cmd

        ret = ntohll64(networkCMD); // store returned cmd in return cmd

        if (ret == -1) {
            return(-1);
        }

        if (opcode == FS3_OP_RDSECT) {
            
            if (read(socket_fd, buf, FS3_SECTOR_SIZE) != FS3_SECTOR_SIZE) { 
                printf( "Error reading network data [%s]\n", strerror(errno) );
                return(-1);
            } // for sys read, take read data and put into buf

        } // read buf for read

    } 
    
    if (opcode == FS3_OP_UMOUNT) {
        close(socket_fd); // Close the socket
    }

    return(0);
}  
