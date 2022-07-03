#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

//Tamanho dos vetores
int dim;

//Número de threads
int nthreads;

//Vetores
int *vetorSeq;
int *vetorConc;

int **sublistas; //Matriz de sublistas
int *tamDeCadaSublista; //Vetor que armazena o tamanho de cada sublista
int x=0; //Variável de controle das threads
int aux=0; //Variável que auxilia o preenchimento dos elementos no vetorconc

//Variáveis para sincronização
pthread_mutex_t mutex;
pthread_cond_t cond_cond; 

//Inicializa o vetor com números aleatorios de 0 até 999
void inicializarVetor(int dim){
    for(int i=0; i<dim; i++){
        int num_rand = rand() % 1000;
        vetorSeq[i] = num_rand;    
        vetorConc[i] = num_rand;    
    }

}

//Acha o maior elemento do vetorConc
int maiorElemento(int dim){
    int maiorElemento = 0;
    for(int i = 0; i < dim; i++){
        if(maiorElemento < vetorConc[i]){
            maiorElemento = vetorConc[i];
        }
    }
    return maiorElemento;
}

//Verifica se os vetores são iguais
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
//Passa a sublista e o tamanho dela
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

//Cria as sublistas
void criaSublistas(int maiorNumero, int tamanhoLista, int nThreads) {  
    
    int coeficienteFinal, coeficienteInicial, temp;
    for (int i = 0; i < nThreads; i++) {
        //Determina a faixa dos valores para cada sublista
        //de acordo com o maior número e o número de threads.
        //Dessa forma cada thread só vai organizar valores menores ou iguais
        //o coeficienteFinal e maiores do que o coeficienteInicial.
        if(i == 0){
            coeficienteInicial = -1; //Para não excluir o número 0
        }else{
            coeficienteInicial = temp;
        }

        if(i + 1 == nThreads){                              
            coeficienteFinal = maiorNumero;    
        }else{
            coeficienteFinal = maiorNumero/ (nThreads - i);
        }
        temp = coeficienteFinal; //Pega o coefFinal anterior
                                                       
        tamDeCadaSublista[i] = 0;

        for (int j = 0; j < tamanhoLista; j++) {
            int aux = tamDeCadaSublista[i];
            if (vetorConc[j] <= coeficienteFinal && vetorConc[j] > coeficienteInicial) {
                sublistas[i][aux] = vetorConc[j];

                tamDeCadaSublista[i]++;
            };
        };
    };

};

void *tarefa(void *arg){
    int ident = * (int *) arg; //Identificador das threads
    
    int tamanho = tamDeCadaSublista[ident]; //Pega o tamanho da sublista

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
        
        //Preenche o vetor com as sublistas já organizadas
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
    double ini, fim, tempoConc, tempoSeq, tempoSublistas, tempoMaiorNum;

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }

    dim = atoi(argv[1]); 
    nthreads = atoi(argv[2]);
    
    
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
    
    //Aloca as sublistas.
    //O nthreads é o número de linhas da matriz.
    //Já dim é o número de colunas.
    sublistas = (int**) malloc(sizeof(int*) * nthreads);
    for(int i=0; i<nthreads; i++){
        sublistas[i] = (int *) malloc (dim * sizeof(int));
    }
    if(sublistas == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 1;
    }

    //Aloca o vetor que guarda o tamanho das sublistas
    tamDeCadaSublista = (int*) malloc(sizeof(int) * nthreads);
    if(tamDeCadaSublista == NULL) {
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

    //Pega o maior número
    GET_TIME(ini);
    int maiorNum = maiorElemento(dim);    
    GET_TIME(fim);
    tempoMaiorNum = fim - ini;

    //Inicia o mutex e a condição (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond_cond, NULL);

    GET_TIME(ini);
    //Cria as sublistas
    criaSublistas(maiorNum, dim, nthreads);
    GET_TIME(fim);
    tempoSublistas = fim - ini;

    //Pega o tempo da forma concorrente
    GET_TIME(ini);    

    //Cria as threads
    int ident[nthreads]; //Identificador das threads
    for(int i=0; i<nthreads; i++){
        ident[i] = i;
        if(pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i])){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;        
        }
    }
    
    //Espera as threads terminarem 
    for(int i=0; i<nthreads; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }

    GET_TIME(fim);
    tempoConc = fim-ini;
    
    //Chama o Bubble Sort sequencial
    //Pega o tempo da forma sequencial
    
    GET_TIME(ini);
    bubbleSortSequencial(vetorSeq);
    GET_TIME(fim);
    tempoSeq = fim-ini;
    
    //Prints de desempenho
    printf("Tempo sequencial:  %lf\n", tempoSeq);
    printf("Tempo concorrente:  %lf\n", tempoConc);
    printf("Tempo de criação das sublistas:  %lf\n", tempoSublistas);
    printf("Tempo de achar o maior elemento:  %lf\n", tempoMaiorNum);
    printf("Ganho de desempenho: %lf\n", (tempoSeq/tempoConc));
    puts("----------------------------------\n");

    //Verifica se os vetores são iguais.
    //Se eles são iguais "printa" que está tudo certo.
    //Caso contrário, "printa" que eles são diferentes. 
    verificaSeVetoresIguais(vetorSeq, vetorConc, dim);
    
    //Destrói o mutex e a condição
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_cond);

    //Libera as areas de memoria alocadas
    free(vetorSeq);
    free(vetorConc);
    free(tid);
    free(sublistas);
    free(tamDeCadaSublista);
}
