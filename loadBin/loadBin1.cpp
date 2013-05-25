// load image file to Pru1


/******************************************************************************
* Include Files                                                               *
******************************************************************************/

// Standard header files
#include <stdio.h>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

using namespace std;

/******************************************************************************
* Explicit External Declarations                                              *
******************************************************************************/

/******************************************************************************
* Local Macro Declarations                                                    *
******************************************************************************/

#define PRU_NUM 	 0

#define OFFSET_SHAREDRAM 2048		//equivalent with 0x00002000


/******************************************************************************
* Local Typedef Declarations                                                  *
******************************************************************************/


/******************************************************************************
* Local Function Declarations                                                 *
******************************************************************************/

static int MapMemory ( );

/******************************************************************************
* Local Variable Definitions                                                  *
******************************************************************************/


/******************************************************************************
* Intertupt Service Routines                                                  *
******************************************************************************/


/******************************************************************************
* Global Variable Definitions                                                 *
******************************************************************************/

static int mem_fd;
static void *ddrMem, *sharedMem;


/******************************************************************************
* Global Function Definitions                                                 *
******************************************************************************/

#define PRUSS_MAX_IRAM_SIZE 0x2000


int main (int argc, char ** argvs)
{
    unsigned int ret;
    if(argc < 2)
	{
		cout << "loadBin binfile" << endl;
		cout << "load image file to Pru1" << endl;
		return 0;
	}
	cout << "loadBin " << argvs[1] << endl;

	// read file
    unsigned char fileDataArray[PRUSS_MAX_IRAM_SIZE];
    int fileSize = 0;	
	
	char * filename = argvs[1];
    FILE *fPtr;
	
    // Open an File from the hard drive
    fPtr = fopen(filename, "rb");
    if (fPtr == NULL) {
        printf("File %s open failed\n", filename);
    } else {
        printf("File %s open passed\n", filename);
    }
    // Read file size
    fseek(fPtr, 0, SEEK_END);
    fileSize = ftell(fPtr);

    if (fileSize == 0) {
        printf("File read failed.. Closing program\n");
        fclose(fPtr);
        return -1;
    }

    fseek(fPtr, 0, SEEK_SET);

    if (fileSize !=
        fread((unsigned char *) fileDataArray, 1, fileSize, fPtr)) {
        printf("WARNING: File Size mismatch\n");
    }

    fclose(fPtr);	
	cout << "Read File"<< endl;
	
	
	
    /* Initialize example */
    MapMemory();
	char * p = (char*)sharedMem;
	for(int i = 0; i < fileSize; i ++)
	{
		*(p + i) = fileDataArray[i];
	}
	cout << "write file to memory" << endl;
    munmap(ddrMem, 0x2000);
    close(mem_fd);

    return(0);
}

/*****************************************************************************
* Local Function Definitions                                                 *
*****************************************************************************/

static int MapMemory (  )
{

    /* open the device */
    mem_fd = open("/dev/mem", O_RDWR);
    if (mem_fd < 0) {
        printf("Failed to open /dev/mem (%s)\n", strerror(errno));
        return -1;
    }	

    /* map the shared memory */
    sharedMem = mmap(0, 0x2000, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, 0x4a338000);
    if (sharedMem == NULL) {
        printf("Failed to map the device (%s)\n", strerror(errno));
        close(mem_fd);
        return -1;
    }
    

    return(0);
}

