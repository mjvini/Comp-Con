// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira e Filipe Trajlezer Junqueira dos Santos
// DRE: 118142223 e 117247224
// Módulo 1 - Laboratório: 1 
// Atividade 5

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
#define TAM_VET 10000
int vet[TAM_VET];
int vetor[NTHREADS];

//verifica todos os elementos do vetor. 
void verifica (int *vet){
//se cont = 0, todos os elementos estão ok! Ou seja, iguais a 4. 
    int cont = 0;    
    for(int i=0; i<TAM_VET;i++){
        if(vet[i] != 4){
              cont++;
        }         
    }
    if(cont==0){
        printf("Tudo certo!\n");
    }else{
        printf("Algo deu errado!\n");    
    }
}

//Função que a thread irá executar
void * tarefa (void * arg){
    int ident = * (int *) arg;
    vetor[ident-1] = ident;

    int thread_parte = ident - 1;
    //Executa a parte do vetor de acordo com a thread.
    for(int i = thread_parte * (TAM_VET/2);
     i < (thread_parte + 1) * (TAM_VET/2); i++){
         vet[i] *= vet[i];
    }
    printf("A thread %d está fazendo a parte %d do vetor \n",
    ident, thread_parte);
    
    pthread_exit(NULL);
}

//Função principal
int main(void) {
    //Inicialização do vetor com 10_000 elementos (de 0 à 9999)
    //Para facilitar a vizualização do que está acontecendo
    //vou preencher o vetor com todos os elementos = 2.
    //Desse forma, espera-se que as threas elevem eles
    //ao quadrado para 4.

    for(int i=0; i<TAM_VET; i++){
        vet[i] = 2; 
    }


    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int ident[NTHREADS]; //identificador local da thread
    //cria as threads 
    for(int i=0; i<NTHREADS; i++) {
       ident[i] = i+1;
       if (pthread_create(&tid[i], NULL,
        tarefa, (void *)&ident[i])) 
          printf("ERRO -- pthread_create\n");
    }

    //espera as threads terminarem 
    for(int i=0; i<NTHREADS; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }

    //funcao para testar se o vetor está preenchido corretamente com os elementos ao quadrado
    verifica(vet);
    

}
