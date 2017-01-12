## VERSCHAEVE Théo & CANADA Antoine

#   Un système de fichiers « à la Unix »

I   - Commande a executer 

	Pour compiler, dans le dossier source, utiliser la commande :
			make
	
	Pour executer la procedure de formatage du disque vu dans la couche 1 : 
			./frmt <valeur> 
			Si il n'y a pas de valeur fournie, il y a en valeur par defaut 0.
			
	Pour executer la procedure d'affichage d'un secteur a la position (x,y):
			./dmps <pos_cylindre> <pos_secteur>
			Si il n'y a pas de valeur fournie, on affiche par defaut le secteur de position (0,0).
	
	Pour executer la procedure d'affichage d'informations d'un/des volumes present sur le disque :
			./dvol
			
	Pour executer la procedure de creation d'un volume d'une taille donnee -s a partir des informations relative au cylindre -fc et secteur -fs :
			./mkvol -s <taille volume> -fc <pos du premier cylindre> -fs <pos du premier secteur>
			
	Pour afficher les informations relatives a une partition :
			./dfs 
			
	Pour creer une nouvelle partition :
			./mknfs

	Pour tester l'allocation et la liberation de blocs:
			./test_bloc_libre
			
	Il y a plusieurs scripts shell egalement present. Pour les executer, veuillez utiliser les commandes :
	
	script: permet de valider le TP sur new_bloc et free_bloc:  
			./script
	
II  - Presentation de chaque couche logicielle

	1 - Premiere couche logicielle : accès au matériel

	Dans cette premiere partie, nous avons du creer la bibliotheque drive.
	En autre elle contient les fonctions read_sector, write_sector et format_sector. 
	La validation minimale de cette bibliotheque est faite grace aux fonctions frmt et dmps qui utilisent les fonctions de la bibliotheque drive. 


	2 - Seconde couche logicielle : gestion de volumes

	Dans cette seconde partie, nous avons creer la bibliotheque mbr. 
	Cette bibliotheque contient les fonctions read_bloc, write_bloc et format_vol. Elle contient egalement deux premieres structures qui sont :
	 - mbr_s representant le mbr du disque (master boot record) contenant les differentes informations du disque comme le nombre de volume, et un tableau de volume avec le magic,
	 - vol_s representant un volume sur le disque avec notamment le premier cylindre et secteur de ce volume, le nombre de bloc et le type du volume (base, annexe ou autre).
	Nous avons des methodes specifiques au mbr qui sont save_mbr() qui va ecrire sur le disque en position (0,0) l'etat de la structure de donnee globale du MBR et son oppose qui est
	load_mbr() qui va a partir du MBR du disque en (0,0) charger une structure de donnee globale avec les donnees du MBR. 
	Les fonctions specifiques au volume seront les deux fonctions de conversion bloc_to_cyl et bloc_to_sec qui retourne les positions du cylindre et secteur sur le disque d'un bloc d'un volume.

	La validation minimale de cette gestion de volume est faite grace aux deux methodes mkvol qui creer un volume et dvol qui affiche les informations d'un volume. 

	3 - Troisieme couche logicielle : structure d'un volume / 1ere partie

	Dans cette troisieme partie, nous allons creer deux nouvelles structures de donnes qui sont superbloc_s et bloclibres_s representant le premier bloc d'un volume ou le bloc 0 ainsi que 
	la liste chainee des des blocs libres du volume avec la seconde structure. 
	Le superbloc contient differentes informations comme un magic, un numero de serie, un nom, un identifiant d'inoeud racine ainsi que la position du premier bloc libre et le nombre de blocs libres du volume.
	La structure bloclibres_s indique la position du bloc libre suivant et un magic. 

	Les fonctions ajoutees sont init_super qui va initialise le superbloc, load_super et save_super qui vont charger dans une structure globale le superbloc et le reecrire sur le disque apres modification.
	Il y a egalement pour les blocs libres les fonctions new_bloc et free_bloc. 

	Il est necessaire de creer deux programmes mknfs qui creer un nouveau système de fichier et un programme dfs qui l'affiche. 

	La validation minimale sera fait grace a un programme qui va utiliser new_bloc jusqu'a arriver a une erreur puis on verifiera l'etat du disque. 
	Ensuite on libere un nombre aleatoire de bloc puis on affiche le status du disque (nombre de bloc libre) pour enfin allouee a nouveau ces blocs et en comparant si on a bien l'egalite entre taille qui etait libre
	et bloc allouee. 
	
	Premiere etape de la creation d'un systeme de fichier avec la structure de l'inoeud. On peut en autre y creer un inoeud, en suprimmer un . On a egalement des fonctions utilitaires qui sont read_inode , write_inode.
	Enfin on a vbloc_of_fbloc qui a partir d'un numero de inoeud, un numero de bloc retourne le bloc sur le volume correspondant (place de facon lineaire). 
	
