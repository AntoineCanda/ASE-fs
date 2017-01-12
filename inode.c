#include "inode.h"

void read_inode(unsigned int inumber, struct inode_s *inode) {
	unsigned char buff[HDA_SECTORSIZE];
	unsigned int vol_courant = get_current_volume();
	
	memset(buff, 0, HDA_SECTORSIZE);
	read_bloc(vol_courant, inumber, buff);
	memcpy(&inode, buff, sizeof(struct inode_s));

	if(inode->magic != MAGIC_INODE){
		memset(&inode, 0 , sizeof(struct inode_s));
		inode->magic = MAGIC_INODE;
		printf ("Initialisation de l'inode\n");
	}
}

void write_inode(unsigned int inumber, struct inode_s *inode) {
	
	unsigned char buff[HDA_SECTORSIZE];
	unsigned int vol_courant = get_current_volume();

	memset(buff, 0, HDA_SECTORSIZE);

	assert(sizeof(struct inode_s) <= HDA_SECTORSIZE);

	memcpy(buff, &inode, sizeof(struct inode_s));
	write_bloc(vol_courant, inumber, buff);
}

unsigned int create_inode(enum file_type_e type) {
	struct inode_s inode;
	unsigned int inumber = new_bloc();
	if (inumber != 0) {
		memset(&inode, 0, sizeof(struct inode_s));
		inode.type = type;
		inode.magic = MAGIC_INODE;
		write_inode(inumber,&inode);
	}
	return inumber;
}

void free_blocs(unsigned int blocs[], unsigned int nb_blocs) {
	for (unsigned int i=0; i<nb_blocs; i++) {
		if (blocs[i] != 0) {
			free_bloc(blocs[i]);
		}
	}
}

int delete_inode(unsigned int inumber) {
	
	unsigned int vol_courant = get_current_volume();
	unsigned int indirect[NB_BLOCS_PAR_BLOC];
	unsigned int dbl_indirect[NB_BLOCS_PAR_BLOC];
	
	struct inode_s inode;
	
	read_inode(inumber, &inode);
	free_blocs(inode.directs, NB_BLOCS_DIRECTS);
	
	if (inode.indirect != 0) {
		read_bloc(vol_courant, inode.indirect, (unsigned char *)&indirect);
		free_blocs(indirect, NB_BLOCS_PAR_BLOC);
		free_bloc(inode.indirect);
	}
	if (inode.dbl_indirect != 0) {
		read_bloc(vol_courant, inode.dbl_indirect, (unsigned char *)&dbl_indirect);
		for (unsigned int i=0; i<NB_BLOCS_PAR_BLOC; i++) {
			if (dbl_indirect[i] != 0) {
				read_bloc(vol_courant, dbl_indirect[i], (unsigned char *)&indirect);
				free_blocs(indirect, NB_BLOCS_PAR_BLOC);
				free_bloc(dbl_indirect[i]);
			}
		}
		free_bloc(inode.dbl_indirect);
	}
	free_bloc(inumber);
	
	return EXIT_SUCCES;
}

/*
unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc){
	struct inode_s inode;
	unsigned int indirect[NB_BLOCS_PAR_BLOC];
	unsigned int dbl_indirect[NB_BLOCS_PAR_BLOC];
	unsigned int vol_courant = get_current_volume();
	unsigned int bloc_indirection;

	read_inode(inumber, &inode);
	if(fbloc < NB_BLOCS_DIRECTS){
		if(inode.directs[fbloc] == 0){
			return 0;
		}
		else {
			return inode.directs[fbloc];
		}
	}
	fbloc -= NB_BLOCS_DIRECTS;
	if(fbloc < NB_BLOCS_PAR_BLOC){
		if(inode.indirect == 0){
			return 0;
		}
		read_bloc(vol_courant, inode.indirect,(unsigned char*)&indirect);
		if(indirect[fbloc] == 0){
			return 0;		
		}
		else {
			return indirect[fbloc];
		}
	}
	fbloc-=NB_BLOCS_PAR_BLOC;

	if(inode.dbl_indirect == 0){
		return 0;
	}
	else {
		bloc_indirection = fbloc/NB_BLOCS_PAR_BLOC;
		if(bloc_indirection > NB_BLOCS_PAR_BLOC){
			printf("Le bloc demandé est en dehors des blocs accessible via cet inoeud");
			exit(EXIT_FAILURE);
		}
		else{
			fbloc %= NB_BLOCS_PAR_BLOC;
			read_bloc(vol_courant, inode.dbl_indirect, (unsigned char *)&dbl_indirect);
			read_bloc(vol_courant, dbl_indirect[bloc_indirection], (unsigned char *)&indirect);
			if(indirect[fbloc] == 0){
				return 0;
			}
			else{
				return indirect[fbloc];
			}
		}
	}
}


On a pas reelement HDA_SECTORSIZE place sur un bloc si on tient compte du fait qu'on a la structure bl dessus ?
Utilse new_bloc ?
*/

