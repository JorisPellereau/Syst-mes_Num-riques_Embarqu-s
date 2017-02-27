// PELLEREAU Joris
// 27/02/2017
// Incrementation ou decrementation d'une variav=ble compteur via 2 BP géré en intérruption
//-- Librairies --
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <sys/alt_irq.h>
#include "system.h"

//-- Prototypes
void inc_dec_var(void* ptrg, int lvl_IT) ;


int main()
{	
		void* ptrg;															// Pointeur générique pour utiliser la fonction d'altera
		int cmpt , imask_button , edge_cap;
		ptrg = (void*)&cmpt;												// Chargement du pointeur
		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE,0);						// Ecriture des LEDS à 0
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE,0);					// RAZ du registre Edge Capture
		alt_irq_register(2 , ptrg , inc_dec_var);							// Creation de l'interruption

		imask_button = IORD_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_PIO_BASE);	// Lecture de IMASK
		imask_button = 512 + 15;
		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_PIO_BASE , imask_button);	// Autorisation IT 
		
		while(1) {
			edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE);
			printf("edge_cap : %d \n" , edge_cap);
		}


  return 0;
}

void inc_dec_var(void* ptrg, int lvl_IT)									// Fonction d'intérruption
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
