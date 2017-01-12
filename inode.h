#ifndef BLOCS_H
#define BLOCS_H
#define EXIT_SUCCES 1
#define MAGIC_INODE 0xDEADBEEF
#define NB_BLOCS_DIRECTS (HDA_SECTORSIZE - 4*sizeof(unsigned int) - sizeof(enum file_type_e)) / sizeof(unsigned int)
#define NB_BLOCS_PAR_BLOC (HDA_SECTORSIZE / sizeof(unsigned int))
#include "super.h"


struct inode_s {
	unsigned int taille;
	enum file_type_e{FILE_FILE,FILE_DIRECTORY,FILE_SPECIAL} type;
	unsigned int directs[NB_BLOCS_DIRECTS];
	unsigned int indirect;
	unsigned int dbl_indirect;
	unsigned int magic;
};

typedef enum { TRUE  = 1, FALSE = 0 } bool_t;

void read_inode(unsigned int inumber, struct inode_s *inode);
void write_inode(unsigned int inumber, struct inode_s *inode);
unsigned int create_inode(enum file_type_e type);
void free_blocs(unsigned int blocs[], unsigned int nb_blocs);
int delete_inode(unsigned int inumber);
//unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc);
unsigned int do_allocation(unsigned int tab[], unsigned int blocWrite, unsigned int fbloc);
unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc, bool_t do_allocate);

#endif
