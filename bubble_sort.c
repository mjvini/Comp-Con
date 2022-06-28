#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

//Tamanho dos vetores
int dim;

//Vetores
int nthreads;
int *vetorSeq;
int *vetorConc;

int **sublistas; //matriz de sublistas
int *sublistaIndex;
int x=0;
int aux=0; 

//Variáveis para sincronização
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

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
void bubbleSortSequencial(int *vetor){
    int aux = 0;
    int swapped; //Break o for se não ocorrer swap.
    for (int i = 0; i < dim - 1; i++){
        swapped = 0; //False
        for(int j = 0; j < dim - i - 1; j++){
            //Swap
            if(vetor[j] > vetor[j+1]){
                aux = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = aux;
                swapped = 1; //True
            }
        }
        if(swapped == 0){
            break;
        }
    }
}

//void *maxElemento(void *arg);

void criaSublistas(int maiorNumero, int tamanhoLista, int nThreads) {
    int coeficiente = maiorNumero / nThreads;  //vetor [4,2,8,10,5,7,6,3,1,3,9] maiorelemento = 10
    //int sublistas[nThreads][tamanhoLista];  //supondo int coeficiente = 10/4 = 2 e  nThreads = 4
    int coeficienteFinal, coeficienteInicial, temp;                 //i = 0  / i = 1  / i = 2  / i = 3
    for (int i = 0; i < nThreads; i++) {                            // 2     / 4      / 6      / 10
                                                                    // 0     / 3      / 5      / 7
        if(i == 0){
            coeficienteInicial = 0;
        }else{
            coeficienteInicial = temp + 1;
        }
        
        if(i + 1 == nThreads){                              
            coeficienteFinal = maiorNumero;    
        }else{
            coeficienteFinal = (coeficiente) * (i + 1);
        }
        temp = coeficienteFinal; //Pega o coefFinal anterior
                                                       
        sublistaIndex[i] = 0;

        for (int j = 0; j < tamanhoLista; j++) {
            int sublistaIndex2 = sublistaIndex[i];
            if (vetorConc[j] <= coeficienteFinal && vetorConc[j] > coeficienteInicial) {
                sublistas[i][sublistaIndex2] = vetorConc[j];

                sublistaIndex[i]++;
            };
        };
    };

    //return sublistas;
};

void *tarefa(void *arg){
    int ident = * (int *) arg;
    pthread_mutex_lock(&mutex);
    if(ident==x){
        pthread_mutex_unlock(&mutex);
        int tamanho = sublistaIndex[ident]; //Pega o tamanho da sublista
        bubbleSortSequencial(sublistas[ident]); //Pensar em como pegar a sublista para usar no bubble_sort método
        int inicio = aux; //Elemento inicial de sublista
        int fim = tamanho; //Elemento final de cada sublista
    //    pthread_mutex_lock(&mutex);
    //if(ident==x){
        for(int i=inicio; i<fim; i++){
            int j = 0;
            vetorConc[i] = sublistas[ident][j];
            j++;
        }
        aux = tamanho;
        x++;
    }
        

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t *tid;
    double ini, fim, tempoConc, tempoSeq;

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }

    dim = atoi(argv[1]); //Salva o número da dimensão dos vetores
    nthreads = atoi(argv[2]); //Salva o número de threads
    //sublistaIndex[nthreads]; 
    
    
    //Aloca os vetores
    vetorSeq = (int*) malloc(sizeof(int) * dim);
    if(vetorSeq == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }
    
    vetorConc = (int*) malloc(sizeof(int) * dim);
    if(vetorConc == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }
    
    //Aloca as sublistas
    sublistas = (int**) malloc(sizeof(int*) * nthreads);
    for(int i=0; i<nthreads; i++){
        sublistas[i] = (int *) malloc (dim * sizeof(int));
    }
    if(sublistas == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }

    sublistaIndex = (int*) malloc(sizeof(int) * nthreads);
    if(sublistaIndex == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }

    //Aloca tid
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }
    
    //--inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);

    //Criar as threads
    int ident[nthreads]; //Identificador local da thread
    for(int i=0; i<nthreads; i++){
        if(pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i])){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;        
        }
    }

    //Preenche o vetor
    srand(time(NULL));
    inicializarVetor(dim);

    int maiorNum = maiorElemento(dim);

    //Cria as sublistas
    criaSublistas(maiorNum, dim, nthreads);

    //Printa o vetor
    for(int i = 0; i < dim; i++){
        printf("Vetor[%d] = %d\n", i, vetorConc[i]);
    }
    puts("--------------------------------------");

    //Chama o Bubble Sort sequencial 
    bubbleSortSequencial(vetorConc);


    for(int i = 0; i < dim; i++){
        printf("Vetor[%d] = %d\n", i, vetorConc[i]);
    }



    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&mutex);

    //Libera o malloc
    free(vetorSeq);
    free(vetorConc);
    free(tid);
    free(sublistas);
    free(sublistaIndex);
}
