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
pthread_cond_t cond_cond;

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

//Verifica se o vetorSeq é igual ao vetorConc
void verificaSeVetoresIguais(int *vetor1, int *vetor2, int dim){
    int iguais = 0; //True
    for(int i = 0; i < dim; i++){
        if(vetor1[i] != vetor2[i]){
            iguais++; //False
            break;
        }
    }
    if(iguais != 0){
        printf("Os vetores são diferentes!\n");
    }else{
        printf("Tudo certo! Os vetores são iguais!\n");    
    }
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

//Bubble sort concorrente
void bubbleSortConc(int *vetor, int tamanho){
    int aux = 0;
    int swapped; //Break o for se não ocorrer swap.
    for (int i = 0; i < tamanho - 1; i++){
        swapped = 0; //False
        for(int j = 0; j < tamanho - i - 1; j++){
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

void printaVetor(int *vetor, int dim){
    for(int i = 0; i < dim; i++){
        printf("Vetor[%d]: %d\n", i, vetor[i]);
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
            coeficienteInicial = -1; //Para não excluir o número 0
        }else{
            coeficienteInicial = temp; //problema com o + 1, vou tirar
        }
        
        if(i + 1 == nThreads){                              
            coeficienteFinal = maiorNumero;    
        }else{
            coeficienteFinal = maiorNumero/ (nThreads - i);
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
    int ident = * (int *) arg; //Identificador das threads
    
    int tamanho = sublistaIndex[ident]; //Pega o tamanho da sublista

    //Faz o bubble_sort da sublista se ela possuir mais de 1 elemento
    if(tamanho > 1){
        bubbleSortConc(sublistas[ident], tamanho); 
    }
    
    //bubbleSortConc(sublistas[ident], tamanho); //Faz o bubble_sort da sublista

    pthread_mutex_lock(&mutex);
    while(ident>x){ //Garante que as threads respeitem a ordem para o vetorConc ser preenchido corretamente
        pthread_cond_wait(&cond_cond, &mutex); 
    }

    if(tamanho != 0){ //Evita threads que não tem nenhum elemento em sua sublista
        int inicio = aux; //Elemento inicial de sublista
        int fim = inicio + tamanho; //Elemento final de cada sublista
        int j = 0;
        for(int i=inicio; i<fim; i++){
            int temp = sublistas[ident][j];
            vetorConc[i] = temp;
            j++;
            aux++;
        }
        
    }
    x++;
    
    pthread_cond_broadcast(&cond_cond);    
    pthread_mutex_unlock(&mutex);    

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
    //o nthreads é o número de linhas da matriz
    //dim é o número de colunas.
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
    
    //Preenche o vetor
    srand(time(NULL));
    inicializarVetor(dim);

    /*
    printaVetor(vetorConc, dim);
    puts("----------------------------------");
    */
    int maiorNum = maiorElemento(dim);
    printf("MaiorNUM: %d\n", maiorNum);
    puts("----------------------------------\n");    
    
    //--inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond_cond, NULL);

    //Cria as sublistas
    criaSublistas(maiorNum, dim, nthreads);

    //Prints de test
    /*
    for(int i=0; i<nthreads;i++){
        int tamanho = sublistaIndex[i];
        printf("tamanho: %d\n", tamanho);
        for(int j=0; j< tamanho;j++ ){
            printf("sublista[%d][%d]: %d\n", i, j,sublistas[i][j]);
        }
    }
    puts("---------------------------------------------\n");
    
    int tamanho = sublistaIndex[0];
    printf("tamanho: %d\n", tamanho);
    bubbleSortConc(sublistas[0], tamanho);

    for(int i=0; i<nthreads;i++){
        int tamanho = sublistaIndex[i];
        for(int j=0; j< tamanho;j++ ){
            printf("sublista[%d][%d]: %d\n", i, j,sublistas[i][j]);
        }
    }
    */
    
    //Pega o tempo da forma concorrente
    GET_TIME(ini);    

    //Criar as threads
    int ident[nthreads]; //Identificador local da thread
    for(int i=0; i<nthreads; i++){
        ident[i] = i;
        if(pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i])){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;        
        }
    }
    

    //espera as threads terminarem 
    for(int i=0; i<nthreads; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }

    GET_TIME(fim);
    tempoConc = fim-ini;
    
    //Chama o Bubble Sort
    //Pega o tempo da forma sequencial
    GET_TIME(ini);
    bubbleSortSequencial(vetorSeq);
    GET_TIME(fim);
    tempoSeq = fim-ini;
    printf("Tempo sequencial:  %lf\n", tempoSeq);

    printf("Tempo concorrente:  %lf\n", tempoConc);

    
    //printaVetor(vetorSeq, dim);
    puts("----------------------------------\n");
    //printaVetor(vetorConc, dim);
    //printf("ultimo elemento do vetorConc: %d", vetorConc[999]);
    
    //Verifica se os vetores são iguais
    //Se eles são iguais, "printa" que está tudo certo
    //Caso contrário, "printa" que eles são diferentes 
    verificaSeVetoresIguais(vetorSeq, vetorConc, dim);
    
    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_cond);

    //Libera o malloc
    free(vetorSeq);
    free(vetorConc);
    free(tid);
    free(sublistas);
    free(sublistaIndex);
}
