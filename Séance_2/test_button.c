/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "system.h"
#include <altera_avalon_pio_regs.h>

int main()
{
	int Data, Edge_Capture ;
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,0);
	while(1)
	{
		Data = IORD_ALTERA_AVALON_PIO_DATA(BUTTON_PIO_BASE);
		Edge_Capture = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);
		printf("Data = %d\n",Data);
		printf("Edge_Capture = %d\n",Edge_Capture);
		usleep(500000);

	}

  return 0;
}
