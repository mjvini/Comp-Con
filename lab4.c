// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 1 - Laboratório: 4 
// Atividade 1

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h> //botar -lm para rodar a biblioteca
#include "timer.h"

int nthreads, *vetorEntrada, i_global = 0;
float *vetorSaida, *vetorSaidaConc;
long long int dim; 
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

//checa se é primo
int ehPrimo (long long int n){
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2 ==0) return 0;
    for (int i=3; i<sqrt(n)+1; i+=2)
        if(n%i ==0) return 0;
    return 1; //Se retornar 1 é primo
}
//Inicializa o vetor com números aleatorios 
void inicializarVetor(int dim){
    for(int i=0; i<dim; i++){
        int num_rand = rand() / 1000;
        vetorEntrada[i] = num_rand;    
    }

}

//printa o vetor de saída
void printaVet(float vetor[], int vetor2[], int dim){
    for(int i=0; i<dim; i++){
        printf("vetSaida[%d] = %.2f\n", i, vetorSaida[i]);
    }
    printf("\n");
    for(int i=0; i<dim; i++){
        printf("vetEntrada[%d] = %.2d\n", i, vetorEntrada[i]);
    }
}

void processaPrimos(int vetorEntrada[], float vetorSaida[], int dim) {
    for(int i=0; i<dim; i++) {
        if (ehPrimo(vetorEntrada[i])) //É primo
            vetorSaida[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaida[i] = vetorEntrada[i];
    }
}

void comparaVet(float vetor[], float vetor2[], int dim){
    int cont=0;
    for(int i=0; i<dim; i++){
        if(vetorSaida[i] != vetorSaidaConc[i]){
            cont++;
            printf("Valor de saída diferente na posição %d\n", i);
            break;
        }
    }
    if(cont == 0){
        printf("Tudo ok!\n");
    }

}

void *tarefa (void * arg){
    int i_local;
    //int ident = * (int *) arg; //Identificador da thread
    //Bloqueia a próxima thread de mudar a variável global 
    pthread_mutex_lock(&mutex);
    i_local = i_global; i_global++;
    pthread_mutex_unlock(&mutex);
    while(i_local < dim){
        if(ehPrimo(vetorEntrada[i_local]))
            vetorSaidaConc[i_local] = sqrt(vetorEntrada[i_local]);
        else
            vetorSaidaConc[i_local] = vetorEntrada[i_local];
        pthread_mutex_lock(&mutex);
        i_local = i_global; i_global++;
        pthread_mutex_unlock(&mutex);
    }

    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
     pthread_t *tid;
     double ini, fim;

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }

    dim = atoll(argv[1]); //converte a str para long long int
    nthreads = atoi(argv[2]);

    //aloca os vetores de entrada e saída 
    vetorEntrada = (int*) malloc(sizeof(int)*dim);
    if(vetorEntrada == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }
    vetorSaida = (float*) malloc(sizeof(float)*dim);
    if(vetorSaida == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }
    vetorSaidaConc = (float*) malloc(sizeof(float)*dim);
    if(vetorSaidaConc == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }

    //Preenche o vetor
    srand(time(NULL));
    inicializarVetor(dim);

    //Pega o tempo da forma sequencial
    GET_TIME(ini);
    processaPrimos(vetorEntrada,vetorSaida,dim);
    GET_TIME(fim);
    //Printa o tempo sequencial
    printf("Tempo sequencial: %lf\n", fim - ini);
    //printaVet(vetorSaida, vetorEntrada ,dim);

    //Aloca tid
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }
    
    //--inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);

    //Pega o tempo da forma concorrente
    GET_TIME(ini);
    //Criar as threads
    //(void *) &ident[i])
    //int ident[nthreads];
    for(int i=0; i<nthreads; i++){
        //ident[i] = i;
        if(pthread_create(&tid[i], NULL, tarefa, NULL)){
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
    printf("Tempo Concorrente: %lf\n", fim - ini);
    pthread_mutex_destroy(&mutex);
    //Compara os valores dos vetores de saída.
    //Se todos os valores estiverem iguais, printa tudo ok!
    comparaVet(vetorSaida, vetorSaidaConc, dim);
    //libera as areas de memoria alocadas
    free(vetorEntrada);
    free(vetorSaidaConc);
    free(vetorSaida);
    free(tid); 
}
