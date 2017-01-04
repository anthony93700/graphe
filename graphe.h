#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NMAX 50

#define YMAX 50

#define FALSE 0

#define TRUE 1

#define INFINI 65536

static struct grapheMA{
	int n;
	int e;
	int adj[NMAX][YMAX];
	int poids[NMAX][YMAX];	
	int ordreprefixe[NMAX];
};

void lecture(struct grapheMA* pm,FILE* f);
void affichage(struct grapheMA m);
void makeFileDot(struct grapheMA m);
int DPS(struct grapheMA m,int sommet,int b);
void Kosaraju_sharir(struct grapheMA m);
void ordre_Topologique(struct grapheMA m);
int* tri_topologique(struct grapheMA m);
void BellmanFord(struct grapheMA* graph, int src);
void Dijkstra(struct grapheMA g, int start);
