// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 3 - Laboratório: 10

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define L 3 //numero de threads leitoras
#define E 3 //numero de threads escritoras

/*
Ativida 1:
a) Os valores iniciais dos semáforos(em_e, em_l, escr, leit)
devem ser 1.

b) A primeira thread (leitora ou escritora) pegará o sem_wait(&escr) e isso vai fazer com que as outras threads do tipo oposto não possam executar.
Porém, as outras threads do seu tipo vão poder executar, pois elas não ficarão presas no sem_wait(&escr)
//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("Leitora %d quer ler\n", *id);  
    sem_wait(&em_l); l++;
      
    if(l == 1) {
      sem_wait(&escr); 
    }
    sem_post(&em_l);
    printf("Leitora %d esta lendo\n", *id);  
    sem_wait(&em_l); l--; 
    printf("Leitora %d terminou de ler\n", *id);
    if(l == 0) {
        sem_post(&escr); 
    }
    sem_post(&em_l); 
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}



//thread leitora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("Escritora %d quer escrever\n", *id);
    sem_wait(&em_e); e++; 
    if(e == 1) {
      sem_wait(&escr);  
    }
    sem_post(&em_e);
    printf("Escritora %d esta escrevendo\n", *id);
    sem_wait(&em_e); 
    e--;
    printf("Escritora %d terminou de escrever\n", *id);
    if(e == 0){
      sem_post(&escr); 
    }
    sem_post(&em_e);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

*/

sem_t em_e, em_l, escr, leit; //semaforos
int e=0, l=0; //globais

//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("Leitora %d quer ler\n", *id);
    sem_wait(&leit);  
    sem_wait(&em_l); l++;  
    if(l == 1) {
      sem_wait(&escr); 
    }
    sem_post(&em_l);
    printf("Leitora %d esta lendo\n", *id);  
    sem_post(&leit);  
    sem_wait(&em_l); l--; 
    printf("Leitora %d terminou de ler\n", *id);
    if(l == 0) {
        sem_post(&escr); 
    }
    sem_post(&em_l); 
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}



//thread leitora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("Escritora %d quer escrever\n", *id);
    sem_wait(&em_e); e++; 
    if(e == 1) {  
      sem_wait(&leit); 
    }
    sem_post(&em_e);
    sem_wait(&escr);
    printf("Escritora %d esta escrevendo\n", *id);
    sem_post(&escr);
    sem_wait(&em_e); 
    e--;
    printf("Escritora %d terminou de escrever\n", *id);
    if(e == 0){ 
      sem_post(&leit);
    }
    sem_post(&em_e);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  sem_init(&escr, 0, 1);
  sem_init(&leit, 0, 1);
  sem_init(&em_e, 0, 1);
  sem_init(&em_l, 0, 1);

  //cria as threads leitoras
  for(int i=0; i<L; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
  } 
  
  //cria as threads escritoras
  for(int i=0; i<E; i++) {
    id[i+L] = i+1;
    if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
  } 

  pthread_exit(NULL);
  return 0;
}
