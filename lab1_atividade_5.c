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
int vet[TAM_VET];
int vetor[NTHREADS];
int parte =0; //Determina a parte do vetor a ser executada

//verifica todos os elementos do vetor. 
void verifica (int *vet){
//se cont = 0, todos os elementos estão ok! Ou seja, iguais a 4. 
    int cont = 0;    
    for(int i=0; i<TAM_VET;i++){
        if(vet[i] != 4){
     //       printf("Erro: Vetor não foi preenchido corretamente!!");
    //        return 0;
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

    int thread_parte = parte++;
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
    //int vet[TAM_VET];
    //int cont = 0; //Verifica o total de elementos preenchidos
    for(int i=0; i<TAM_VET; i++){
        vet[i] = 2; 
        //cont++;
    }
    //printa um valor qualquer o vetor antes de ser elevado ao quadrado.
    //printf("valor qualquer do vetor antes. vet[4999] = %d\n", vet[4999]);

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

    verifica(vet);
    
    //printa um valor qualquer o vetor depois de ser elevado ao quadrado.
    //printf("valor qualquer do vetor depois. vet[3000] = %d\n", vet[3000]);

}
