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

void inc_dec_var(void* ptrg, int lvl_IT) ;


int main()
{
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,0);
	void* ptrg ;
	int cmpt , imask_button , edge_cap ;
	ptrg = (void*)&cmpt ;
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);//RAZ
	alt_irq_register(2 , ptrg , inc_dec_var);

	imask_button = IORD_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_PIO_BASE);	// Lecture de IMAKS
	imask_button = 512 + 15;
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_PIO_BASE , imask_button);	// Autorisation IT sur les 4 buttons
	while(1) {
		edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);
		printf("edge_cap : %d \n" , edge_cap);
	}


  return 0;
}

void inc_dec_var(void* ptrg, int lvl_IT)
{
		int* aff , edge_cap ;
		aff = (int*)ptrg ;
		edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);	// lecture de edge cap
		//IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , edge_cap&0xFF);
		if( (edge_cap&0xFF) == 0x08) {
			*aff += 1;
		}
		else if((edge_cap&0xFF) == 0x04) {
			*aff -= 1;
		}
		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,*aff);
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);
}
