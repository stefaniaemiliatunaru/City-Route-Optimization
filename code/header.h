/* TUNARU Stefania-Emilia 311CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define OSUTA 100
#define OMIE 1000

/* implementare graf neorientat */
typedef struct celarc {
    int d;               /* destinatie arc */
    int c;               /* cost arc */
    struct celarc *urm;  /* adresa urmatorul adiacent */
} TCelArc, *AArc;

/* implementare lista de adiacenta a grafului */
typedef struct {
    int n;         /* numar noduri */
    AArc* x;       /* vector adrese liste arce */
} TGL;

/* implementare graf orientat */
typedef struct {
    int n;         /* numar noduri */
    int *ad;       /* vectorul de adancime */
    AArc* x;       /* vector adrese liste arce */
} TGL2;

TGL* AlocG(int nr);
void DistrG(TGL** ag);
TGL* CitGraf(char matrice[OMIE][OSUTA]);
int codificare(char matrice[OMIE][OSUTA], int* contor, char sir[OSUTA]);
void AfiGrafL(TGL* g);
void DFS(int nr_noduri, AArc vector[OSUTA], int nod, int vizitat[OSUTA]);
void CompConex(TGL *g);
int DistantaMinima(int* vizitate, int* distante, int nr_noduri);
int Prim(int nod, TGL* g);
TGL2* AlocG2(int nr);
void DistrG2(TGL2** ag);
TGL2* CitGraf2(char matrice[OMIE][OSUTA], int *greutate);
void AfiGrafL2(TGL2* g);
int ScorMinim(int* vizitat, float* scor, int nr_noduri);
void Dijkstra(int nod, TGL2* g, char matrice[OMIE][OSUTA], int greutate);