III - Suivi du travail effectue. 
	
	20/10/2016 : Premier TP sur le système de fichier à la UNIX. Durant le tp, nous avons commence a implanter la bibliotheque drive vu en cours. 
	03/11/2016 : Mise a jour de la couche 1 avec diverses corrections. 
	03/11/2016 : Durant le tp, nous avons commence a creer la couche 2 vu en cours et continuer a corriger la premiere couche.
	04/11/2016 : Suite du tp sur la couche 2 avec des corrections egalement de la premiere couche.
	09/11/2016 : Suite du tp. Ajout de methode comme check_hda qui verifie simplement que les informations obtenus par le disque avec la taille du secteur sont coherentes. 
	10/11/2016 : Durant le tp, debut de creation des fonctions de la couche 3 vu en cours. Plus correction de bug present.	
	11/11/2016 : Reorganisation complete du code afin de se rapprocher un maximum du polycopie. 
	En autre, renommage de fichier (create_volume.c en mkvol, display_vol.c en dvol.c, format.c en frmt.c), fusion des fichiers mbr.c et vol.c (de meme avec les .h). Retouche des .h.
	Correction et suite des TP precedents. 
	Ajout de methode comme init_master() faisant l'initialisation du disque maitre, check_mbr() et check_super() qui testent si le MBR et le superbloc sont initalises ou pas. 
	12/11/2016 : Suite 11/11/2016. Ajout de fonctions get_current_volume() et modification de init_master pour ajouter le fichier de configuration en variable d'environnement. (Encadré 1 page 7).
	Modification du Makefile. Ajout de dfs.c avec une premiere version de ce qu'il faut faire.
	16/11/2016: Suite TP. Corrections diverses. mkvol fonctionne et dvol egalement. Il reste des warnings a corriger. 
	Creation de test_bloc_libre.c selon l'encadre et d'un script shell. A verifier. 
	17/11/2016: Nombreuses corrections effectuees durant la seance sur tout le programme. Creation de ce qui a ete vu durant le TD du jour.
	21/11/2016: Correction new_bloc() => Dans le cas ou nombre de blocs libre dans le superbloc arrivait a 0, on ne mettait pas a jour le premier bloc libre a 0...
	Le script marche maintenant completement. 
	23/11/2016:  Separation de mbr.c (et .h) avec super.c et super.h. Debut d'harmonisation par rapport aux fichiers de la quatrieme couche.
	Renommage de blocs.c et blocs.h en inode.c et inode.h. Il y a eu de nombreuses corrections apportees. Compile sans erreur mais pas encore de programme pour valider et il manque la fonction vbloc_of_fbloc.. 
	24/11/2016:  Version non testee de vbloc_of_fbloc avec allocation. Ajout test sauvage sur new_bloc avec assert si egale a 0. 
	
IV - Resultat du travail, presentation des erreurs et difficultes rencontrees. 

	Nous n'avons pas eu les memes facilites a comprendre ce que nous devons faire et l'interet de le faire. Cela a forcement impacte sur le resultat final mais nous avons reussi 
	a faire plusieurs choses demandees neanmoins. Ce TP aura permis (ou pas) de se reconcilier un peu avec le C et le domaine du systeme d'exploitation qui avait peut être ete diabolise
	l'annee derniere par rapprot aux difficultes qu'on avait pu rencontrer. Nous avons au moins pu confirmer ou infirmer cette premiere impression.
	
	La premiere couche n'a pas pose specialement de probleme apres avoir pris le temps de comprendre comment fonctionner la bibliotheque d'acces au materiel.
	
	La seconde couche aura ete source de plus d'erreur a cause d'une erreur de comprehension par rapport a la structure mbr qui n'avait pas ete initialise en variable globale. 
	Nous avons perdu pas mal de temps a nous en rendre compte mais nous y sommes parvenus.
	
	La troisieme couche est assez similaire dans l'esprit a la seconde, on change juste de niveau, on reproduit globalement le mbr au niveau du volume (le superbloc).
	L'implementation aura ete aussi relativement delicate suite a des erreurs presentes dans les tp precedents mais petit a petit on a pu les corriger. 
	La validation par le script marche selon la specification.
	
	La fonction vbloc_of_fbloc pose actuellement un petit probleme. 
	Fonction tres importante avec enormement d'etape, tres facile d'en oublier une et de se retrouver avec une erreur...  
	
	
