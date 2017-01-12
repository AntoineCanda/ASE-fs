#include "super.h"


static int create_volume(int nombre_bloc, int premier_cyl, int premier_sec)
{
	struct vol_s vol;
	unsigned int i;
	int erreur = 0, tmpc, tmps, posDebutAvantDebut, posFinAvantDebut, posDebutApresFin;
	
	if(0 >= premier_cyl || premier_cyl >= NB_MAX_CYLINDER) {
		printf("valeur incorrecte premier_cyl : 0 < premier_cyl < %d (max cylinder)\n", NB_MAX_CYLINDER);
		return 1;
	}
  
    	if(0 >= premier_sec || premier_sec >= NB_MAX_SECTOR) {
		printf("valeur incorrecte premier_sec : 0 < premier_sec < %d (max sector)\n", NB_MAX_SECTOR);
		return 1;
  	  }

   	if(0 >= nombre_bloc || nombre_bloc >= NB_MAX_SECTOR*NB_MAX_CYLINDER) {
		printf("valeur incorrecte nombre_bloc : 0 < nombre_bloc < %d (max capacity)\n", NB_MAX_SECTOR*NB_MAX_CYLINDER);
		return 1;
    	}
	
	for(i = 0; i <= mbr.nombre_volume; i++){  
		vol = mbr.vols[i];
    
    /* dernier cylindre utilisé par le volume */
		tmpc = (int)vol.premier_cylindre + ((int)vol.premier_secteur + (int)vol.nombre_blocs - 1)/NB_MAX_SECTOR ;
    
    /* dernier secteur utilisé par le volume */
		tmps = ((int)vol.premier_secteur + (int)vol.nombre_blocs -1 ) % NB_MAX_SECTOR;
		
	
	/* test de positionnement */
		posDebutAvantDebut = (premier_cyl == (int)vol.premier_cylindre && premier_sec < (int)vol.premier_secteur) || (premier_cyl < (int)vol.premier_cylindre);
			
		posFinAvantDebut =  ((premier_cyl + (premier_sec + nombre_bloc - 1)/NB_MAX_SECTOR) == (int)vol.premier_cylindre && ((premier_sec + nombre_bloc -1)%NB_MAX_SECTOR) < (int)vol.premier_secteur) || ((premier_cyl + (premier_sec + nombre_bloc - 1)/NB_MAX_SECTOR) < (int)vol.premier_cylindre);
		 	
		posDebutApresFin = (premier_cyl == tmpc && premier_sec > tmps) || (premier_cyl > tmpc);
	   
		/* on test que soit le debut et la fin du nouveau volume soit avant le debut de l'ancien, soit apres la fin de l'ancien */
		if(!((posDebutAvantDebut && posFinAvantDebut ) || posDebutApresFin)){
		  erreur = 1;
		}
    	 }
	
   	 if(erreur == 1){
		printf("erreur, impossible de créer le volume\n");
		return 0;
    	 }
	
	    vol.premier_cylindre = premier_cyl;
	    vol.premier_secteur = premier_sec;
	    vol.nombre_blocs = nombre_bloc;
	    vol.type = BASE;
	    mbr.vols[mbr.nombre_volume] = vol;
	    mbr.nombre_volume++;

	    return 1;
}

int main(int argc, char **argv)
{
   int nombre_bloc = -1, premier_cyl = -1, premier_sec = -1;
  
	if(argc != 7)
	{
		printf("erreur d'arguments, usage :\n\t./mkvol -fc <premier cylindre> -fs <premier secteur> -s <nb de secteurs> \n");
		return EXIT_FAILURE;
	}
	else 
	{
		for(int i = 1; i < argc; i+=2) 
		{
			// Utilise strcmp pour comparer les flags et associer les arguments aux variables
			if(strcmp(argv[i],"-fc")==0)
			{
				premier_cyl = atoi(argv[i+1]);	
			}
			else if(strcmp(argv[i],"-fs")==0)
			{
				premier_sec = atoi(argv[i+1]);	
			}
			else if(strcmp(argv[i],"-s")==0)
			{
				nombre_bloc = atoi(argv[i+1]);	
			}
		}
	}
	printf("nombre bloc : %d , premier cyl : %d , premier sec : %d\n", nombre_bloc , premier_cyl, premier_sec);
	
  
	if(nombre_bloc == -1 || premier_cyl == -1 || premier_sec == -1){
		printf("erreur d'arguments, usage :\n\t./mkvol -fc <premier cylindre> -fs <premier secteur> -s <nb de secteurs> \n");
		return EXIT_FAILURE;
	}
  
	init_master();

	// init mbr
	load_mbr();

	
	// Possible de creer d'autre volume ?
	if(mbr.nombre_volume >= NBMAXVOLS){
		printf("nombre maximum de volume créé.\n");
		return EXIT_FAILURE;
	}
	

	// Creation volume 
	if(!create_volume(nombre_bloc, premier_cyl, premier_sec)){
		return EXIT_FAILURE;
	}
	
	// save mbr
	save_mbr();
		
	return EXIT_SUCCESS;
}
