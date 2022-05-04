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
    //srand(time(NULL));
    
    for(int i=1; i<dim+1;i++){
        double num_random = (rand() / 1000000.7)*i; //gera um número random 
        vetor[i - 1] = num_random;
    }
    //printa os elementos do vetor
    /*
    for(int i=0; i<dim; i++){
        printf("vet[%d] = %.2f\n", i, vetor[i]);
    }
    */
}

//Achar menor e maior elemento do vetor de forma sequencial
void acharMenorMaiorElementoSeq(int dim){
    double menor = vetor[0], maior = vetor[0];
    for(int i=0; i<dim; i++){
        if (menor > vetor[i]) {
            menor = vetor[i];
        }

        if (maior < vetor[i]) {
            maior = vetor[i];        
        }
    }
    printf("menorSeq = %.2f e maiorSeq = %.2f \n", menor, maior);
}

typedef struct {
    int id;  //identificador do elemento que a thread vai processar
    double menor, maior; //Maior e menor elemento que a thread achou na parte do vetor dela
} tArgs;

void * tarefa(void * arg) {
    tArgs *args = (tArgs*) arg;
    int ident = args->id;

    int thread_parte = ident;
    //Executa a parte do vetor de acordo com a thread.
    for(int i = thread_parte * (dim/nthreads); i < (thread_parte + 1) * (dim/nthreads); i++){
        if (args->menor > vetor[i]) {
            args->menor = vetor[i];
            //printf("Entrou no if menor\n");
        }
        if (args->maior < vetor[i]) {
            args->maior = vetor[i];
            //printf("Entrou no if maior\n");        
         }
    }
    //printf("A thread %d está fazendo a parte %d do vetor \n", ident, thread_parte);

    
    pthread_exit(NULL); 
}

void retornaMenorMaiorConc(void * arg){
    tArgs *args = (tArgs*) arg;
    double menorConc = (args+0)->menor, maiorConc = (args+0)->maior;
    for(int i=0; i<nthreads; i++){
        if((args+i)->menor < menorConc){
            menorConc = (args+i)->menor;
            //printf("A thread %d entrou no menor\n", i);  
        }
        if((args+i)->maior > maiorConc){
            maiorConc = (args+i)->maior;
            //printf("A thread %d entrou no maior\n", i);
        }
    }
    printf("menorConc = %.2f e maiorConc = %.2f \n", menorConc, maiorConc);
}

//fluxo principal
int main(int argc, char *argv[]) {
   double ini, fim, tempoConc, tempoSeq; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensão
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
   srand(time(NULL));
   inicializa_vet(dim);
   
   //Pega o tempo da forma sequencial
   GET_TIME(ini);
   //Achar menor e maior elemento de forma sequencial
   acharMenorMaiorElementoSeq(dim);
   GET_TIME(fim);
   tempoSeq = fim-ini;
   printf("Tempo sequencial:  %lf\n", tempoSeq);



   //Achar o maior e o menor elemento de forma concorrente
   //Pegar o tempo da forma concorrente
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}

   //criar as threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->menor = 10000000000000000; //Cria as threads com um valor default 
      (args+i)->maior = 0;
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
   tempoConc = fim-ini;
   //Pega o maior e o menor elemento pelas threads de forma sequencial
   retornaMenorMaiorConc((void*) args);

   printf("Tempo concorrente:  %lf\n", tempoConc);
   printf("Ganho de tempo: %lf (TempoSeq/ TempoConc)\n", (tempoSeq/tempoConc));

   //libera as areas de memoria alocadas
   free(vetor);
   free(args);
   free(tid);

   return 0;
}
