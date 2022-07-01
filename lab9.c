// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 3 - Laboratório: 9

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS  5

int x = 0;
sem_t sem1, sem2;

void * tarefa1(void *arg){

    sem_wait(&sem2);
    printf("Volte sempre! \n");
    pthread_exit(NULL);
}

void * tarefa2(void *arg){

    sem_wait(&sem1);
    printf("Fique a vontade. \n");
    x++;
    sem_post(&sem1);
    //Só libera a última thread (T1) quando todas as 4 já tiverem imprimido.
    if(x==3){
        sem_post(&sem2); //Cada uma precisa do post, pois a ordem de quem vai ser a 4º a printar é aleatória
    }
    pthread_exit(NULL);
}

void * tarefa3(void *arg){

    sem_wait(&sem1);
    printf("Sente-se por favor. \n");
    x++;
    sem_post(&sem1);
    //Só libera a última thread (T1) quando todas as 4 já tiverem imprimido.
    if(x==3){
        sem_post(&sem2); //Cada uma precisa do post, pois a ordem de quem vai ser a 4º a printar é aleatória
    }
    pthread_exit(NULL);
}

void * tarefa4(void *arg){

    sem_wait(&sem1);
    printf("Aceita um copo d'água?. \n");
    x++;
    sem_post(&sem1);
    //Só libera a última thread (T1) quando todas as 4 já tiverem imprimido.
    if(x==3){
        sem_post(&sem2); //Cada uma precisa do post, pois a ordem de quem vai ser a 4º a printar é aleatória
    }
    pthread_exit(NULL);
}

void * tarefa5(void *arg){

    //A tarefa 5 será a primeria a executar, por isso não precisa do wait.
    printf("Seja bem-vindo! \n");
    sem_post(&sem1);
    pthread_exit(NULL);
}

int main(){
    
    pthread_t tid[NTHREADS]; //identificador da thread no sistema


    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
     
    /* Cria as threads */
    pthread_create(&tid[0], NULL, tarefa1, NULL);
    pthread_create(&tid[1], NULL, tarefa2, NULL);
    pthread_create(&tid[2], NULL, tarefa3, NULL);
    pthread_create(&tid[3], NULL, tarefa4, NULL);
    pthread_create(&tid[4], NULL, tarefa5, NULL);

    /* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++) {
    pthread_join(tid[i], NULL);
    }    
    
}