
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>
#include <altera_avalon_spi_regs.h>
#include "sys/alt_irq.h"
#include "system.h"

//#define PIO_7SEG1_BASE 0x11090

void gestion_SWI( void * ptrg , int lvl_it);
void envoi_spi(int datas);
int lecture_spi();
void affich_segi(int nb, int mystr , int pt);

int main()
{
		// == INITIALISATION REGISTRES 	==
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWI_BASE , 0);				// RAZ du registre edge cap
		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 0);				// RAZ LEds
		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 255);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_7SEG1_BASE , 1);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_7SEG2_BASE , 128);
		//=====================




		// Variables internes
		void * ptrg;
		int edge_cap , spi_status, spi_control, spi_ss , spi_tx , spi_rx;
		int data_retour , i , j;
		// ================


		// == AUTORISATION ITs ==
		alt_irq_register(2 , ptrg , gestion_SWI);					// 2 : num IT
		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWI_BASE , 0x0F);			// IT sur les 4 BPs
		IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_0_BASE, 0);					// IT SPI + SS
		IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_0_BASE, 0xAA);
		// ======================

		//envoi datas
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_ENB_SPI_BASE , 1);			// Commande buffer direction data SPI SPI 1 : ECRITURE VERS SLAVE
		//---------


		IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_0_BASE, 0);



		while(1) {
			spi_status = IORD_ALTERA_AVALON_SPI_STATUS(SPI_0_BASE);
			spi_control = IORD_ALTERA_AVALON_SPI_CONTROL(SPI_0_BASE);
			spi_ss = IORD_ALTERA_AVALON_SPI_SLAVE_SEL(SPI_0_BASE);
			spi_rx = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_0_BASE);
			spi_tx = IORD_ALTERA_AVALON_SPI_TXDATA(SPI_0_BASE);
			/*
			printf("spi_status : %d\n" , spi_status);
			printf("spi_control : %d\n" , spi_control);
			printf("spi_rx : %d\n" , spi_rx);
			printf("spi_tx : %d\n" , spi_tx);
			printf("spi_ss : %d\n\n" , spi_ss);
			*/

			for(i = 0 ; i <= 9 ; i++) {
				for(j = 0 ; j <= 9 ; j++) {
					affich_segi(i,PIO_7SEG1_BASE , 1);
					affich_segi(j,PIO_7SEG2_BASE , 1);
					usleep(50000);
				}
			}

			// lecture config CPT TEMP
			//envoi_spi(0xAC);
			//data_retour = lecture_spi();
			//usleep(1);
		}

  return 0;
}


void affich_segi(int nb, int mystr , int pt ) {

	if( (nb == 1)) {
		if(pt == 1) IOWR_ALTERA_AVALON_PIO_DATA(mystr , (0xF3) & (0x7F) );
		else IOWR_ALTERA_AVALON_PIO_DATA(mystr , (0xF3) );
	}
	else if( (nb == 2)) {
		if(pt == 1) IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0xA4 & 0x7F);
		else IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0xA4);
	}
	else if( (nb == 3)) {
		if(pt == 1) IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0xA1 & 0x7F);
		else IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0xA1);
	}
	else if( (nb == 4)) {
		IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0x93);
	}
	else if( (nb == 5)) {
		IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0x89);
	}
	else if( (nb == 6)) {
		IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0x88);
	}
	else if( (nb == 7)) {
		IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0xE3);
	}
	else if( (nb == 8)) {
		IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0x80);
	}
	else if( (nb == 9)) {
		IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0x81);
	}
	else if( (nb == 0) ) {
		IOWR_ALTERA_AVALON_PIO_DATA(mystr , 0xC0);
	}
	else;
}

void envoi_spi(int datas) {
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_ENB_SPI_BASE , 1);			// Commande buffer direction data SPI SPI 1 : ECRITURE VERS SLAVE
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_0_BASE, datas);			// Ecrit direct
}

int lecture_spi() {
	int spi_rx;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_ENB_SPI_BASE , 0);			// Commande buffer direction data SPI SPI 1 : ECRITURE VERS SLAVE
	spi_rx = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_0_BASE);
	printf("spi_rx : %d\n" , spi_rx);
	return spi_rx;
}


void gestion_SWI( void * ptrg , int lvl_it) {

		long edge_cap;
		edge_cap = IORD_ALTERA_AVALON_PIO_EDGE_CAP(SWI_BASE);	// Lecture de EDGE CAP

		if( (edge_cap & 0xFF) == 0x01) {						// Lecture SW0

			IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_0_BASE, 0xAA);
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_ENB_SPI_BASE , 1);			// Commande buffer direction data SPI

		}
		else if((edge_cap & 0xFF) == 0x02) {					// Lecture SW1


			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , 2);		// Pour test

		}
		else if((edge_cap & 0xFF) == 0x04) {					// Lecture SW2


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
