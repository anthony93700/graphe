#include <stdlib.h>
#include <stdio.h>
#include "graphe.h"

int main(){
	struct grapheMA m;
	FILE * f = NULL;
	f = fopen("fichier.txt","r+");
	int num= 5,sommet = 0;
	lecture(&m,f);
	while(num!=0){
		printf("saisir un nombre.\n1 pour afficher le graphe\n2 pour créer le fichier dot\n3 pour appliquer l'algorithme DPS en ordre suffixe\n4 appliquer l'algorithme de DPS en ordrePrefixe\n5 pour appliquer Kosaraju\n0 pour quitter\n");
		scanf("%d",&num);
		switch(num){
			case 0:
			break;
			case 1:
				affichage(m);
			break;
			case 2:
				makeFileDot(m);
			break;
			case 3:
				printf("sommet de départ :");
				scanf("%d",&sommet);
				printf("\n");
				DPS(m,sommet,FALSE);
			break;
			case 4:
				printf("sommet de départ :");
				scanf("%d",&sommet);
				printf("\n");
				DPS(m,sommet,TRUE);
			break;
			case 5:
			
				Kosaraju_sharir(m);
			break;
			default:
				printf("vous avez tapez une mauvaise commande \n");
			break;
		}
	}
	fclose(f);	
	return 0;
}
