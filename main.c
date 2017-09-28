/*
 * main.c
 *
 *  Created on: 2017 Jan 13 09:06:06
 *  Author: CMP
 */




#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */
	

/*  This program illustrates the following embedded software concepts:
 *
 * 		1. How to use memory-mapped I/O to access the peripheral registers of the XMC1100 microcontroller.
 *  	2. How to use polling to detect events, in this case, zero-to-one transitions on P2.0.
 *
 *  The program reads the digital input level on Port 2 pin 0 (P2.0).
 *
 *  It measures how long the P2.0 is at logic one using a crude counter variable.  No measures are taken
 *  to detect or prevent counter overflows!!
 *
 *  The latest count value is saved to the global variable lastCount.
 *
 *                          <- count ->
 *                          ___________               ____
 *                          |          |              |   |
 *                          |          |              |   |
 *                          |          |              |   |
 *           P2.0  ----------          ---------------     -----------
 *
 *  One of the CCU4 Timers is configured to generate a square wave on P2.0 using its PWM mode.
 *  P2.0 is actually an output but its current value can be read from the Port 2 Input register
 *  allowing it to be used as a simulated input.  This eliminates the need for an external signal
 *  generator or switch, making it easy to run on the XMC 2GO board.
 */

// Some global variables

unsigned char PWM_pin;
int32_t count;
int32_t lastCount = 0;
unsigned char LED_Pin;

int main(void)
{
  DAVE_STATUS_t status;


		status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if(status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  // First, configure Port 1 Pin 0 as a general purpose output to drive the LED.
  // This is done by setting the output type in the IO Control Register IOCR0 for Port 1.

 // PORT1->IOCR0 = PORT1->IOCR0 | ( (PORT1_IOCR0_PC0_Msk & 0x10) << PORT1_IOCR0_PC0_Pos );

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {


	  	  // An example of how to read Port 2 pin 0 (P2.0) status

	  	//  PWM_pin = PORT2->IN & (PORT2_IN_P0_Msk | PORT2_IN_P4_Msk);     		// non-zero result if P2.0 = 1, else zero.

					PWM_pin = DIGITAL_IO_GetInput( &exSwitch );
		
	  	  // Measure how long P2.0 is high, but first synchronise on the next zero-to-one edge

	  	  // wait for the next zero to one transition of P2.0

	  	//  if ( !(PORT2->IN & PORT2_IN_P0_Msk) )				// if P2.0 currently zero
				if (!DIGITAL_IO_GetInput( &exSwitch))
				while ( !(PWM_pin=DIGITAL_IO_GetInput( &exSwitch)));		// loop until next edge transition
	  	  else
	  	  {													// P2.0 currently one, but may be mid-cycle!
	  		  while ( DIGITAL_IO_GetInput( &exSwitch) );		// loop while still one
	  	      while (!(PWM_pin=DIGITAL_IO_GetInput( &exSwitch)));		// now loop while zero until next rising edge
	  	  }

	  	  // Use "count" variable as rough indication of signal high period.

	  	  count = 0;
	  	  while (DIGITAL_IO_GetInput( &exSwitch) ) count++;	// keep counting while P2.0 = 1

	  	  // Latch the last count value

	  	  lastCount = count;

	  	  // Toggle the LED on P1.0 of the XMC 2GO board

	  //	  PORT1->OUT = PORT1->OUT ^ PORT1_OUT_P0_Msk;
				DIGITAL_IO_ToggleOutput (&led);
				
			//	LED_Pin = PORT1->OUT & PORT1_OUT_P0_Msk;

				LED_Pin = DIGITAL_IO_GetInput(&led);
//		  the last instruction may also be written as
//		  PORT1->OUT ^= PORT1_OUT_P0_Msk;

  }
}
