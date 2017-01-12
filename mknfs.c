#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "super.h"


int main()
{
  int vol;

  
  /* init master drive and load MBR */  
  init_master();

  load_mbr();

  vol = get_current_volume();

  init_super(vol,"Mon volume", 1);

  printf("Système de fichier initialisé avec succès\n");
  
  return EXIT_SUCCESS;
}
