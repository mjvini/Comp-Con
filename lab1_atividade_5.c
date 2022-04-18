// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira 
// DRE: 118142223 
// Módulo 1 - Laboratório: 1 
// Atividade 5

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
#define TAM_VET 10000
int vet2[TAM_VET];

//Função que a thread irá executar
void * tarefa (void * arg){
    int vet = * (int *) arg; //Recebe o elemento do vetor vet original
    vet2[TAM_VET] = vet; 

    int thread_parte = 0;
    for(int i = thread_parte * (TAM_VET/2);
     i < (thread_parte + 1) * (TAM_VET/2); i++){
         vet2[i] *= vet2[i];
    }

    pthread_exit(NULL);
}

//Função principal
int main(void) {
    //Inicialização do vetor com 10_000 elementos
    //Para facilitar a vizualização do que está acontecendo
    //vou preencher o vetor com todos os elementos = 2.
    //Desse forma, espera-se que as threas elevem eles
    //ao quadrado para 4.
    int vet[TAM_VET];
    int cont = 0; //Verifica o total de elementos preenchidos
    for(int i =0; i<TAM_VET; i++){
        vet[i] = 2;
        cont++;
    }
    printf("Total de elementos preenchidos %d", cont);
    
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    //cria as threads 
    for(int i=1; i<=NTHREADS; i++) {
       //vet[i-1] = i;
       if (pthread_create(&tid[i-1], NULL, tarefa, (void *)&vet[i-1])) 
          printf("ERRO -- pthread_create\n");
    }
    //espera as threads terminarem 
    for(int i=0; i<NTHREADS; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }
}