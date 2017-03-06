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

//void inc_dec_var(void* ptrg, int lvl_IT) ;
void v1_v2(void * ptrg , int lvl_IT);

int main()
{
	int v1 , v2 , tab_v1v2[2] = {0 , 0};				// Pour exo_3
	void* ptrg ;
	int cmpt , imask_button , edge_cap ;
	//ptrg = (void*)&cmpt ;
	ptrg = (void*)tab_v1v2;			// Chargemenent ptrg pr exo 3


	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,0);			// Force les leds à 0
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);		//RAZ Registre edge_capt
	//alt_irq_register(2 , ptrg , inc_dec_var);
	alt_irq_register(2 , ptrg , v1_v2);						// Creation sous routine IT

	imask_button = IORD_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_PIO_BASE);	// Lecture de IMAKS ici juste pr test
	imask_button = 512 + 15;
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_PIO_BASE , imask_button);	// Autorisation IT sur les 4 buttons
	while(1) {
		//edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);
		//printf("edge_cap : %d \n" , edge_cap);
		printf("tab_v1v2[0] : %d , tab_v1v2[1] : %d \n" ,tab_v1v2[0] , tab_v1v2[1]);		//
	}


  return 0;
}

void v1_v2(void * ptrg , int lvl_IT) {				// Evolution de V1 et V2 en fonction de 3 BPs
	int edge_cap , * ptr_v1v2;
	ptr_v1v2 = (int *)ptrg;						// Cast à l'envers =>>> BIEN METTRE EN MEME TYPE !!!!!
	edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);	// lecture de edge cap
	if( (edge_cap&0xFF) == 0x01) {	// Lecture de 	SW0 (ou SW3 suivant config port)	 // Recuperation appui sur BP
		*ptr_v1v2 = 1;		/// V1 = 1
		*(ptr_v1v2 + 1) = 0; // V2 = 0
	}
	else if( (edge_cap&0xFF) == 0x02) {	// Lecture de 	SW1 (ou SW2 suivant config port)	 // Recuperation appui sur BP
		*ptr_v1v2 = 0;		/// V1 = 0
		*(ptr_v1v2 + 1) = 1; // V2 = 1
	}
	else if( (edge_cap&0xFF) == 0x04) {	// Lecture de 	SW2 (ou SW1 suivant config port)	 // Recuperation appui sur BP
		*ptr_v1v2 = 0;		/// V1 = 0
		*(ptr_v1v2 + 1) = 0; // V2 = 0
	}
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);			// On RAZ le registre Edge Capt
}


/*

void inc_dec_var(void* ptrg, int lvl_IT)
{
		int* aff , edge_cap ;
		aff = (int*)ptrg ;
		edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);	// lecture de edge cap
		//IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , edge_cap&0xFF);
		if( (edge_cap&0xFF) == 0x08) {									// Recuperation appui sur BP
			*aff += 1;
		}
		else if((edge_cap&0xFF) == 0x04) {
			*aff -= 1;
		}
		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,*aff);
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);
}
*/
