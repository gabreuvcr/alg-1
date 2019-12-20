#include "sudoku.h"

void alocaMatriz(celula*** matriz, int dimensaoSudoku, int numAdjacentes) {
    int i, j;
    //aloca a memoria da matriz dimensaoSudoku x dimensaoSudoku
    (*matriz) = malloc(dimensaoSudoku * sizeof(celula*));
    for(i = 0; i < dimensaoSudoku; i++) {
        (*matriz)[i] = malloc(dimensaoSudoku * sizeof(celula));
    }

    //pra cada celula da matriz, aloca o vetor de adjacentes de tamanho numAdjacentes
    for(i = 0; i < dimensaoSudoku; i++) {
        for(j = 0; j < dimensaoSudoku; j++) {
            (*matriz)[i][j].adjacentes = malloc(numAdjacentes * sizeof(adjacente));
        }
    }
}

void liberaMatriz(celula*** matriz, int dimensaoSudoku, int numAdjacentes) {
    int i, j;
    //libera a memoria de todos os vetores de adjacencia
    for(i = 0; i < dimensaoSudoku; i++) {
        for(j = 0; j < dimensaoSudoku; j++) {
            free((*matriz)[i][j].adjacentes);
        }
    }
    //libera a memoria da matriz
    for(i = 0; i < dimensaoSudoku; i++) {
        free((*matriz)[i]);
    }
    free((*matriz));

}

void imprimeMatriz(celula** matriz, int dimensaoSudoku) {
    int i, j;
    //imprime a cor de cada célula da matriz
    for(i = 0; i < dimensaoSudoku; i++) {
        for(j = 0; j < dimensaoSudoku; j++) {
            printf("%d ", matriz[i][j].cor);
        }
        printf("\n");
    }
}

int verificaCorExiste(celula celula, int cor, int numAdjacentes) {
    /*essa funcao ira checar se a cor já existe no vetor
      de adjacentes da celula enviada como parametro
      se existir retorna 0, senão, retorna 1*/
    int k;
    for(k = 0; k < numAdjacentes; k++) {
        if(celula.adjacentes[k].cor == cor) {
            return 0;
        }
    }
    return 1;
}

void zeraGrausCores(celula** matriz, int dimensaoSudoku, int numAdjacentes) {
    /*essa funcao ira zerar todos os adjacentes (para que na checagem verificarCorExiste
      não possuia valores anteriores) e zerar os graus de cada celula para não acumular 
      valores dos graus após utilizar a funcao colocaAdjacentes  dentro da coloreGrafo quando usada 
      para atualizar as informações após se modificar um vértice */
    int i, j, k;
    for(i = 0; i < dimensaoSudoku; i++) {
        for(j = 0; j < dimensaoSudoku; j++) {
            matriz[i][j].grau = 0;
            for(k = 0; k < numAdjacentes; k++) {
                matriz[i][j].adjacentes[k].cor = 0;
            }
        }
    }
}

void colocaAdjacentes(celula** matriz, int dimensaoSudoku, int linhaQuadrante, int colunaQuadrante, int numAdjacentes) {
    int i, j, k, m, indiceAdj;
    //zera os graus e as cores para atualizar
    zeraGrausCores(matriz, dimensaoSudoku, numAdjacentes);
    for(k = 0; k < dimensaoSudoku; k++) {
        for(m = 0; m < dimensaoSudoku; m++) {
            //indice para controlar a adição do vértice no vetor
            indiceAdj = 0;
            for(i = 0; i < dimensaoSudoku; i++) {
                for(j = 0; j < dimensaoSudoku; j++) {
                    if(matriz[k][m].id != matriz[i][j].id) {
                        //teste se está na mesma linha || coluna || quadrante
                        if((matriz[i][j].id / dimensaoSudoku == matriz[k][m].id  / dimensaoSudoku) ||
                           (matriz[i][j].id  % dimensaoSudoku == matriz[k][m].id  % dimensaoSudoku) ||
                           ((k / linhaQuadrante == i / linhaQuadrante) && (m / colunaQuadrante == j / colunaQuadrante))) {
                            //se a cor não é 0 e a cor não está no vetor, então aumenta o grau do vértice 
                            if(matriz[i][j].cor > 0 && verificaCorExiste(matriz[k][m], matriz[i][j].cor, numAdjacentes)) {
                                matriz[k][m].grau++;
                            }
                            //coloca o id e a cor do vértice vizinho no vetor de adjacentes e incrementa indiceAdj 
                            matriz[k][m].adjacentes[indiceAdj].id = matriz[i][j].id;
                            matriz[k][m].adjacentes[indiceAdj].cor = matriz[i][j].cor;
                            indiceAdj++;
                        }   
                    }
                }
            }
        }
    }
}

int verificaCor(celula matriz, int dimensaoSudoku, int numAdjacentes) {
    int testeCor, check, k;
    /*essa funcao irá apenas checar qual cor está disponível para colorir,
      pega sempre a primeira que achar*/
    for(testeCor = 1; testeCor <= dimensaoSudoku; testeCor++) {
        check = 0;
        for(k = 0; k < numAdjacentes; k++) {
            if(matriz.adjacentes[k].cor == testeCor) {
                check = 1;
            }
        }
        // se testaCor não está no vetor de adjacentes, entao retorna ela
        if(check == 0) {
            return testeCor;
        }
    }
    return 0;
}

void coloreGrafo(celula** matriz, int dimensaoSudoku, int numAdjacentes, int linhaQuadrante, int colunaQuadrante) {
    /*essa função irá primeiramente andar na matriz achando o *primeiro* maior grau PARA CADA vértice (dimensao * dimensao)
      guardando sua posição i e j
      após achar esse maior grau, irá verificar qual cor está disponivel para colorir*/
    int i, j, k, cor, maiorGrau, maiorI, maiorJ;
    //caminha em todas células da matriz
    for(k = 0; k < dimensaoSudoku * dimensaoSudoku; k++) {
        maiorGrau = 0;
        /*caminha na matriz achando o *primeiro* maior grau, guardando sua posicao i, j e atualizando
          a variavel maiorGrau */
        for(i = 0; i < dimensaoSudoku; i++) {
            for(j = 0; j < dimensaoSudoku; j++) {
                if(matriz[i][j].grau > maiorGrau && matriz[i][j].cor == 0) {
                    maiorGrau = matriz[i][j].grau;
                    maiorI = i;
                    maiorJ = j;
                }
            }
        }
        /*apos achar a celula com maior grau, verifica qual cor está disponivel, note que
          em muitas vezes terá grau máximo, ou seja, só há uma possibilidade de cor,
          caso houver mais uma opção de cor, a solução já não será mais garantida, podendo
          retornar a cor 0 em algum momento, o que significa q não há solução*/
        cor = verificaCor(matriz[maiorI][maiorJ], dimensaoSudoku, numAdjacentes);
        //entao, se achar 0, não há mais solução
        if(cor == 0) {
            break;
        }
        //colore a celula com a cor encontrada
        matriz[maiorI][maiorJ].cor = cor;
        //atualiza todo o "grafo" para que todas listas de adjacentes tenha a alteração
        colocaAdjacentes(matriz, dimensaoSudoku, linhaQuadrante, colunaQuadrante, numAdjacentes);
    }
    //se a cor terminou em 0, então imprima "sem solução", caso contrario, imprima "solução"
    if(cor == 0) {
        printf("sem solucao\n");
    }
    else {
        printf("solucao\n");
    }
    //matriz do Sudoku final com ou sem solução
    imprimeMatriz(matriz, dimensaoSudoku);
}
