#include <stdio.h>
#include <stdlib.h>
//#include <altera_avalon_pio_reg.h>
//#include "system.h"

// Visualiser la difference entre les @ des types
// Visualisation sur les leds

// Prototypes fctn
void affich_tab_char(char *tab);
void affich_tab_int(int *tab);
void affich_tab_long(long *tab);
void affich_leds(void *pg);

int main(void)
{
	char tab_char[5] = { 1 , 2 , 3 , 4 , 5 } , cmpt;
	int tab_int[5] = { 100 , 2 , 3 , 4 , 5 };
	long tab_long[5] = { 1 , 2 , 3 , 4 , 5 };

	void * pg;
	printf("Affichare char \n");
	affich_tab_char(tab_char);
	printf("\n");
	printf("Affichage int \n");
	affich_tab_int(tab_int);
	printf("\n");
	printf("Affichage long \n");
	affich_tab_long(tab_long);
	printf("\n");
	pg = (void *)tab_int;
	affich_leds(pg);
	
	while(1) {
		for(cmpt = 0 ; cmpt <= 255 ; cmpt++) {
			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , cmpt );
			usleep(500000);		// Attente 1/2 s
		}
	}
	
	return 0;
}

void affich_leds(void *pg) {
	char *p;
	p = (char *)pg;
	printf("%d \n" , *p);
	IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE , *p );
}

void affich_tab_char(char *tab) {
	char *ptr , i;
	ptr = tab;
	for(i = 0 ; i < 5 ; i++) {
		printf("tab[%d] = %d \n" , i , *ptr);	// ou avec : printf("tab[%d] = %d" , i , *(ptr_tab + i)); sans ptr_tab++;		
		printf("Adresse tab[%d] : %d \n" , i , ptr);
		ptr++;
	}
}

void affich_tab_int(int *tab) {
        int *ptr;
	char i;
        ptr = tab;
        for(i = 0 ; i < 5 ; i++) {
                printf("tab[%d] = %d \n" , i , *ptr);   // ou avec : printf("tab[%d] = %d"$
                printf("Adresse tab[%d] : %d \n" , i , ptr);
                ptr++;
        }
}

void affich_tab_long(long *tab) {
        long *ptr;
	char i;
        ptr = tab;
        for(i = 0 ; i < 5 ; i++) {
                printf("tab[%d] = %d \n" , i , *ptr);   // ou avec : printf("tab[%d] = %d"$
                printf("Adresse tab[%d] : %d \n" , i , ptr);
                ptr++;
        }
}
