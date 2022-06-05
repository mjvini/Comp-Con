#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

//Número de threads
#define NTHREADS 5

//Tamanho dos vetores
int dim = 10;



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
        vetorConc[i] = num_rand;    
    }

}

int maiorElemento(int dim){
    int maiorElemento = 0;
    for(int i = 0; i < dim; i++){
        if(maiorElemento < vetorConc[i]){
            maiorElemento = vetorConc[i];
        }
    }
    return maiorElemento;
}

//Bubble sort sequencial
void bubbleSortSequencial(int dim){
    int aux = 0;
    int swapped; //Break o for se não ocorrer swap.
    for (int i = 0; i < dim - 1; i++){
        swapped = 0; //False
        for(int j = 0; j < dim - i - 1; j++){
            //Swap
            if(vetorSeq[j] > vetorSeq[j+1]){
                aux = vetorSeq[j];
                vetorSeq[j] = vetorSeq[j+1];
                vetorSeq[j+1] = aux;
                swapped = 1; //True
            }
        }
        if(swapped == 0){
            break;
        }
    }
}

void *maxElemento(void *arg);

int* criaSublistas(int maiorNumero, int tamanhoLista, int nThreads) {
    int coeficiente = maiorNumero / nThreads;
    int sublistas[nThreads][tamanhoLista];

    for (int i = 0; i < nThreads; i++) {
        int coeficienteAtual = coeficiente * (i + 1);
        int coeficienteAnterior = coeficiente * i;

        int sublistaIndex = 0;

        for (int j = 0; j < tamanhoLista; j++) {
            if (vetorConc[j] <= coeficienteAtual && vetorConc[j] > coeficienteAnterior) {
                sublistas[i][sublistaIndex] = vetorConc[j];

                sublistaIndex++;
            };
        };
    };

    return sublistas;
};

void *tarefa(void *arg){
    // int maiorElemento = 0;
    // int id = * (int *) arg;
    // int tamBloco = dim/NTHREADS; //Tamanho do bloco de cada thread
    // int inicio = id * tamBloco; //Elemento inicial de cada thread
    // int fim; //Elemento final(nao processado) do bloco da thread
    // if(id == NTHREADS-1) fim = dim;
    // else fim = inicio + tamBloco; //Trata o resto se houver

        

    pthread_exit(NULL);
}

int main() {
    
    //Identificador da thread no sistema
    pthread_t tid[NTHREADS];
    
    
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

    // pthread_t tid[NTHREADS]; //Identificador da thread no sistema
    int ident[NTHREADS]; //Identificador local da thread
    
    //Cria as threads 
    for(int i = 0; i < NTHREADS; i++) {
       ident[i] = i;
       if (pthread_create(&tid[i], NULL,
        tarefa, (void *)&ident[i])) 
          printf("ERRO -- pthread_create\n");
    }

    //Preenche o vetor
    srand(time(NULL));
    inicializarVetor(dim);

    //Chama o Bubble Sort sequencial 
    bubbleSortSequencial(dim);

    //Printa o vetor
    for(int i = 0; i < dim; i++){
        printf("Vetor[%d] = %d\n", i, vetorSeq[i]);
    }

    //Libera o malloc
    free(vetorSeq);
    free(vetorConc);
}
