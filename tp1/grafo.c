#include "grafo.h"

//inicializa a lista de adjacencia
void fazGrafo(TipoGrafo* Grafo, int numPessoas) {
    Grafo->Primeiro = (Apontador) malloc(sizeof(Vertice));
    Grafo->Ultimo = Grafo->Primeiro;
    Grafo->numVertices = numPessoas;
    Grafo->Primeiro->Prox = NULL;
}

//checa se a lista esta vazia, retornando 0 se não estiver
int vazia(TipoGrafo* Grafo) {
    return (Grafo->Primeiro == Grafo->Ultimo);
}

//adiciona na celula cabeça o id e a idade do vértice
void inserePrimeira(TipoGrafo* Grafo, int id, int idade) {
    Grafo->Primeiro->id = id;
    Grafo->Primeiro->idade = idade;
}

//cria uma celula na lista de quem comanda, colocando
//os dados do vértice comandado nessa nova célula
void adicionaRelacao(TipoGrafo* Comanda, TipoGrafo* Comandado) {
    Comanda->Ultimo->Prox = (Apontador) malloc(sizeof(Vertice));
    Comanda->Ultimo = Comanda->Ultimo->Prox;
    Comanda->Ultimo->id = Comandado->Primeiro->id;
    Comanda->Ultimo->idade = Comandado->Primeiro->idade;
    Comanda->Ultimo->Prox = NULL;
}

//remove um vértice de id B em uma lista Grafo[i]
void removeVertice(TipoGrafo* Grafo, int B) {
    Apontador temp, anterior;
    anterior = Grafo->Primeiro;
    temp = anterior->Prox;
    while(temp != NULL) {
        if(temp->id == B) {
            anterior->Prox = temp->Prox;
            if(anterior->Prox == NULL) {
                Grafo->Ultimo = anterior;
            }
            free(temp);
            return;
        }
        anterior = anterior->Prox;
        temp = temp->Prox;
    }
}

//apaga todas as celulas do Grafo
void removeGrafo(TipoGrafo* Grafo) {
    int id, numVertices, i;
    Apontador proximo, temp;
    numVertices = Grafo[1].numVertices;

    for(i = 1; i <= numVertices; i++) {
        temp = Grafo[i].Primeiro;
        while(temp != NULL) {
            proximo = temp->Prox;
            free(temp);
            temp = proximo;
        }
    }
}

//caminhamento recursivo do DFS, verificando quando há ciclos
//incrementando 1 na variável ciclo 
void DFSRec(int u, TipoGrafo* Grafo, int* ciclo, char* status) {
    int v;
    status[u] = 'C';    
    Apontador temp;
    temp = Grafo[u].Primeiro;
    if(vazia(&Grafo[u]) == 0) {
        while(temp->Prox != NULL) {
            v = temp->Prox->id;
            if(status[v] == 'C') {
                (*ciclo)++;
            }
            if(status[v] == 'B') {
                DFSRec(v, Grafo, ciclo, status);
            }
            temp = temp->Prox;
        }
    }
    status[u] = 'P';
}

//função que chama o DFSRec, aqui é inicializado todas variáveis
//e retorna o resultado de ciclo
int DFS(TipoGrafo* Grafo) {
    int i, ciclo, numVertices;
    ciclo = 0;
    numVertices = Grafo[1].numVertices;
    /*  B = branco (não visitado)
        C = cinza (visitando)
        P = preto (visitado)  */
    char *status = malloc((numVertices + 1) * sizeof(char));
    //preenche todo o vetor com 'B' (não visitado)
    for(i = 1; i <= numVertices; i++) {
        status[i] = 'B';
    }
    for(i = 1; i <= numVertices; i++) {
        if(status[i] == 'B') {
           DFSRec(i, Grafo, &ciclo, status);
        }
    }
    free(status);
    return ciclo;
}

