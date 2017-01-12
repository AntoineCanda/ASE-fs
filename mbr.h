#ifndef MBR_H
#define MBR_H

#include "drive.h"

#define NBMAXVOLS 8
#define NB_MAX_SECTOR 16
#define NB_MAX_CYLINDER 16
#define MBRMAGIC 0xEFCACFFD



struct vol_s{
	unsigned int premier_cylindre;
	unsigned int premier_secteur;
	unsigned int nombre_blocs;
	enum voltype_e {BASE,ANNEXE,AUTRE} type;
};

struct mbr_s{
	unsigned int nombre_volume;
	struct vol_s vols[NBMAXVOLS];
	unsigned int magic;
};



extern struct mbr_s mbr;


void check_mbr();


void save_mbr();
void load_mbr();

int bloc_to_cyl(unsigned int volume, unsigned int num_bloc);
int bloc_to_sec(unsigned int volume, unsigned int num_bloc);

void read_bloc(unsigned int volume, unsigned int nbloc, unsigned char *buffer);
void write_bloc(unsigned int volume, unsigned int nbloc, const unsigned char *buffer);
void format_vol(unsigned int volume);


unsigned int get_current_volume();
#endif
