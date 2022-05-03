// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 1 - Laboratório: 3 
// Atividade 1

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long int dim;  //dimensao do vetor de entrada
int nthreads;  //numero de threads
double *vetor; //vetor de entrada com dimensao dim 

//Inicializa o vetor com números random
void inicializa_vet(int dim){
    float randMax = 3500000;
    srand(time(NULL));
    
    for(int i=0; i<dim;i++){
        float a = 1.0;
        float num_random = ((float)rand() / randMax) * a; //gera um número random entre 0 e 999
        vetor[i] = num_random;
    }/*
    for(int i=0; i<dim; i++){
        printf("vet[%d] = %.2f\n", i, vetor[i]);
    }
    */
}

//Achar menor e maior elemento do vetor de forma sequencial
void acharMenorMaiorElemento(int dim){
    double menor = vetor[0], maior = vetor[0];
    for(int i=0; i<dim; i++){
        if (menor > vetor[i]) {
            menor = vetor[i];
        }

        if (maior < vetor[i]) {
            maior = vetor[i];        
        }
    }
    printf("menor = %.2f e maior = %.2f \n", menor, maior);
}

typedef struct {
    int id;  //identificador do elemento que a thread vai processar
    double menor, maior; //Maior e menor elemento que a thread achou na parte do vetor dela
} tArgs;

void * tarefa(void * arg) {
    tArgs *args = (tArgs*) arg;
    int ident = args->id;
    
    vetor[ident-1] = ident;

    int thread_parte = ident - 1;
    //Executa a parte do vetor de acordo com a thread.
    for(int i = thread_parte * (TAM_VET/2); i < (thread_parte + 1) * (TAM_VET/2); i++){
     if (args->menor > vetor[i]) {
            args->menor = vetor[i];
        }
        if (args->maior < vetor[i]) {
            args->maior = vetor[i];        
         }
    }
    printf("A thread %d está fazendo a parte %d do vetor \n",
    ident, thread_parte);

    
    pthread_exit(NULL); 
}

//fluxo principal
int main(int argc, char *argv[]) {
   double ini, fim; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   double *retorno; //valor de retorno das threads

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
       return 1; 
   }
   dim = atoll(argv[1]); //converte a str para long long int
   nthreads = atoi(argv[2]);
   //aloca o vetor de entrada
   vetor = (double*) malloc(sizeof(double)*dim);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //preenche o vetor de entrada com número random
   inicializa_vet(dim);
   
   //Achar menor e maior elemento de forma sequencial
   acharMenorMaiorElemento(dim);

   //soma concorrente dos elementos
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->menor;
      (args+i)->maior;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      
   }
   GET_TIME(fim);
   printf("Tempo concorrente:  %lf\n", fim-ini);


   //libera as areas de memoria alocadas
   free(vetor);
   free(tid);

   return 0;
}
