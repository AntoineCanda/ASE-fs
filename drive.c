#include "drive.h"

void read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer){
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

	// Lecture de la donnee sur le secteur voulu
	
	// Passage du parametre nombre de secteur a lire, ici 1	
	_out(HDA_DATAREGS,  0x00);
	_out(HDA_DATAREGS+1,0x01);
	// Passage de la commande READ pour effectuer la lecture
	_out(HDA_CMDREG,CMD_READ);
	// Attente de l'interruption associee a la fin du deplacement de la tete
	_sleep(HDA_IRQ);

	// Ecriture dans le buffer

	// memcpy(void *dest, void *src, size_t taille) Copie plus rapidement
	memcpy(&buffer[0], &MASTERBUFFER[0], HDA_SECTORSIZE);

}

void write_sector(unsigned int cylinder, unsigned int sector,const unsigned char *buffer){
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

	// Ecriture de la donnee sur le secteur voulu
	memcpy(&MASTERBUFFER[0], &buffer[0], HDA_SECTORSIZE);
	// Passage du parametre nombre de secteur a ecrire, ici 1	
	_out(HDA_DATAREGS,  0x00);
	_out(HDA_DATAREGS+1,0x01);
	// Passage de la commande WRITE pour effectuer la lecture
	_out(HDA_CMDREG,CMD_WRITE);
	// Attente de l'interruption associee a la fin du deplacement de la tete
	_sleep(HDA_IRQ);

	// Lecture depuis le buffer pour ecriture effective

	// memcpy(void *dest, void *src, size_t taille)
}	


void format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value){
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

	// Formater les secteurs voulus
	
	// Passage du parametre nombre de secteur a formater, ici nsector sur 16 bits
	_out(HDA_DATAREGS, (nsector >> 8) & 0xFF);
	_out(HDA_DATAREGS+1, nsector & 0xFF);
	// Passage de la valeur a ecrire sur les secteurs, ici value sur 32 bits	
	_out(HDA_DATAREGS+2, (value >> 24) & 0xFF);
	_out(HDA_DATAREGS+3, (value >> 16) & 0xFF);
	_out(HDA_DATAREGS+4, (value >> 8) & 0xFF);
	_out(HDA_DATAREGS+5, value & 0xFF);

	// Passage de la commande FORMAT pour effectuer la lecture
	_out(HDA_CMDREG,CMD_FORMAT);

	// Attente de l'interruption associee a la fin du deplacement de la tete
	unsigned int nbSignalAttente = nsector;
	
	while(nbSignalAttente != 0){
		_sleep(HDA_IRQ);
		nbSignalAttente--;
	}
}

void check_hda (void) {
  int sectorsize;
  _out(HDA_CMDREG, CMD_DSKINFO);
  sectorsize = (_in(HDA_DATAREGS +4) << 8) +  _in(HDA_DATAREGS +5);
  assert( HDA_SECTORSIZE == sectorsize);
}

static void empty_it() {
    return;
}

void init_master(void) {
	char *config;
	int i;
	
	config = getenv("HW_CONFIG");
	if(init_hardware(config) == 0) {
		fprintf(stderr, "Error in hardware initialization\n");
		exit(EXIT_FAILURE);
	}
  
  /* Interrupt handlers */
	for(i=0; i<16; i++)
		IRQVECTOR[i] = empty_it;

  /* Allows all IT */
	_mask(1);
	check_hda();
}