//troca a relação A -> B para A <- B
//ou B -> A para B <- A, caso alguma exista
void swap(TipoGrafo* Grafo, int A, int B) {
    Apontador temp;
    int verificaRelacaoAB, verificaRelacaoBA, ciclo;
    verificaRelacaoAB = 0;
    verificaRelacaoBA = 0;

    //verifica se A comanda B
    temp = Grafo[A].Primeiro->Prox;
    while(temp != NULL) {
        if(temp->id == B) {
            verificaRelacaoAB = 1;
            break;
        }
        temp = temp->Prox;
    }

    //verifica se B comanda A se AB for 0 (false)
    if(verificaRelacaoAB == 0) {
        temp = Grafo[B].Primeiro->Prox;
        while(temp != NULL) {
            if(temp->id == A) {
                verificaRelacaoBA = 1;
                break;
            }
            temp = temp->Prox;
        }
    }
    
    //se nao existe relacao entre nenhum dos dois, retorna e imprime o resultado S N
    if(verificaRelacaoBA == 0 && verificaRelacaoAB == 0) {
        printf("S N\n");
        return;
    }
    
    //se existe a relacao A->B, remove e adiciona a troca, se houver
    //ciclos, a troca é desfeita, imprimindo o resultado S N
    if(verificaRelacaoAB == 1) {
        removeVertice(&Grafo[A], B);
        adicionaRelacao(&Grafo[B], &Grafo[A]);
        ciclo = DFS(Grafo); //verifica se há ciclos
        if(ciclo > 0) {
            removeVertice(&Grafo[B], A);
            adicionaRelacao(&Grafo[A], &Grafo[B]);
            printf("S N\n");
            return;
        }
    }
    //se nao, se existe a relacao B->A, remove e adiciona a troca, se houver
    //ciclos, a troca é desfeita, imprimindo o resultado S N
    else if(verificaRelacaoBA == 1) {
        removeVertice(&Grafo[B], A);
        adicionaRelacao(&Grafo[A], &Grafo[B]);
        ciclo = DFS(Grafo); //verifica se há ciclos
        if(ciclo > 0) {
            removeVertice(&Grafo[A], B);
            adicionaRelacao(&Grafo[B], &Grafo[A]);
            printf("S N\n");
            return;
        }
    }
    //caso a troca foi bem sucedida, imprimindo o resultado S T
    printf("S T\n");
}


//caminhamento recursivo DFS, colocando no vetor ordemFala
//os primeiros id's (menores tempos) de trás pra frente no vetor, para que
//fique ordenada pelo maior tempo de término para os menores tempos
void meetingDFSRec(int u, TipoGrafo* Grafo, int *index, int* ordemFala, char* status) {
    int v;
    Apontador temp;
    temp = Grafo[u].Primeiro;
    if(vazia(&Grafo[u]) == 0) {
        while(temp->Prox != NULL) {
            v = temp->Prox->id;
            if(status[v] == 'B') {
                meetingDFSRec(v, Grafo, index, ordemFala, status);
            }
            temp = temp->Prox;
        }
    }
    status[u] = 'P';
    //acrescenta o id do vertice u na posicao index
    ordemFala[(*index)] = u;
    //decrementa o index (começo a acrescentar no ultimo até o index 1)
    (*index)--;
}

//inicia as variaveis e chama a funcao recursiva meetingDFSRec
//no final, imprime a ordem de fala
void meeting(TipoGrafo* Grafo) {
    int i, index, numVertices;
    index = Grafo[1].numVertices;
    numVertices = Grafo[1].numVertices;
    /*  B = branco (não visitado)
        P = preto (visitado)  */
    char *status = malloc((numVertices + 1) * sizeof(char));
    int *ordemFala = malloc((numVertices + 1) * sizeof(int));
    //preenche todo o vetor com 'B' (não visitado)
    for(i = 1; i <= numVertices; i++) {
        status[i] = 'B';
    }
    for(i = 1; i <= numVertices; i++) {
        if(status[i] == 'B') {
           meetingDFSRec(i, Grafo, &index, ordemFala, status);
        }
    }
    printf("M ");
    for(i = 1; i <= numVertices; i++) {
        printf("%d ", ordemFala[i]);
    }
    printf("\n");
    free(status);
    free(ordemFala);
}

