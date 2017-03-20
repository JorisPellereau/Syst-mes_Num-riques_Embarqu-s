// Commande de 2 servomoteurs en continu

#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>
#include "sys/alt_irq.h"
#include "system.h"

// prototypes fctn

void gestion_SWI( void * ptrg , int lvl_it);

int main() {

	// == INITIALISATION REGISTRES 	==
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWI_BASE , 0);				// RAZ du registre edge cap
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0);				// RAZ LEds
	// ===============================

	// == VARIABLES INTERNES ==
	void * ptrg;												// Pointeur generique
	int edge_cap , val ;
	long  duty_cycle_M1 , duty_cycle_M2 , clk_divide;
	clk_divide = 1000000;
	duty_cycle_M1 = 75000;
	duty_cycle_M2 = 75000;
	ptrg = (void *) val;										// Init ptrg
	// ========================

	// == AUTORISATION ITs ==
	alt_irq_register(2 , ptrg , gestion_SWI);					// 2 : num IT
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWI_BASE , 0x0F);			// IT sur les 4 BPs
	// ======================

	IOWR(PWM_32_1_BASE , 0 , clk_divide);			// clk_divide
	IOWR(PWM_32_2_BASE , 0 , clk_divide);			// clk_divide	
	IOWR(PWM_32_1_BASE , 1 , duty_cycle_M1);			// Duty cycle	
	IOWR(PWM_32_2_BASE , 1 , duty_cycle_M2);			// Duty cycle
	IOWR(PWM_32_2_BASE , 2 , 3);			// Enable 3 = Enable 
	IOWR(PWM_32_1_BASE , 2 , 3);			// Enable 3 = Enable 


	while(1) {
		edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(SWI_BASE);	// Lecture de EDGE CAP
		printf("edge_cap : %d \n" , edge_cap);
	}

	return 0;
}

void gestion_SWI( void * ptrg , int lvl_it) {
		long edge_cap , duty_cycle_M1 , duty_cycle_M2 , clk_divide;

		clk_divide = 1000000;									// Pour avoir une periode de 20 ms
		duty_cycle_M1 = 75000;										// Pour avoir 1.5 ms H => ARRET MOTEURS
		IOWR(PWM_32_1_BASE , 0 , clk_divide);			// clk_divide
		IOWR(PWM_32_2_BASE , 0 , clk_divide);			// clk_divide

		edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(SWI_BASE);	// Lecture de EDGE CAP
		if( (edge_cap & 0xFF) == 0x01) {						// Lecture SW0

			duty_cycle_M1 = 95000;
			duty_cycle_M2 = 55000;
			IOWR(PWM_32_1_BASE , 1 , duty_cycle_M1);			// Duty cycle
			IOWR(PWM_32_1_BASE , 2 , 3);			// Enable 3 = Enable
			IOWR(PWM_32_2_BASE , 1 , duty_cycle_M2);			// Duty cycle
			IOWR(PWM_32_2_BASE , 2 , 3);			// Enable 3 = Enable
			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 1);		// Pour test

		}
		else if((edge_cap & 0xFF) == 0x02) {					// Lecture SW1

			duty_cycle_M1 = 55000;
			duty_cycle_M2 = 95000;
			IOWR(PWM_32_1_BASE , 1 , duty_cycle_M1);			// Duty cycle
			IOWR(PWM_32_1_BASE , 2 , 3);			// Enable 3 = Enable
			IOWR(PWM_32_2_BASE , 1 , duty_cycle_M2);			// Duty cycle
			IOWR(PWM_32_2_BASE , 2 , 3);			// Enable 3 = Enable
			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 2);		// Pour test

		}
		else if((edge_cap & 0xFF) == 0x04) {					// Lecture SW2

			duty_cycle_M1 = 75000;
			duty_cycle_M2 = 75000;
			IOWR(PWM_32_1_BASE , 1 , duty_cycle_M1);			// Duty cycle			
			IOWR(PWM_32_2_BASE , 1 , duty_cycle_M2);			// Duty cycle
			IOWR(PWM_32_1_BASE , 2 , 0);			// Enable 0 = Disable
			IOWR(PWM_32_2_BASE , 2 , 0);			// Enable 0 = Disable
			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 3);
		}
		else if((edge_cap & 0xFF) == 0x08) {					// Lecture SW3

			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 4);
		}
		else {
			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0xAA);
		}
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWI_BASE , 0);				// RAZ du registre edge cap
}

