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
struct grapheMA TableauTographe(struct grapheMA g,int * tab){
	struct grapheMA m;
	int i =0,y = 0;
	m.n = g.n;
	m.e = g.n;
	for(i = 0;i<g.n;i++){
		for(y=0;y<g.n;y++){
			g.adj[i][y] = 0;
			g.poids[i][y] = 0;
		}
	}
	for(i=0;i<g.n;i++){
		m.adj[i][i+1]= 1;
		if(m.poids[i][i+1]!=INFINI)
			m.poids[i][i+1]=tab[i];
	}
	return m;
}
char * ecriture(struct grapheMA g){
	FILE* fichier = NULL;
	int i = 0, y;
    	fichier = fopen("sauvegarde.txt", "w");
	fputc(g.n,fichier);
	fputc('\n',fichier);
	fputc(g.e,fichier);
	fputc('\n',fichier);
	for(i = 0;i<g.n;i++){
		for(y=0;y<g.n;y++){
			if(g.adj[i][y] == 1)
				fputc(i,fichier);	
				fputc(' ',fichier);
				fputc(y,fichier);
				fputc(' ',fichier);
				fputc(g.poids[i][y],fichier);
				fputc('\n',fichier);
		}
	}
	return "sauvegarde.txt";
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
int tritopo(struct grapheMA *g, int *tabOrdre){

	int i , sommetChoisi , j=0;
	int *sommetRestant=NULL;
	sommetRestant= malloc(g->n * sizeof(int));

	for(i=0; i<g->n; i++){
		sommetRestant[i]=1;
	}
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
		return FALSE;
	else
		return TRUE;

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
			return 1;
	}
	return 0;
}
int * Bellman(struct grapheMA *g, int sommetDepart, int *tabOrdre){
    int nbOperations = 0;
    int i, j, posSommetDepart;
	int *dist , *tabPre=NULL;
	dist=malloc(g->n * sizeof(int));
	tabPre=malloc(g->n * sizeof(int));
	for(i=0; i<g->n; i++){
        	nbOperations++;
		dist[i]=INFINI;
		nbOperations++;
		tabPre[i]=i;
		nbOperations++;
		if(tabOrdre[i]==sommetDepart){
		    nbOperations++;
		    posSommetDepart=i;
		    nbOperations++;
       		 }
	}
	dist[posSommetDepart]=0;
	tabPre[posSommetDepart]=posSommetDepart;
	nbOperations+=2;

	for(i=posSommetDepart; i<g->n; i++){
        nbOperations++;
		for(j=0; j<g->n;j++){
		    nbOperations++;
			if(g->adj[i][j] == 1 && (g->poids[i][j] + dist[i])<dist[j]){
			    nbOperations+=3;
				dist[j]=(g->poids[i][j] + dist[i]);
                nbOperations+=2;
				tabPre[j]=i;
				nbOperations++;

			}
		}
	}
	int k;
    for(k=0;k<g->n;k++)
        printf("le plus court chemin entre %d et %d est %d\n", sommetDepart+1, k+1, dist[k]);
    printf("l'execution du programme pour votre graphe a necessite %d operations\n", nbOperations);
	return dist;
}
int * BellmanFord(struct grapheMA* graph, int src){
	int V = (*graph).n;
	int E = (*graph).e;
	int i = 0,j = 0,z = 0,weight = 0;
	int dist[V];
	int nbOperations = 0;
	for (i = 0; i < V; i++){
		dist[i]   = INFINI;
	}
	dist[src] = 0;

	for(z = 0 ; z < E ; z++){
		for (i = 0; i < V; i++){
			for (j = 0; j < V; j++){
				weight = (*graph).poids[i][j];
				if (dist[i] != INFINI && dist[i] + weight < dist[j] && (*graph).adj[i][j] == 1){
					dist[j] = dist[i] + weight;
				nbOperations+=2;				
				}
					
			}
		}
	}

	for (i = 0; i < V; i++){
			for (j = 0; j < V; j++){
				weight = (*graph).poids[i][j];
				if (dist[i] != INFINI && dist[i] + weight < dist[j] && (*graph).adj[i][j] == 1){
					printf("Il y a un cycle absorbant\n");
					nbOperations+=2;
				}
			}
		}
	return dist;
}
int poidsPositif(struct grapheMA g){
	int i,y, boole = TRUE;
	for(i = 0;i<g.n;i++){
		for(y=0;y<g.n;y++){
			if(g.poids[i][y]<0){
				boole=FALSE;
			}
		}
	}
	return boole;
}
int * Dijkstra(struct grapheMA g, int start){
	int marquer[g.n] ,dist[g.n] ,i = 0 ,j = 0, SommetActif = start,SommetActifMomentA = 0,SommetActifMomentB = 0, PoidsCourtChemin = 0, z = 0,nbOperations = 0;
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
							nbOperations++;
						}
						dist[j] = dist[z] + g.poids[z][j];
						nbOperations++;
						if(dist[j] <=  dist[SommetActifMomentB]){
							SommetActifMomentB = j;
							nbOperations++;
						}
					}
					if(g.adj[j][z] == 1 && dist[j] + g.poids[j][z] < dist[z]){
				
						dist[z] = dist[j] + g.poids[j][z];
						nbOperations++;	
					}			
				}
			}
			if(SommetActif != SommetActifMomentB)
				PoidsCourtChemin = dist[SommetActifMomentB];
		marquer[SommetActif] = FALSE;
		SommetActif = SommetActifMomentB;
		}
	printf("l'execution du programme pour votre graphe a necessite %d operations\n", nbOperations);
	return dist;
}	
