#include "mbr.h"

struct mbr_s mbr;

// Test si le MBR est initialise
void check_mbr() {
  if(mbr.magic != MBRMAGIC) {
    printf("erreur : MBR non initialise \n");
    exit(EXIT_FAILURE);
  }
}


 
// Sauvegarde le MBR en position (0,0) du disque
void save_mbr(){

	unsigned char buf[HDA_SECTORSIZE];
	assert(sizeof(struct mbr_s) <= HDA_SECTORSIZE);
	memset(buf+sizeof(struct mbr_s), 0 , HDA_SECTORSIZE - sizeof(struct mbr_s));
	memcpy(buf, &mbr, sizeof(struct mbr_s));
	write_sector(0,0, buf);

}

// On charge le MBR du disque dans la variable global mbr. Si il est non initialise, on en cree un. 
void load_mbr(){

	unsigned char buf[HDA_SECTORSIZE];
	assert(sizeof(struct mbr_s) <= HDA_SECTORSIZE);
	read_sector(0,0,buf);
	memcpy(&mbr, buf, sizeof(struct mbr_s));

	if(mbr.magic != MBRMAGIC){
		memset(&mbr, 0 , sizeof(struct mbr_s));
		mbr.magic = MBRMAGIC;
		printf ("Initialisation du MBR\n");
	}
}

// Fonction de conversion qui a partir d'un numero de volume et de bloc retourne le cylindre ou se trouve le bloc sur le disque. 
int bloc_to_cyl(unsigned int volume, unsigned int num_bloc){
	
	struct vol_s vol = mbr.vols[volume];
	assert((volume < mbr.nombre_volume) && (num_bloc < vol.nombre_blocs));

	int res = 0;

	res = vol.premier_cylindre + (vol.premier_secteur + num_bloc)/NB_MAX_SECTOR;
	return res;

}

// Fonction de conversion qui a partir d'un numero de volume et de bloc retourne le secteur ou se trouve le bloc sur le disque. 
int bloc_to_sec(unsigned int volume, unsigned int num_bloc){

	struct vol_s vol = mbr.vols[volume];
	assert((volume < mbr.nombre_volume) && (num_bloc < vol.nombre_blocs));

	int res = 0;

	res =  (vol.premier_secteur + num_bloc) % NB_MAX_SECTOR;
	return res;

}

// Fonction qui lit le bloc nbloc du volume volume. 
void read_bloc(unsigned int volume, unsigned int nbloc, unsigned char *buffer) {
  unsigned int cyl, sec;

  assert(volume < NBMAXVOLS);
  
  struct vol_s vol = mbr.vols[volume];

  assert(nbloc < vol.nombre_blocs);
  
  cyl = bloc_to_cyl(volume, nbloc);
  sec = bloc_to_sec(volume, nbloc);
  
  read_sector(cyl, sec, &buffer[0]);
}

// Fonction qui ecrit le contenu de buffer sur le bloc nbloc du volume volume. 
void write_bloc(unsigned int volume, unsigned int nbloc, const unsigned char *buffer) {
  unsigned int cyl, sec;

  assert(volume < NBMAXVOLS);

  struct vol_s vol = mbr.vols[volume];

  printf("nbloc: %d, nombre_blocs : %d\n", nbloc, vol.nombre_blocs);
  assert(nbloc < vol.nombre_blocs);
  
  cyl = bloc_to_cyl(volume, nbloc);
  sec = bloc_to_sec(volume, nbloc);
  
  write_sector(cyl, sec, &buffer[0]);
}

// Fonction qui formate le volume volume
void format_vol(unsigned int volume) {
  unsigned int cyl, sec, nb;

  assert(volume < NBMAXVOLS);

  cyl = bloc_to_cyl(volume, 0);
  sec = bloc_to_sec(volume, 0);
  
  struct vol_s vol = mbr.vols[volume];

  nb = vol.nombre_blocs;
  
  format_sector(cyl, sec, nb , 0);
  
}



unsigned int get_current_volume(){
	char *vol;

	vol = getenv("CURRENT_VOLUME");
	assert(vol);

	return atoi(vol);
}
