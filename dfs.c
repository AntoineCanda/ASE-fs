#include "super.h"


// Premiere version qui affiche l'etat de la partition vol 
void print_info_super(int vol) {
	check_super();
	printf("Volume: %d\n Numero de serie: %d\n Nom: %s\n Position du premier bloc libre: %d\n Nombre de blocs libres: %d\n", vol, super.num_serie, super.nom, super.premier_bloc_libre, super.nb_blocs_libres);
}

int main()
{
	int vol;

	printf("======Display File System======\n");
  
	/* init master drive and load MBR */  
	init_master();

	load_mbr();
  
	vol = get_current_volume();

	load_super(vol);

	print_info_super(vol);

	return EXIT_SUCCESS;
}
