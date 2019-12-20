#include "merge.h"

void mergesortRec(ilha *ilhas, ilha *aux, int e, int m, int d) {
    int i, j, k;

    for(i = 0; i <= m; i++) {
        aux[i] = ilhas[i];
    }
    
    for(j = m + 1; j <= d; j++) {
        aux[d - j + m + 1] = ilhas[j];
    }

    i = e;
    j = d;

    /* ordena o vetor de ilha em ordem decrescente em relacao 
       aos valores de custo beneficio */
    for(k = e; k <= d; k++) {
        if(aux[i].custoBeneficio > aux[j].custoBeneficio) {
            ilhas[k] = aux[i];
            i++;
        }
        else {
            ilhas[k] = aux[j];
            j--;
        }
    }
}

void mergesort(ilha *ilhas, ilha *aux, int e, int d) {
    int m = (d + e) / 2;
    if(e < d) {
        mergesort(ilhas, aux, e, m);
        mergesort(ilhas, aux, m + 1, d);
        mergesortRec(ilhas, aux, e, m , d);
    }
}
