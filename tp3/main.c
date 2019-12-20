#include "sudoku.h"

int main(int argc, char* argv[]) {
    FILE *arquivo;
    int i, j, k, numVertices, numAdjacentes;
    const int dimensaoSudoku, colunaQuadrante, linhaQuadrante;
    celula **matriz;

    //checa se o nome do arquivo foi passado
    if(argc < 2) {
        printf("Nome do arquivo nao informado.\n");
        exit(1);
    }

    //abre arquivo e testa se existe
    arquivo = fopen(argv[1], "r");

    if((arquivo == NULL) || (arquivo == 0)) {
        printf("Arquivo não encontrado.\n");
        exit(1);
    }

    //le as 3 primeiras entradas
    fscanf(arquivo, "%d %d %d", &dimensaoSudoku, &colunaQuadrante, &linhaQuadrante);

    //calcula o número de vértices que são adjacentes
    numAdjacentes = 2 * (dimensaoSudoku - 1) + (linhaQuadrante * colunaQuadrante) - linhaQuadrante - colunaQuadrante + 1;

    //aloca a memoria necessaria para matriz e seus vetores
    alocaMatriz(&matriz, dimensaoSudoku, numAdjacentes);
    
    //le o sudoku da entrada
    for(i = 0; i < dimensaoSudoku; i++) {
        for(j = 0; j < dimensaoSudoku; j++) {
            fscanf(arquivo, "%d", &matriz[i][j].cor);
            matriz[i][j].id = i * dimensaoSudoku + j;
        }
    }

    //transformar o problema do sudoku para um grafo (vetor de adjacentes)
    colocaAdjacentes(matriz, dimensaoSudoku, linhaQuadrante, colunaQuadrante, numAdjacentes);
    //colore o grafo de modo que não haja conflitos de cores
    coloreGrafo(matriz, dimensaoSudoku, numAdjacentes, linhaQuadrante, colunaQuadrante);
    //libera a memoria da matriz e de seus vetores
    liberaMatriz(&matriz, dimensaoSudoku, numAdjacentes);
    fclose(arquivo);
    return 0;
}