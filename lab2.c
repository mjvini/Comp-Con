// Disciplina: Computacao Concorrente 
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 1 - Laboratório: 2 
// Atividade 1

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat1; //1° matriz de entrada
//float *mat2; //2° matriz de entrada **Obs.: Alocação de mem desnecessária
float *mat_saida_seq; //matriz de saída sequencial
float *mat_saida_conc; //matriz de saída não sequencial
int nthreads; //número de threads

typedef struct{
    int id; //identificador do elemento que a thread vai processar
    int dim; //dimensão das estruturas de entrada
} tArgs;

//função que as threas vão executar
void * tarefa(void *arg){
    tArgs *args = (tArgs*) arg;
    printf("Thread %d\n", args->id); //visualiza a thread sendo criada
    //processa a linha de acordo com o seu identificador.
    //"Salta" o n° de linhas de acordo com o n° de threads 
    for(int i=args->id; i<args->dim; i+=nthreads){ 
        for(int j=0; j<args->dim; j++){
            for(int k=0; k<args->dim; k++){
                mat_saida_conc[i*(args->dim)+j] += 
                    mat1[i*(args->dim)+k] * mat1[k*(args->dim)+j];
            }
        }
    }
    pthread_exit(NULL);
}
//função que multiplica as matrizes de forma sequencial
void matrizSeq(int dim){
    for(int i=0; i<dim;i++){
        for(int j=0; j<dim; j++){
            for(int k=0; k<dim; k++){
                mat_saida_seq[i*dim+j] += mat1[i*dim+k] * mat1[k*dim+j];
            }
        }
    }
    /*
    printf("matriz saída seq: \n");
    for(int i=0; i<dim;i++){
        for(int j=0; j<dim; j++){
            printf("%.1f ", mat_saida_seq[i*dim+j]);
        }
        puts("");
    } 
    */       

}
//Inicializa as matrizes de entrada e saída
void inicializarMat(int dim){
    for(int i=0; i<dim;i++){
        for(int j=0; j<dim; j++){
            mat1[i*dim+j] = 1;
            //mat2[i*dim+j] = 2;
            mat_saida_seq[i*dim+j] = 0;
            mat_saida_conc[i*dim+j] = 0;
        }
    }
}
//Verifica se o resultado das matrizes 
//de saída(Concorrente e sequencial) é igual
void verificaResMatSaida(int dim){
    int boolean = 0;
    for(int i=0; i<dim;i++){
        for(int j=0; j<dim; j++){
            if(mat_saida_conc[i*dim + j] != mat_saida_seq[i*dim+j]){
                printf("ERRO no resultado das matrizes!\n");
                boolean = 1;
                break;
            }
        }
    }
    if(boolean == 0){
        printf("O resulta das matrizes está ok\n");
    }
}

//fluxo principal
int main(int argc, char* argv[]){

    int dim;//dimensão das matrizes de entrada
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensão

    double inicio, fim, tempoMatSeq, tempoMatConc;

    //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]); //argv[0] trás o nome do programa
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim; //Evita threads desnecessárias

   //alocacao de memoria para as estruturas de dados
   mat1 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   //mat2 = (float *) malloc(sizeof(float) * dim * dim);
   //if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   mat_saida_seq = (float *) malloc(sizeof(float) * dim * dim);
   if (mat_saida_seq == NULL) {printf("ERRO--malloc\n"); return 2;}
   mat_saida_conc = (float *) malloc(sizeof(float) * dim * dim);
   if (mat_saida_conc == NULL) {printf("ERRO--malloc\n"); return 2;}


    //inicialização das matrizes
    inicializarMat(dim);

    //Pega o tempo da função sequencial de mult das matrizes
    //letra a)
    GET_TIME(inicio);
    //multiplicação das matrizes de forma sequencial
    matrizSeq(dim);
    GET_TIME(fim);
    tempoMatSeq = fim - inicio;

    //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}

    //Pega o tempo da parte concorrente do processamento
    //letra b)
    GET_TIME(inicio);
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim);
   tempoMatConc = fim - inicio;
    //printa a matriz concorrente de saída
   /* 
   printf("matriz saída conc: \n");
    for(int i=0; i<dim;i++){
        for(int j=0; j<dim; j++){
            printf("%.1f ", mat_saida_conc[i*dim+j]);
        }
        puts("");
    }
    */
   //Verifica se as matrizes de saída são iguais
    verificaResMatSaida(dim);    

   //liberação da memória
   free(mat1);
   //free(mat2);
   free(mat_saida_seq);
   free(mat_saida_conc);
   free(args);
   free(tid);

    //prints da avaliação de desempenho da solução
   printf("Tempo da função sequencial:%lf\n", tempoMatSeq);
   printf("Tempo da função concorrente:%lf\n", tempoMatConc);
   printf("Avaliação de tempo (func seq/func conc):%lf\n", 
   (tempoMatSeq/tempoMatConc));

    return 0;
}
