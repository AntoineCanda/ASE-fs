#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "drive.h"
#define HDA_IRQ 14
#define NB_MAX_SECTOR 16
#define NB_MAX_CYLINDER 16
#define TAILLE_SECTEUR 256
#define HDA_CMDREG      0x3F6      
#define HDA_DATAREGS    0x110  

static void empty_it(){
  return;
}

void dmps(unsigned int cylinder, unsigned int sector){


	unsigned char buffer[TAILLE_SECTEUR];

	read_sector(cylinder, sector, buffer);
	unsigned int i;
	for(i=0; i<TAILLE_SECTEUR; i++){
		printf("%02x ", buffer[i]);
	}
}

int main(int argc, char **argv){
  unsigned int i;
  unsigned int cylinder, sector;
  cylinder = sector = 0;
  
  /* init hardware */
  if(init_hardware("hardware.ini") == 0) {
    fprintf(stderr, "Error in hardware initialization\n");
    exit(EXIT_FAILURE);
  }

  /* Interreupt handlers */
  for(i=0; i<16; i++)
    IRQVECTOR[i] = empty_it;

  /* Allows all IT */
  _mask(1);

  if(argc == 3) {
	/* On convertit les parametres en entier */
    cylinder = atoi(argv[1]);
    sector = atoi(argv[2]);
  } else {
    printf("no parameter given, assuming cylinder 0 & sector 0\n");
  }
  
  dmps(cylinder, sector);

  printf("\n");

  /* and exit! */
  exit(EXIT_SUCCESS);
}

