#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

//Vetores
int *vetorSeq;
int *vetorConc;

//Variáveis para sincronização
pthread_mutex_t mutex;

//Inicializa o vetor com números aleatorios de 0 até 999
void inicializarVetor(int dim){
    for(int i=0; i<dim; i++){
        int num_rand = rand() % 1000;
        vetorSeq[i] = num_rand;    
    }

}

//Bubble sort sequencial
void bubble_sort_sequencial(int dim){
    int aux = 0;
    for (int i = 0; i < dim - 1; i++){
        for(int j = 0; j < dim - i - 1; j++){
            //Swap
            if(vetorSeq[j] > vetorSeq[j+1]){
            aux = vetorSeq[j];
            vetorSeq[j] = vetorSeq[j+1];
            vetorSeq[j+1] = aux;
            }
        }
    }
}

int *list (int * list){

}

int main(){
    //Tamanho dos vetores
    int dim =10;
    
    //Aloca os vetores
    vetorSeq = (int*) malloc(sizeof(int)*dim);
    if(vetorSeq == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }
    vetorConc = (int*) malloc(sizeof(int)*dim);
    if(vetorConc == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }

    //Preenche o vetor
    srand(time(NULL));
    inicializarVetor(dim);

    //Chama o Bubble Sort sequencial 
    bubble_sort_sequencial(dim);

    //Printa o vetor
    for(int i = 0; i < dim; i++){
        printf("Vetor[%d] = %d\n", i, vetorSeq[i]);
    }

    //Libera o malloc
    free(vetorSeq);
    free(vetorConc);
}