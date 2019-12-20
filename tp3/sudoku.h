#ifndef SUDOKU_H
#define SUDOKU_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct adjacente {
    int cor, id;
} adjacente;

typedef struct celula {
    int cor, id, grau;
    adjacente* adjacentes;
} celula;

void alocaMatriz(celula***, int, int); 
void liberaMatriz(celula***, int, int);

void imprimeMatriz(celula**, int);

int verificaCorExiste(celula, int, int);
void zeraGrausCores(celula**, int, int);

void colocaAdjacentes(celula**, int, int, int, int);

int verificaCor(celula, int, int);
void coloreGrafo(celula**, int, int, int, int);

#endif