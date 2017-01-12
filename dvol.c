#include "super.h"


static void display_volume()
{
  unsigned int i;
  struct vol_s vol;
  printf("Il y a %d partition(s)\n", mbr.nombre_volume);
  for(i = 0; i< mbr.nombre_volume; i++){
    vol = mbr.vols[i];
    printf("vol n°%d, commence (%d,%d) taille du volume : %d blocs type : %d\n", i, vol.premier_cylindre, vol.premier_secteur, vol.nombre_blocs, vol.type);
  }
}

int main()
{

  init_master();
  load_mbr();
  display_volume();
  
  exit(EXIT_SUCCESS);
}
