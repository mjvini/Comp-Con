// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 2 - Laboratório: 5 
// Atividade 4


#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  5


//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_cond;
int x = 0;

const char* escolheFrase(){
    srand(time(NULL));
    int randomNum;
    const char* y[] = {"Fique a vontade.", "Sente-se por favor.", "Aceita um copo d'água?."};
    randomNum = (rand()%3);
    return y[randomNum];
}

void * tarefa1(void *arg){
    
    pthread_mutex_lock(&mutex);
    if(x==0){
        printf("Seja bem-vindo!\n");
        x++;
    }
    pthread_cond_broadcast(&cond_cond); //Usa broadcast, pois pode ocorrer de mais de uma ficar em wait antes do signal.
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL);
}

void * tarefa2(void *arg){
    
    
    pthread_mutex_lock(&mutex);
    //if(x<4){
    while(x<4){ //O While garante que a função vai esperar até ela ser a última a printar.
        pthread_cond_wait(&cond_cond, &mutex);    
    }
    printf("Volte sempre!\n");
    //pthread_cond_signal(&cond_cond);
        
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
    
}

void * tarefa3(void *arg){
    
    int y = 0;
    pthread_mutex_lock(&mutex);
    if(x<1){
        pthread_cond_wait(&cond_cond, &mutex);
    }
    if(y == 0){ //Faz com que a frase só seja printada uma vez
        printf("Fique a vontade.\n");
        y++;
        x++;
    }
    //Precisa do signal, pois uma thread pode entrar no wait dps do broadcast da tarefa 1, ou para liberar a tarefa 2.
    pthread_cond_signal(&cond_cond); 
    pthread_mutex_unlock(&mutex);    
    pthread_exit(NULL);
}

void * tarefa4(void *arg){
    
    int y=0;
    pthread_mutex_lock(&mutex);
    if(x<1){
        pthread_cond_wait(&cond_cond, &mutex);
    }
    if(y==0){ //Faz com que a frase só seja printada uma vez
        printf("Sente-se por favor.\n");
        y++;
        x++;
    }
    //Precisa do signal, pois uma thread pode entrar no wait dps do broadcast da tarefa 1, ou para liberar a tarefa 2.
    pthread_cond_signal(&cond_cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void * tarefa5(void *arg){
    
    int y=0;
    pthread_mutex_lock(&mutex);
    if(x<1){
        pthread_cond_wait(&cond_cond, &mutex);
    }
    if(y==0){ //Faz com que a frase só seja printada uma vez
        printf("Aceita um copo d'água?.\n");
        y++;
        x++;
    }
    //Precisa do signal, pois uma thread pode entrar no wait dps do broadcast da tarefa 1, ou para liberar a tarefa 2.
    pthread_cond_signal(&cond_cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(){
    
    pthread_t tid[NTHREADS]; //identificador da thread no sistema

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond_cond, NULL);
     
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

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_cond);
    
}
