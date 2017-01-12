#include "super.h"

struct superbloc_s super;

// Test si le superbloc est initialise
void check_super() {  
  if(super.magic != SUPERMAGIC) {
    printf("erreur : Super bloc non initialise \n");
    exit(EXIT_FAILURE);
  }
}

void init_super(unsigned int vol, const char* nom, unsigned int serie) {
	
	printf("nb_volume : %d\n", mbr.nombre_volume);
	assert(vol < mbr.nombre_volume);

	unsigned int vol_courant = vol;
	super.magic = SUPERMAGIC;
	super.num_serie = serie;
	strncpy(super.nom, nom, 33);
	super.nom[32] = '\0';
	super.racine = 1;
	super.premier_bloc_libre = 1;
	super.nb_blocs_libres = mbr.vols[vol_courant].nombre_blocs-1;
	save_super();

	struct bloc_libre_s bl;
	unsigned char buff[HDA_SECTORSIZE];
	memset(buff, 0, HDA_SECTORSIZE);
	
	for (unsigned int i=1; i<mbr.vols[vol_courant].nombre_blocs; i++) {
		bl.magic = MAGICBLOCLIBRE;
		bl.suivant = (i==mbr.vols[vol_courant].nombre_blocs) ? 0 : i+1;
		memcpy(buff, &bl, sizeof(struct bloc_libre_s));
		write_bloc(vol, i, buff);
	}
}

int load_super(unsigned int vol) {
	
	unsigned char buff[HDA_SECTORSIZE];
	assert(vol < mbr.nombre_volume);
	unsigned int vol_courant = vol;

	memset(buff, 0, HDA_SECTORSIZE);
	read_bloc(vol_courant, 0, buff);
	memcpy(&super, buff, sizeof(struct superbloc_s));
	if (super.magic != SUPERMAGIC) {
		return -1;
	} else {
		return 0;
	}
}

void save_super() {
	
	unsigned char buff[HDA_SECTORSIZE];
	unsigned int vol_courant = get_current_volume();
	memset(buff, 0, HDA_SECTORSIZE);
	assert(sizeof(struct superbloc_s) <= HDA_SECTORSIZE);
	memcpy(buff, &super, sizeof(struct superbloc_s));
	write_bloc(vol_courant, 0, buff);
}

unsigned int new_bloc() {

	struct bloc_libre_s bl;
	unsigned int bloc = super.premier_bloc_libre;
	
	// Superbloc initialise ?
	check_super();
	
	// Le premier bloc libre est il 0 ? Si oui il n'y a plus de bloc libre car 0 = superbloc qui ne sera jamais libre. 
	if (bloc !=0) {
		read_bloc_libre(bloc, &bl);
		super.nb_blocs_libres--;
		if(super.nb_blocs_libres == 0){
			super.premier_bloc_libre = 0;		
		}
		else
		{
			super.premier_bloc_libre = bl.suivant;
		}		
		save_super();
	}
	return bloc;
}

void free_bloc(unsigned int bloc) {
	
	struct bloc_libre_s bl;

	bl.magic = MAGICBLOCLIBRE;
	bl.suivant = super.premier_bloc_libre;	
	super.premier_bloc_libre = bloc;
	super.nb_blocs_libres++;
	write_bloc_libre(bloc, &bl);
	save_super();
}

void read_bloc_libre(unsigned int bloc, struct bloc_libre_s *bl){
	unsigned int vol_courant = get_current_volume();
	unsigned char buff[HDA_SECTORSIZE];

	memset(buff, 0, HDA_SECTORSIZE);
	assert(sizeof(struct superbloc_s) <= HDA_SECTORSIZE);
	read_bloc(vol_courant, bloc, buff);
	memcpy(bl, buff, sizeof(struct bloc_libre_s));
}

void write_bloc_libre(unsigned int bloc, struct bloc_libre_s *bl){
	unsigned int vol_courant = get_current_volume();
	unsigned char buff[HDA_SECTORSIZE];

	memset(buff, 0, HDA_SECTORSIZE);
	assert(sizeof(struct bloc_libre_s) <= HDA_SECTORSIZE);
	memcpy(buff, bl, sizeof(struct bloc_libre_s));
	write_bloc(vol_courant, bloc, buff);
}
