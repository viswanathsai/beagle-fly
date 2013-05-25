// this is demo of how to start ecap
// the program will start the ecap  and copy the timer count to 
// share ram of pru
// the current value of counter in shareram can be viewed by 
// devmem2 0x4a310000, the value should keep changing
 
.origin 0
.entrypoint PROG_INIT

// Address for the eCap (eCap)
#define ECAP_TIMER_ADDR         0x30000
#define ECCTL2_ADDR				0x30028
#define	ECCTL2_VALUE			0x100000		// flag to start time

PROG_INIT:
	// start the timer
	MOV R1, ECCTL2_ADDR		// copy ecap config register address
	MOV R2, ECCTL2_VALUE		// copy the same value to current frame Reg
	SBBO R2, R1, 0, 4		// set register of ECCTL2, the timer will start now
	
PROG_READ_TIMER:

	// create snap shot by copy input and timer counter
	MOV R1, ECAP_TIMER_ADDR		// copy current timer count address to R1
	LBBO R2, R1, 0, 4
	MOV R1, SHARE_RAM_ADDR		// load share memory address to r1
	SBBO R2, R1, 0, 4
	
	JMP PROG_READ_TIMER
	HALT