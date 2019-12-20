#include <string.h>
#include "grafo.h"

int main(int argc, char *argv[]) {
    FILE *arquivo;
    int numPessoas, numRelacoes, numInstrucoes;
    int i, idade, relacao[2], membroInt[2];
    char instrucao, nomeArquivo[25];

    if(argc >= 2) { //testa se passou o nome do arquivo como parametro
        strcpy(nomeArquivo, argv[1]);
    }
    else { //se nao, retorna
        printf("Quantidade de argumentos invalido.\n");
        exit(1);
    }

    //abre o arquivo passado e checa
    arquivo = fopen(nomeArquivo, "r");
    if((arquivo == NULL) || (arquivo == 0)) {
        printf("Arquivo não encontrado ou retorno 0.\n");
        exit(1);
    }

    //le a primeira linha do arquivo
    fscanf(arquivo, "%d %d %d", &numPessoas, &numRelacoes, &numInstrucoes);

    //caso não tiver nenhum membro
    if(numPessoas == 0) {
        exit(1);
    }

    //criando um array do TipoGrafo de tamanho numPessoas + 1 (ignorando a posicao 0)
    //ou seja, um vetor de listas encadeadas
    TipoGrafo *Grafo = malloc((numPessoas + 1) * sizeof(TipoGrafo));

    //inicializa a "lista" de cada pessoa
    //coloca a idade e a posicao correspondente (id) para cada pessoa
    //na celula cabeça de cada lista
    for(i = 1; i <= numPessoas; i++) {
        fazGrafo(&Grafo[i], numPessoas);
        fscanf(arquivo, "%d", &idade);
        inserePrimeira(&Grafo[i], i, idade);
    }

    //adiciona cada relacao na lista de adjacencia da pessoa que comanda
    for(i = 1; i <= numRelacoes; i++) {
        fscanf(arquivo, "%d %d", &relacao[0], &relacao[1]);
        adicionaRelacao(&Grafo[relacao[0]], &Grafo[relacao[1]]);
    }

    //lendo as instrucoes
    //membroInt é uma variavel temporaria para
    //guardar o(s) valor(es) após a instrução
    //onde S precisa de 2, C de 1.
    for(i = 0; i < numInstrucoes; i++) {
        fscanf(arquivo, " %c", &instrucao);
        if(instrucao == 'S') {
            fscanf(arquivo, "%d %d", &membroInt[0], &membroInt[1]);
            swap(Grafo, membroInt[0], membroInt[1]);
        }
        else if(instrucao == 'C') {
            fscanf(arquivo, "%d", &membroInt[0]);
            commander(Grafo, membroInt[0]);        
        }
        else if(instrucao == 'M') {
            meeting(Grafo);
        }
        else {
            printf("Instrucao invalida.");
            exit(1);
        }
    }

    removeGrafo(Grafo); //libera memoria de todas listas do Grafo
    free(Grafo); //libera memoria do vetor
    fclose(arquivo); //fecha o arquivo
    
    return 0;
}