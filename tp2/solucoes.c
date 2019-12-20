#include "solucoes.h"

int max(int valor1, int valor2) {
    return valor1 > valor2 ? valor1 : valor2;
}

void solucaoGulosa(const int numIlhas, const int valorMax, ilha* ilhas) {
    int i;
    int valorRestante, pontuacaoFinal, numDias;

    valorRestante = valorMax;

    // vetor auxilar para realizar mergesort
    ilha *aux = malloc(numIlhas * sizeof(ilha));

    // ordena em relacao ao custoBeneficio
    mergesort(ilhas, aux, 0, numIlhas - 1);

    numDias = 0;
    pontuacaoFinal = 0;

    /* laço que irá partir da ilha com maior custoBeneficio
       retirando do valorRestante o custo da ilha[i], desde que
       o valorRestante seja maior ou igual ao custo desta ilha */
    for(i = 0; i < numIlhas; i++) {
        while(1) {
            if(valorRestante >= ilhas[i].custo) {
                valorRestante = valorRestante - ilhas[i].custo;
                numDias++;
                pontuacaoFinal = pontuacaoFinal + ilhas[i].pontos;
            }
            else {
                break;
            }
        }
        if(valorRestante == 0) {
            break;
        }
    }
    printf("%d %d\n", pontuacaoFinal, numDias);
    free(aux);
}

void solucaoDinamica(const int numIlhas, const int valorMax, ilha* ilhas) {
    int i, j;
    int numDias, controleColuna, comIlhaAtual, semIlhaAtual;

    // alocando memoria para matriz
    int **matriz = malloc((numIlhas + 1) * sizeof(int*));

    for(i = 0; i < numIlhas + 1; i++) {
        matriz[i] = malloc((valorMax + 1) * sizeof(int));
    }

    // preenchendo a primeira linha com 0's
    for(j = 0; j < valorMax + 1; j++) {
        matriz[0][j] = 0;
    }

    // percorre a matriz (note que é usado ilha[i-1] pois o vetor começa no indice 0)
    for(i = 1; i < numIlhas + 1; i++) {
        for(j = 0; j < valorMax + 1; j++) {
            semIlhaAtual = matriz[i - 1][j];

            /* se o custo da ilha for maior que a coluna, 
               adiciona a melhor solução sem a ilha[i-1] */
            if(ilhas[i-1].custo > j) {
                matriz[i][j] = semIlhaAtual;
            }
            /* senao, irá pegar o maior valor entre
               a solução sem a ilha[i-1] e a solução com a ilha[i-1] */
            else{ 
                comIlhaAtual = ilhas[i - 1].pontos + matriz[i - 1][j - ilhas[i - 1].custo];
                matriz[i][j] = max(semIlhaAtual, comIlhaAtual);
            }
        }
    }

    numDias = 0;
    controleColuna = valorMax;

    /* laço para pegar o número de dias (que significa 
       o número de ilhas visitas), o caminhamento é iniciado
       no ultimo valor da matriz, comparando sempre com o indice
       acima, caso for igual, vá para o de cima, senão, vá para linha
       de cima mas também vá para a coluna subtraída do peso de ilha 
       que se estava */
    for(i = numIlhas; i >= 1; i--) {
        if(matriz[i][controleColuna] != matriz[i - 1][controleColuna]) {
            controleColuna = controleColuna - ilhas[i-1].custo;
            numDias++;
        }
    }

    printf("%d %d\n", matriz[numIlhas][valorMax], numDias);

    for(i = 0; i < numIlhas + 1; i++) {
        free(matriz[i]);
    }
    
    free(matriz);
}
