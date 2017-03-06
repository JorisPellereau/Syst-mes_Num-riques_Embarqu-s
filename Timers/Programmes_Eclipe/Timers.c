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
#include <sys/alt_irq.h>
#include "system.h"
#include <altera_avalon_timer_regs.h>

void tempo_2s(void * ptrg , int lvl_IT);

int main()
{
	// Init
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE , 0);

	int reg_control , reg_sys_clk , snap_L , snap_H ;
	int data_reg_ctrl , cmpt = 0, status_clk , status_Led_IR , periodL , periodH;
	void * ptrg;
	ptrg = (void *)&cmpt;
	// Chargement de pêriode
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_LED_IR_BASE, 0xE100);			// Pr debug
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_LED_IR_BASE, 0x05F5);			// Pr debug

	reg_control = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE);
	data_reg_ctrl = 0x07;		// => Start = 1 // CONT = 1 // IT = 1 => generation IT
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE , data_reg_ctrl);			// Mise en marche du Timer
	reg_control = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE);
	status_Led_IR = IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_LED_IR_BASE);
	printf("reg_ctrl_LEDIR : %d status_Led_IR : %d\n" , reg_control , status_Led_IR);

	printf("Avant lalt_irq_register");
	alt_irq_register(3 , ptrg , tempo_2s);			// Plante ICI !!



	printf("Avant le while 1");
	while(1) {



		printf("Tes ici !! \n");


		/*
		reg_control = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_LED_IR_BASE);
		reg_sys_clk = IORD_ALTERA_AVALON_TIMER_CONTROL(SYS_CLK_TIMER_BASE);
		printf("reg_ctrl LED_IR : %d reg_crtrl sys_clk : %d \n" , reg_control , reg_sys_clk);

		periodL = IORD_ALTERA_AVALON_TIMER_PERIODL(TIMER_LED_IR_BASE);
		periodH = IORD_ALTERA_AVALON_TIMER_PERIODH(TIMER_LED_IR_BASE);
		printf("periodH_LEDIR : %d periodL_LEDIR : %d \n" , periodH , periodL);

		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_LED_IR_BASE, 111);			// Besoin d'une ecriture avant la lecture, sinon valeur à 0
		IOWR_ALTERA_AVALON_TIMER_SNAPH(TIMER_LED_IR_BASE, 101);		// 1 seule ecriture suffit
		snap_L = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_LED_IR_BASE);
		snap_H = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_LED_IR_BASE);
		printf("snap_H_LEDIR : %d snap_L_LEDIR : %d \n" , snap_H , snap_L);


		/*
		IOWR_ALTERA_AVALON_TIMER_SNAPL(SYS_CLK_TIMER_BASE, 111);			// Besoin d'une ecriture avant la lecture, sinon valeur à 0
		IOWR_ALTERA_AVALON_TIMER_SNAPH(SYS_CLK_TIMER_BASE, 101);
		snap_L = IORD_ALTERA_AVALON_TIMER_SNAPL(SYS_CLK_TIMER_BASE);
		snap_H = IORD_ALTERA_AVALON_TIMER_SNAPH(SYS_CLK_TIMER_BASE);
		printf("snap_H_sys : %d snap_L_sys : %d \n" , snap_H , snap_L);

		status_clk = IORD_ALTERA_AVALON_TIMER_STATUS(SYS_CLK_TIMER_BASE);
		status_Led_IR = IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_LED_IR_BASE);
		printf("status_clk : %d status_Led_IR : %d\n" , status_clk , status_Led_IR);

		periodL = IORD_ALTERA_AVALON_TIMER_PERIODL(TIMER_LED_IR_BASE);
		periodH = IORD_ALTERA_AVALON_TIMER_PERIODH(TIMER_LED_IR_BASE);
		printf("periodH LEDIR : %d periodL LEDIR : %d \n" , periodH , periodL);

		*/



	}
	return 0;
}

void tempo_2s(void * ptrg , int lvl_IT){
	int * aff;
	aff = (int *)ptrg;
	*aff += 1;
	if(*aff == 255) {
		*aff = 0;
	}
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , *aff);
}
