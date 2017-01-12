#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "drive.h"
#include "hardware.h"
#define HDA_IRQ 14
#define NB_MAX_SECTOR 16
#define NB_MAX_CYLINDER 16
#define HDA_CMDREG      0x3F6      
#define HDA_DATAREGS    0x110  

static void empty_it(){
  return;
}


void frmt(unsigned int value){

	unsigned int cylinder, sector = 0;
	for(cylinder = 0; cylinder < NB_MAX_CYLINDER; cylinder++){
			format_sector(cylinder,sector,NB_MAX_SECTOR, value);
/*

			// Deplacement de la tete de lecture au secteur voulu
	
			// Passage des parametres cylinder et sector pour indiquer la position souhaitee	
			_out(HDA_DATAREGS,(cylinder >> 8 ) & 0xFF);
			_out(HDA_DATAREGS+1, cylinder & 0xFF);
			_out(HDA_DATAREGS+2, (sector >> 8 ) & 0xFF);
			_out(HDA_DATAREGS+3, sector & 0xFF);
			// Passage de la commande SEEK pour deplacer la tete de lecture
			_out(HDA_CMDREG,CMD_SEEK);
			// Attente de l'interruption associee a la fin du deplacement de la tete
			_sleep(HDA_IRQ);

			// Formater le secteur
	
			// Passage du nombre de secteur a formater, ici 1
			_out(HDA_DATAREGS, 0);
			_out(HDA_DATAREGS+1, 1);
			// Passage de la valeur a ecrire sur les secteurs, ici value sur 32 bits	
			_out(HDA_DATAREGS+2, (value >> 24) & 0xFF);
			_out(HDA_DATAREGS+3, (value >> 16) & 0xFF);
			_out(HDA_DATAREGS+4, (value >> 8) & 0xFF);
			_out(HDA_DATAREGS+5, value & 0xFF);

			// Passage de la commande FORMAT pour effectuer le formatage
			_out(HDA_CMDREG,CMD_FORMAT);

			// Attente de l'interruption associee a la fin du formatage

			_sleep(HDA_IRQ);
*/
		
	}
}

int main(int argc, char **argv){
unsigned int i;
unsigned int value;
value = 0;
  
  /* init hardware */
  if(init_hardware("hardware.ini") == 0) {
    fprintf(stderr, "Error in hardware initialization\n");
    exit(EXIT_FAILURE);
  }

  /* Interrupt handlers */
  for(i=0; i<16; i++)
    IRQVECTOR[i] = empty_it;

  /* Allows all IT */
  _mask(1);

  if(argc == 2){
	/* On convertit le parametre en entier */
	value = atoi(argv[1]); 
  }
  else {
	  printf("No parameter for the value, default value will be 0\n");
  }
  /* On formate */
  frmt(value);
  
  /* and exit! */
  exit(EXIT_SUCCESS);	
}
