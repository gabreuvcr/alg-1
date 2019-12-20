#include "solucoes.h"

int main(int argc, char* argv[]) {
    FILE *arquivo;
    int i;
    char nomeArquivo[25];
    const int valorMax, numIlhas;

    if(argc >= 2) {
        strcpy(nomeArquivo, argv[1]);
    }
    else {
        printf("Nome do arquivo nao informado.\n");
        exit(1);
    }

    arquivo = fopen(nomeArquivo, "r");

    if((arquivo == NULL) || (arquivo == 0)) {
        printf("Arquivo não encontrado ou retorno 0.\n");
        exit(1);
    }

    fscanf(arquivo, "%d %d", &valorMax, &numIlhas);

    ilha *ilhas = malloc(numIlhas * sizeof(ilha));

    for(i = 0; i < numIlhas; i++) {
        fscanf(arquivo, "%d %d", &ilhas[i].custo, &ilhas[i].pontos);
    }

    for(i = 0; i < numIlhas; i++) {
        ilhas[i].custoBeneficio = (float)ilhas[i].pontos / (float)ilhas[i].custo;
    }
    
    solucaoGulosa(numIlhas, valorMax, ilhas);

    solucaoDinamica(numIlhas, valorMax, ilhas);

    free(ilhas);
    fclose(arquivo);
    
    return 0;
}
