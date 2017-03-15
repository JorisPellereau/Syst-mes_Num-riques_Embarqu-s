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
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>
#include <sys/alt_irq.h>
#include "system.h"



int main()
{
	// Init
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0);				// Ecriture des leds à 0
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE , 0);		// registre detection Edge à 0

	// INITIALISATION DES VARIABLES
	int reg_control , reg_sys_clk , snap_L , snap_H ;
	int data_reg_ctrl , cmpt = 0, status_clk , status_Led_IR , periodL , periodH;
	void * ptrg;					// Pointeur generique
	ptrg = (void *)&cmpt;			// Initi ptrg


	// Chargement de pêriode
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_LED_IR_BASE, 658);			// Pr signal de 38kHz : 1315 (sans bascule D)
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_LED_IR_BASE, 0x0000);			// Pr debug

	reg_control = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE);
	data_reg_ctrl = 0x03;		// => Start = 0 // CONT = 1 // IT = 1 => generation IT
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE , data_reg_ctrl);			// Mise en marche du Timer
	reg_control = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE);
	status_Led_IR = IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_LED_IR_BASE);
	printf("reg_ctrl_LEDIR : %d status_Led_IR : %d\n" , reg_control , status_Led_IR);

	printf("Avant lalt_irq_register \n");
	// Autorisation des ITs
	//alt_irq_register(3 , ptrg , tempo_s);			// Plante ICI !!


	data_reg_ctrl = 0x06;		// => Start = 1 // CONT = 1 // IT = 1 => generation IT
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE , data_reg_ctrl);



	printf("Avant le while 1");
	//IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE , 7);
	while(1) {
		reg_control = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE);
		printf("reg_ctrl_TImer_leds : %d\n" , reg_control);
		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_LED_IR_BASE, 111);			// Besoin d'une ecriture avant la lecture, sinon valeur à 0
		//IOWR_ALTERA_AVALON_TIMER_SNAPH(TIMER_LED_IR_BASE, 101);		// 1 seule ecriture suffit
		snap_L = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_LED_IR_BASE);
		snap_H = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_LED_IR_BASE);
		printf("snap_H_LEDIR : %d snap_L_LEDIR : %d \n" , snap_H , snap_L);
		status_Led_IR = IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_LED_IR_BASE);
		printf("status timer led : %d \n" , status_Led_IR);

	}
	return 0;
}