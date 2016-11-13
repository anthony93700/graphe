#include "graphe.h"

void lecture(struct grapheMA* pm,FILE* f){
	int i = 0,y = 0,nb,r,t;	
	fscanf(f,"%d",&(*pm).n);
	fscanf(f,"%d",&nb);
	printf("%d",nb);
	for(i = 0;i<(*pm).n;i++){
		//(*pm).sousgrapheConnexe[i] = 0;
		for(y=0;y<(*pm).n;y++){
			(*pm).adj[i][y] = 0;
			(*pm).poids[i][y] = 0;
		}
	}
	for(i = 0;i<nb;i++){
		fscanf(f,"%d",&t);
		fscanf(f,"%d",&r);
		fscanf(f,"%d",&(*pm).poids[t][r]);
		(*pm).adj[t][r]=1;
	}
}
struct grapheMA init(int n){
	struct grapheMA gi;
	int y=0,i=0;
	gi.n = n;
	for(y=0;y<n;y++){
		for(i = 0;i<n;i++){
			gi.adj[i][y] = 0;
			gi.poids[i][y] = 0;
		}
	}
	return gi;
}
void affichage(struct grapheMA m){
	system("xdg-open graphe.pdf");
}
void makeFileDot(struct grapheMA m){
	FILE* f = NULL;
	f = fopen("graphe.dot","w");
	if(f==NULL){
		exit(-1);
	}
	fprintf(f,"graph mon_graphe {\n");
	int y=0,i=0;
	for(i = 0;i<NMAX;i++){
		for(y=0;y<YMAX;y++){
			if(m.adj[i][y]!=0){
				fprintf(f,"%d -- %d;\n",i,y);			
			}		
		}
	}
	fprintf(f,"}");
	fclose(f);
	printf("le fichier .dot a été crée\n");
	system("dot -Tpdf graphe.dot > graphe.pdf");
	printf("le fichier .pdf a été crée\n");
}
struct grapheMA inverserAdj(struct grapheMA m){
	int i = 0,y = 0;
	struct grapheMA gi;// initialiser le graphe de m.n sommets
	gi = init(m.n);
	for(i=0;i<m.n;i++){
		for(y=0;y<m.n;y++){
			gi.adj[y][i]=m.adj[i][y];
		}
	}
	return gi;
}
void ExplorePrefixe(struct grapheMA m,int sommet,int marquer[],int *incr,int ordreprefixe[]){
	int i = 0,y=0,sf;
	marquer[sommet]=1;
	printf("sommet :%d\n",sommet);//prefixe
	ordreprefixe[*incr] = sommet;
	(*incr)++;
	for(y=0;y<m.n;y++){
		if(m.adj[sommet][y]!=0&&marquer[y]==0){
			ExplorePrefixe(m,y,marquer,incr,ordreprefixe);
		}
	}
	//printf("sommet :%d\n",sommet);//suffixe
}
void ExploreSuffixe(struct grapheMA m,int sommet,int marquer[],int *incr,int ordresuffixe[]){
	int i = 0,y=0,sf;
	marquer[sommet]=1;
	//printf("sommet :%d\n",sommet);//prefixe
	(*incr)++;
	for(y=0;y<m.n;y++){
		if(m.adj[sommet][y]!=0&&marquer[y]==0){
			ExploreSuffixe(m,y,marquer,incr,ordresuffixe);
		}
	}
	printf("sommet :%d\n",sommet);//suffixe
	ordresuffixe[*incr] = sommet;
}
void Kosaraju_sharir(struct grapheMA m){
	int tab[NMAX][YMAX],sommet,s;
	struct grapheMA gi;
	printf("début de l'algorithme de kosaraju_sharir\n");
	gi = inverserAdj(m);
	printf("premier DPS pour le graphe principal\n");
	s = DPS(m,0,FALSE);
	printf("Deuxiemme DPS pour le graphe inverser\n");
	DPS(gi,s,FALSE);
	printf("fin de l'algorithme de Korasaju");
} 
int DPS(struct grapheMA m,int sommet,int b){
	int marquer[m.n],i=0,incr = 0;
	int ordreprefixe[NMAX];
	int ordresuffixe[NMAX];
	for(i=0;i<m.n;i++){
		marquer[i]=0;
	}
	if(b){
		ExplorePrefixe(m,sommet,marquer,&incr,ordreprefixe);
		for(i=0;i<m.n;i++){
			if(marquer[i]==0)
				ExplorePrefixe(m,i,marquer,&incr,ordreprefixe);
		}
		return 	ordreprefixe[incr];
	}else{
		ExploreSuffixe(m,sommet,marquer,&incr,ordresuffixe);
		for(i=0;i<m.n;i++){
			if(marquer[i]==0)
				ExploreSuffixe(m,i,marquer,&incr,ordresuffixe);
		}
		return 	ordresuffixe[incr];
	}			
}
void ordre_Topologique(struct grapheMA m){

	int i=0,y=0, mark=0, valTri = 0;
	int flag = 0;

	int tri_topo[m.n];

	/* On prend chaque colonne du graphe pour vérifier les prédécesseurs */
	for(i=0; i<m.n; i++){

		flag = 0;	

		for(y=0; y<m.n; y++){
			
			/* Si une autre valeur que -1 en prédécésseur est trouvée */
			if(m.adj[y][i] != -1){
				flag = 1;
				break;
			}
	
		}

		/* Cas ou il n'y a pas de prédécésseurs dans la colonne */
		if(flag == 0){

			tri_topo[valTri] = y;

			for(i=0; i<m.n; i++){
				for(y=0; y<m.n; y++){
					
					

					if(y == valTri){
						m.adj[valTri][i] = 0;
					}
				}
			}

			valTri++;		
	
		}
	}

}	
}
