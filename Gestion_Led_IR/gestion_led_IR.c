#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>
#include "sys/alt_irq.h"
#include "system.h"
#include <stdio.h>

int main()
{
	int snap_L , snap_H , etat_ir;

	// == INITIALISATION REGISTRES 	==
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWI_BASE , 0);				// RAZ du registre edge cap
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0);				// RAZ LEds
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE , 8);	// Mise en stop du timer LED_IR + /IT
	// ===============================

	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0xAA);				// RAZ LEds

	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_LED_IR_BASE , 694);		// Pour generer un signal de f = 72 kHz
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_LED_IR_BASE , 0);
	// 1 bascule d pôur generer un carré à 36kHz
	// Start timer + mode cont + ok IT
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE , 7);
	while(1) {
		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_LED_IR_BASE , 111);
		snap_L = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_LED_IR_BASE);
		snap_H = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_LED_IR_BASE);
		//printf("snap_H : %d snap_L : %d \n" , snap_H , snap_L);
		etat_ir = IORD_ALTERA_AVALON_PIO_DATA(IR_PIO_BASE);
		printf("etat_ir = %d \n" , etat_ir);
		if((etat_ir&0x0F) == 0){
			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0);				// Led à 0
		}
		else if((etat_ir&0x0F) == 1) {
			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 1);				// Led à 1
		}
	}

  return 0;
}
