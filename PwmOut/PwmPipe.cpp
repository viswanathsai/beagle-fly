// read pwm
// the PwmOut.bin will write pwm to gpio pin in R30
// the firmware will read pluse width value, frame width in pru share memory.
// the first channel value will be put into 
// 0x10200 in pru, which is 0x4a310200
// each channel has 16 bytes, 
// 0x4a310100  	1st channel
// 0x4a310110	2nd channel
// 0x4a310120	3rd channel,
// ...
// 0x4a3101E0	15th channel
// for each channel, 
// the first 4 bytes are pulse width, 
// the seconds 4 bytes are frame width, it is timer count number, timer run at 200mhz, 1us is 200.
// the third and fourth 4 byts are are reserved for future use


/******************************************************************************
* Include Files                                                               *
******************************************************************************/

// Standard header files
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


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

static int LOCAL_exampleInit ( );

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

static unsigned int *sharedMem_int;

    void *DDR_regaddr1, *DDR_regaddr2, *DDR_regaddr3;	

/******************************************************************************
* Global Function Definitions                                                 *
******************************************************************************/

int main (void)
{
    unsigned int ret;
    
    printf("Start to read channel values");
    /* Initialize example */
    LOCAL_exampleInit();
	int count = 0;
    while(1)
	{
		for(int i = 0; i < 50; i ++)
		{
			for(int j = 0; j < 7; j++)
			{
				unsigned long n = (1000 + i*20)*200;
				int nPw, nFw;
				nPw = 1500 * 200; 
				nFw = 20000 * 200;
				
				void * pPulseW, * pFrameW;
				pPulseW = sharedMem + 0x104 + j * 0x10;
				pFrameW = sharedMem + 0x108 + j * 0x10;
				nPw =*(unsigned long *) pPulseW;
				nFw =*(unsigned long *) pFrameW;
				pPulseW = sharedMem + 0x200 + j * 0x10;
				pFrameW = sharedMem + 0x204 + j * 0x10;
				*(unsigned long *) pPulseW = nPw;
				*(unsigned long *) pFrameW = nFw;
				printf("%8d", nPw);
			}
			usleep(10000);
			printf("\r");
		}
	}
    munmap(ddrMem, 0x400);
    close(mem_fd);

    return(0);
}

/*****************************************************************************
* Local Function Definitions                                                 *
*****************************************************************************/

static int LOCAL_exampleInit (  )
{

    /* open the device */
    mem_fd = open("/dev/mem", O_RDWR);
    if (mem_fd < 0) {
        printf("Failed to open /dev/mem (%s)\n", strerror(errno));
        return -1;
    }	

    /* map the shared memory */
    sharedMem = mmap(0, 0x400, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, 0x4a310000);
    if (sharedMem == NULL) {
        printf("Failed to map the device (%s)\n", strerror(errno));
        close(mem_fd);
        return -1;
    }
    

    return(0);
}

