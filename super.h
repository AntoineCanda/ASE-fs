#ifndef SUPER_H
#define SUPER_H

#include "mbr.h"

#define SUPERMAGIC 0xDEADBEEF
#define MAGICBLOCLIBRE 0xBEADBEEF



struct superbloc_s {
	unsigned int magic;
	unsigned int num_serie;
	char nom[33];
	unsigned int racine;
	unsigned int premier_bloc_libre;
	unsigned int nb_blocs_libres;
};

struct bloc_libre_s {
	unsigned int magic;
	unsigned int suivant;
};

extern struct superbloc_s super;

void check_super();

void init_super(unsigned int vol, const char* nom, unsigned int serie);
int load_super(unsigned int vol);
void save_super();

unsigned int new_bloc();
void free_bloc(unsigned int bloc);

void read_bloc_libre(unsigned int bloc, struct bloc_libre_s *bl);
void write_bloc_libre(unsigned int bloc, struct bloc_libre_s *bl);

#endif