//cria o grafo transposto do grafo original passado
void fazGrafoTransposto(TipoGrafo* grafoOriginal, TipoGrafo* grafoTransposto) {
    Apontador temp;
    int i, numVertices;
    numVertices = grafoOriginal[1].numVertices;

    for(i = 1; i <= numVertices; i++) {
        fazGrafo(&grafoTransposto[i], numVertices);
        inserePrimeira(&grafoTransposto[i], grafoOriginal[i].Primeiro->id, grafoOriginal[i].Primeiro->idade);
    }

    for(i = 1; i <= grafoOriginal[1].numVertices; i++) {
        temp = grafoOriginal[i].Primeiro->Prox;
        while(temp != NULL) {
            adicionaRelacao(&grafoTransposto[temp->id], &grafoTransposto[i]);
            temp = temp->Prox;
        }
    }
}

void commanderDFSRec(TipoGrafo* grafoTransposto, int u, int membro, int* menorIdade, char* status) {
    int v;
    Apontador temp;
    /* pega o primeiro vertice de cada lista, comparando se a idade é menor 
       e se esse vertice não é o mesmo q procuramos, caso for sim para ambos,
       altera a menor idade pela nova (ira percorrer todos vertices recursivamente
       que é comandado pelo membro que se deseja) */

    temp = grafoTransposto[u].Primeiro;
    if(temp->idade < (*menorIdade) && (u != membro)) {
        (*menorIdade) = temp->idade;
    }
    if(vazia(&grafoTransposto[u]) == 0) { 
        while(temp->Prox != NULL) { 
            v = temp->Prox->id;
            if(status[v] == 'B') {
                commanderDFSRec(grafoTransposto, v, membro, menorIdade, status);
            }
            temp = temp->Prox;
        }
    }
    status[u] = 'P';
}

void commander(TipoGrafo* Grafo, int membro) {
    int i, menorIdade, numVertices;
    menorIdade = 101; //uma idade maior do que a maior permitida (100)
    numVertices = Grafo[1].numVertices;
    /* cria um grafo transposto do orignal, ou seja,
    se um vértice era comandado diretamente ou indiretamente por um conjunto de vértices,
    agora ele comanda diretamente ou indiretamente esse conjunto de vértices, então
    basta fazer um caminhamento DFS na lista de adjacencia do vértice membro (parametro) */
    TipoGrafo* grafoTransposto = malloc((numVertices + 1) * sizeof(TipoGrafo));
    fazGrafoTransposto(Grafo, grafoTransposto);

    /*  vetor de status de cada vertice (1 a numVertices):
        B = branco (não visitado)
        P = preto (visitado)   */
    char *status = malloc((numVertices + 1) * sizeof(char));

    //preenche todo o vetor com 'B' (não visitado)
    for(i = 1; i <= numVertices; i++) {
        status[i] = 'B';
    }
    //1ª  membro = vertice que se encontra (muda na recursao)
    //2ª membro = usado para verificar u != membro, que garante q nao ira colocar a idade do membro
    //passado junto com o C como parametro
    commanderDFSRec(grafoTransposto, membro, membro, &menorIdade, status);
    if(menorIdade == 101) { //se a idade ficou inaulterada, entao ninguem comanda o membro
        printf("C *\n");
    }
    else { //se nao, entao imprime a menor idade encontrada
        printf("C %d\n", menorIdade);
    }
    removeGrafo(grafoTransposto); //libera memoria de todas listas do vetor grafoTransposto
    free(grafoTransposto); //libera memoria do vetor grafoTransposto
    free(status); //libera memoria do vetor de status
}
