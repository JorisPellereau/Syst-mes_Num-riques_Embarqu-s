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
#include <sys/alt_irq.h>

void ma_fct(void* ptrg, int lvl_IT) ;

int main()
{
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,0);
	/*int Data, Edge_Capture ;

	while(1)
	{
		Data = IORD_ALTERA_AVALON_PIO_DATA(BUTTON_PIO_BASE);
		Edge_Capture = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);
		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,Data);
		printf("Data = %d\n",Data);
		printf("Edge_Capture = %d\n",Edge_Capture);
		usleep(500000);
		if ((Data&15) == 15)
			IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);

	}*/
	void* ptrg ;
	int cmpt ;
	ptrg = (void*)&cmpt ;

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);//RAZ

	alt_irq_register(2,ptrg,ma_fct);
	while(1) ;


  return 0;
}

void ma_fct(void* ptrg, int lvl_IT)
{
		int* aff ;

		aff = (int*)ptrg ;
		*aff += 1;

		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,aff);

		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);
}
