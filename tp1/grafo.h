#ifndef GRAFO_H
#define GRAFO_H
#include <stdio.h>
#include <stdlib.h>

typedef struct Vertice_str *Apontador;

typedef struct Vertice_str {
    int id, idade;
    Apontador Prox;
} Vertice;

typedef struct Grafo { 
    int numVertices;
    Apontador Primeiro, Ultimo;
} TipoGrafo;

void fazGrafo(TipoGrafo*, int);
int vazia(TipoGrafo*);

void inserePrimeira(TipoGrafo*, int, int);
void adicionaRelacao(TipoGrafo*, TipoGrafo*);
void removeVertice(TipoGrafo*, int);
void removeGrafo(TipoGrafo*);  

int DFS(TipoGrafo*);
void DFSRec(int, TipoGrafo*, int*, char*);

void swap(TipoGrafo*, int, int);

void meeting(TipoGrafo*);
void meetingDFSRec(int, TipoGrafo*, int*, int*, char*);

void fazGrafoTransposto(TipoGrafo*, TipoGrafo*);
void commander(TipoGrafo*, int);
void commanderDFSRec(TipoGrafo*, int, int, int*, char*);

#endif