unsigned int do_allocation(unsigned int tab[], unsigned int blocWrite, unsigned int fbloc){
	unsigned int bloc = new_bloc();
	assert(bloc != 0);
	unsigned int vol_courant = get_current_volume();

	tab[fbloc] = bloc;
	write_bloc(vol_courant, blocWrite , (unsigned char *) &tab);

	return bloc;
}

unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc, bool_t do_allocate){
	struct inode_s inode;
	unsigned int bloc;
	unsigned int indirect[NB_BLOCS_PAR_BLOC];
	unsigned int dbl_indirect[NB_BLOCS_PAR_BLOC];
	unsigned int vol_courant = get_current_volume();
	unsigned int bloc_indirection;

	read_inode(inumber, &inode);
	if(fbloc < NB_BLOCS_DIRECTS){
		if(inode.directs[fbloc] == 0){
			if(do_allocate){
				bloc = new_bloc();
				assert(bloc != 0);
				inode.directs[fbloc] = bloc;
				return bloc;
			}
			else{
				return 0;
			}
		}
		else {
			return inode.directs[fbloc];
		}
	}
	fbloc -= NB_BLOCS_DIRECTS;
	if(fbloc < NB_BLOCS_PAR_BLOC){
		if(inode.indirect == 0){
			if(do_allocate){
				bloc = new_bloc();
				assert(bloc != 0);
				inode.indirect = bloc;

				memset(&indirect,0,NB_BLOCS_PAR_BLOC);
				bloc = do_allocation(indirect, inode.indirect, fbloc);

				return bloc;
			}
			else{
				return 0;
			}
		}
		else{
			read_bloc(vol_courant, inode.indirect,(unsigned char*)&indirect);
			if(indirect[fbloc] == 0){
				if(do_allocate){
					bloc = do_allocation(indirect, inode.indirect, fbloc);
					return bloc;	
				}
				else{
					return 0;
				}		
			}
			else {
				return indirect[fbloc];
			}
		}
	}

	fbloc-=NB_BLOCS_PAR_BLOC;
	bloc_indirection = fbloc/NB_BLOCS_PAR_BLOC;
	fbloc %= NB_BLOCS_PAR_BLOC;

	if(inode.dbl_indirect == 0){
		if(do_allocate){
			bloc = new_bloc();
			assert(bloc != 0);
			inode.dbl_indirect = bloc;
			
			memset(&indirect,0,NB_BLOCS_PAR_BLOC);

			memset(&indirect,0,NB_BLOCS_PAR_BLOC);
			
			bloc = do_allocation(dbl_indirect, inode.dbl_indirect, bloc_indirection);

			bloc = do_allocation(indirect, bloc_indirection, fbloc);
			return bloc;
		}
		else{
			return 0;
		}
	}
	else {
		

		if(bloc_indirection > NB_BLOCS_PAR_BLOC){
			printf("Le bloc demandé est en dehors des blocs accessible via cet inoeud");
			exit(EXIT_FAILURE);
		}
		else{

			read_bloc(vol_courant, inode.dbl_indirect, (unsigned char *)&dbl_indirect);

			if(dbl_indirect[bloc_indirection] == 0){
				if(do_allocate){

					bloc = do_allocation(dbl_indirect, inode.dbl_indirect, bloc_indirection);

					bloc = do_allocation(indirect, bloc_indirection, fbloc);
					return bloc;
				}
				else{
					return 0;
				}
			}
			else{
				read_bloc(vol_courant, dbl_indirect[bloc_indirection], (unsigned char *)&indirect);
				if(indirect[fbloc] == 0){
					if(do_allocate){
						bloc = do_allocation(indirect, bloc_indirection, fbloc);
						return bloc;
					}
					else{
						return 0;
					}
				}
				else {
					return indirect[fbloc];
				}
			}
		}
	}
}

