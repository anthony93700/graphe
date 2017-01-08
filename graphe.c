#include "graphe.h"

void lecture(struct grapheMA* pm,FILE* f){
	int i = 0,y = 0,nb,r,t;	
	fscanf(f,"%d",&(*pm).n);
	fscanf(f,"%d",&(*pm).e);
	for(i = 0;i<(*pm).n;i++){
		//(*pm).sousgrapheConnexe[i] = 0;
		for(y=0;y<(*pm).n;y++){
			(*pm).adj[i][y] = 0;
			(*pm).poids[i][y] = 0;
		}
	}
	for(i = 0;i<(*pm).e;i++){
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
	fprintf(f,"digraph mon_graph {\n");
	int y=0,i=0;
	for(i = 0;i<NMAX;i++){
		for(y=0;y<YMAX;y++){
			if(m.adj[i][y]==1){
				fprintf(f,"\t%d -> %d [label=%d];\n",i,y,m.poids[i][y]);			
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
int max(int i,int y){
	if(i>y)
		return i;
	else
		return y;
}
/*#-- Etape 1, rechercher des noeuds sans predécesseur (NSP)
        NSP = []
        for name, n in self.__noeuds.items() :
            if n.degre_pred == 0 :
                NSP.append( n )
        #-- Etape 2, tant qu'il y a des NSP (NSP_i)
        while len( NSP ) :
        #--  a) chercher ses successeurs (S_i)
            NSP_i = NSP.pop()
            ordre.append( NSP_i )
            S = NSP_i.liste_succ()
        #--  b) pour chaque S_i de S, retirer le prédecesseur NSP_i
            for S_i in S :
                S_i.retirer_pred( NSP_i )
        #--  c) Si le degré_des_prédécesseurs de S_i = 0, alors ajouter S_i à NSP
                if S_i.degre_pred == 0 :
                    NSP.append( S_i )*/
int* tri_topologique(struct grapheMA m){
	int i=0,y = 0;
	int *d = NULL, *niveau =NULL,marquer = 0;
	int niveaufinal[m.n][m.n];
	d = malloc(m.n * sizeof(int));
	niveau = malloc(m.n * sizeof(int));
	for(i = 0;i<m.n;i++){
		d[i] = 0;
		for(y=0;y<m.n;y++){
			if(m.adj[y][i]==1)
				d[i]++;
		}
		niveau[i]=0;	
	}

	for(i=0;i<m.n;i++){
		if(d[i]==0){
			d[i]=-1;
			for(y = 0;y<m.n;y++){
				if(m.adj[i][y]==1){
					d[y]--;
					niveau[y]= max(niveau[y],niveau[i]+1);
				}
			}
		}
	}
		if(d[i]==-1){
			for(i=0;i<m.n;i++){
				if(d[i]==0){
					d[i]=-1;
					for(y = 0;y<m.n;y++){
						if(m.adj[i][y]==1){
							d[y]--;
							niveau[y]= max(niveau[y],niveau[i]+1);
						}
					}
				}
			}
		}
	for(i=0;i<(m).n;i++){
					printf("%d : %d\n",i,niveau[i]);
				}
	return niveau;
}
int tritopo(struct grapheMA *g, int *tabOrdre){

	int i;
	int *sommetRestant=NULL;
	sommetRestant= malloc(g->nbSommet * sizeof(int));


	//Matrice de nbSommet à 1
	for(i=0; i<g->n; i++){
		sommetRestant[i]=1;
	}



	int sommetChoisi;
	int j=0;
	do{

		sommetChoisi=sommetSansPred(g,sommetRestant);
		if(sommetChoisi!=-1){
			sommetRestant[sommetChoisi]=0;
			tabOrdre[j]=sommetChoisi;
			j++;
		}

	}
	while(tabVide(sommetRestant,g->n)==1 && sommetChoisi !=-1 );

	if(tabVide(sommetRestant,g->n)!=0)
		return 0;
	else
		return 1;

}

int sommetSansPred(struct grapheMA *g, int *sommetRestant){
	int i;
	int j;
	int test = 0;

	for(j=0; j< g->n; j++){

		for(i=0; i<g->n && test==0; i++){
			if(g->adj[i][j]==1 && sommetRestant[i]==1){
				test =1;
			}

		}
		if(test==0 && sommetRestant[j]==1)
			return j;

		test=0;
	}

	return -1;
}
int tabVide (int *tab, int nbSommet){

	int i;

	for(i=0; i<nbSommet; i++){
		if(tab[i]==1)
			return 1; //return tableau non vide donc contient au moins un sommet
	}

	return 0; // contient aucun sommet
}
void Bellman(struct grapheMA *g, int sommetDepart, int *tabOrdre){
    int nombreOperations = 0;

    int i, j, posSommetDepart;
	int *distance=malloc(g->n * sizeof(int));
    int *tabPredecesseur= malloc(g->n * sizeof(int));

	for(i=0; i<g->n; i++){
        nombreOperations++;
		distance[i]=INFINI;
		nombreOperations++;
		tabPredecesseur[i]=i;
		nombreOperations++;
		if(tabOrdre[i]==sommetDepart)
        {
            nombreOperations++;
            posSommetDepart=i;
            nombreOperations++;
        }
	}
	distance[posSommetDepart]=0;
	tabPredecesseur[posSommetDepart]=posSommetDepart;
	nombreOperations+=2;

	for(i=posSommetDepart; i<g->n; i++){
        nombreOperations++;
		for(j=0; j<g->n;j++){
		    nombreOperations++;
			if(g->adj[i][j] == 1 && (g->poids[i][j] + distance[i])<distance[j]){
			    nombreOperations+=3;
				distance[j]=(g->poids[i][j] + distance[i]);
                nombreOperations+=2;
				tabPredecesseur[j]=i;
				nombreOperations++;

			}
		}
	}
	int k;
    printf("les plus courts chemin a partir du sommet %d ont les valeurs suivantes: \n", sommetDepart+1);
    for(k=0;k<g->n;k++)
        printf("de %d au sommet %d : %d\n", sommetDepart+1, k+1, distance[k]);
    printf("l'execution du programme pour votre graphe a necessite %d operations\n", nombreOperations);
}
void BellmanFord(struct grapheMA* graph, int src){
	int V = (*graph).n;
	int E = (*graph).e;
	int i = 0,j = 0,z = 0;
	int dist[V];

	for (i = 0; i < V; i++){
		dist[i]   = INFINI;
	}
	dist[src] = 0;

	for(z = 0 ; z < E ; z++){
		for (i = 0; i < V; i++){
			for (j = 0; j < V; j++){
				int weight = (*graph).poids[i][j];
				if (dist[i] != INFINI && dist[i] + weight < dist[j] && (*graph).adj[i][j] == 1)
					dist[j] = dist[i] + weight;
			}
		}
	}

	for (i = 0; i < V; i++){
			for (j = 0; j < V; j++){
				int weight = (*graph).poids[i][j];
				if (dist[i] != INFINI && dist[i] + weight < dist[j] && (*graph).adj[i][j] == 1)
					printf("Il y a un cycle absorbant\n");
			}
		}
	for(i=0;i<V;i++)
		printf("%d : %d\n",i,dist[i]);
}
void Dijkstra(struct grapheMA g, int start){
	int marquer[g.n] ,dist[g.n] ,i = 0 ,j = 0, SommetActif = start,SommetActifMomentA = 0,SommetActifMomentB = 0, PoidsCourtChemin = 0, z = 0;
	for( i = 0 ; i < g.n ; i++){
		marquer[i] = TRUE;
		dist[i] = INFINI;
	}
	dist[SommetActif] = 0;
		for(j = 0; j < g.n; j++){
			if(marquer[j]){
				for(z = 0; z<g.n;z++){
					if(g.adj[z][j] == 1 && dist[z] + g.poids[z][j] < dist[j]){
						if(SommetActifMomentB == SommetActif){
							SommetActifMomentB =  j;
						}
						dist[j] = dist[z] + g.poids[z][j];
						if(dist[j] <=  dist[SommetActifMomentB]){
							SommetActifMomentB = j;
						}
					}
					if(g.adj[j][z] == 1 && dist[j] + g.poids[j][z] < dist[z]){
				
						dist[z] = dist[j] + g.poids[j][z];	
					}			
				}
			}
			if(SommetActif != SommetActifMomentB)
				PoidsCourtChemin = dist[SommetActifMomentB];
		marquer[SommetActif] = FALSE;
		SommetActif = SommetActifMomentB;
		}
	for(i=0;i<g.n;i++)
		printf("%d : %d\n PoidsPlusCourtChemin : %d \n",i,dist[i],PoidsCourtChemin);
}	
