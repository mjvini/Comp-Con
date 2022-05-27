
// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 2 - Laboratório: 6 



#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include<unistd.h>

#define L 10 //numero de threads leitoras
#define E 1 //numero de threads escritoras

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo
int quer_escr = 0; //Contador de threads que querem escrever e estão bloqueadas

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;


//entrada leitura
void InicLeit (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] quer ler\n", id);
   //Bloqueia as threads de leitura caso tenha alguma thread de escrita escrevendo ou 
   //tentando escrever.
   /*
    L[1] quer ler
    L[2] quer ler
    Leitora 1 esta lendo
    E[1] quer escrever
    E[1] bloqueou       //Bloqueou, pois já tinha uma thread de leitura lendo
    Leitora 2 esta lendo //Leu primeiro, pois ela já tinha passado da condição (escr > 0 || quer_escr > 0 ) 
                        //antes da thread de escrita implementar a variável que_escr
    L[4] quer ler //Perceba que a thread de leitura quer ler, mas a thread de escrita quer escrever
    L[4] bloqueou //Por isso, ela foi bloqueada. Se não fosse o caso, ela leria primeiro, mas ela entrou na condição. 
    L[1] terminou de ler
    L[2] terminou de ler
    E[1] desbloqueou //Asssim que a thread de escrita foi desbloqueada, ela já ganhou a prioridade, mesmo tendo uma thread de leitura
                    //Querendo ler também.  
    Escritora 1 esta escrevendo
    E[1] terminou de escrever
    L[4] desbloqueou
   */
   while(escr > 0 || quer_escr > 0 ) {
     printf("L[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_leit, &mutex);
     printf("L[%d] desbloqueou\n", id);
   }
   leit++;
   pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] terminou de ler\n", id);
   leit--;
   if(leit==0) pthread_cond_signal(&cond_escr);
   pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] quer escrever\n", id);
   while((leit>0) || (escr>0)) {
     printf("E[%d] bloqueou\n", id);
     quer_escr++;
     pthread_cond_wait(&cond_escr, &mutex);
     printf("E[%d] desbloqueou\n", id);
     quer_escr--;
   }
   escr++;
   pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] terminou de escrever\n", id);
   escr--;
   pthread_cond_signal(&cond_escr);
   if(escr == 0 && quer_escr == 0){
     pthread_cond_broadcast(&cond_leit); //Libera todas as threads de leitura, só quando não tem mais nenhuma thread de escrita
                                        //escrevendou ou tentando escrever.
   }
   pthread_mutex_unlock(&mutex);
}

//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    InicLeit(*id);
    printf("Leitora %d esta lendo\n", *id);
    FimLeit(*id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

//thread leitora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    InicEscr(*id);
    printf("Escritora %d esta escrevendo\n", *id);
    FimEscr(*id);
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

  //inicializa as variaveis de sincronizacao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_leit, NULL);
  pthread_cond_init(&cond_escr, NULL);

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
