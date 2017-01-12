#include "super.h"
#include <time.h>


int main()
{
	unsigned int cpt, vol, nb_bloc_libre, bloc, nb_bloc_liberer;
	
	printf("Test des blocs libres\n");
	
	// initialisation du generateur de nombre aleatoire
	srand(time(NULL));
	
	// initialisation du disque et du mbr
	init_master();
	load_mbr();

	// on verifie que le mbr est bien initialise
	check_mbr();
	vol = get_current_volume();
	
	// Initialisation du superbloc
	init_super(vol, "Mon disque", 1);

	// On verifie que le superbloc est bien initialise
	check_super();
	
	nb_bloc_libre = super.nb_blocs_libres;
	
	printf("Appel de new_bloc jusqu'a ce qu'elle retourne une erreur.\n");
	
	while((bloc = new_bloc()) != 0)
	{
		printf("Nouveau bloc alloue: bloc n°%d\n", bloc);
	}
	
	printf("Affichage de l'etat du disque.\n");
	
	if(super.nb_blocs_libres == 0)
	{
		printf("Le disque est plein.\n");
	}
	else
	{
		printf("Le disque n'est pas plein.\n");
	}
	
	printf("Liberation d'un nombre aleatoire de bloc.\n");
	
	// nb_bloc_libre = nombre maximal de bloc de depart
	nb_bloc_liberer = rand() % nb_bloc_libre;
	
	while(nb_bloc_liberer){
		free_bloc(nb_bloc_liberer);
		printf("Le bloc n°%d a ete libere.\n", nb_bloc_liberer);
		nb_bloc_liberer--;
	}
	
	printf("Affichage de l'etat du disque.\n");
	
	printf("Il y a actuellement %d blocs libres.\n", super.nb_blocs_libres);
	
	printf("Allocation de blocs jusqu'à ce que le disque soit plein et affichage du nombre de blocs alloues.\n");
	
	cpt=0;
	
	while(super.nb_blocs_libres > 0){
		bloc = new_bloc();
		printf("Le bloc n°%d a ete alloue\n", bloc);
		cpt++;
	}
	
	printf("%d blocs ont ete alloues.\n", cpt);
	
	printf("Sauvegarde de l'etat du superbloc\n");
	save_super();
	
	return cpt;	
}
