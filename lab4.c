
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nthreads, *vetorEntrada, i_global;
double *vetorSaida, *vetorSaidaConc;
long long int dim; 


//checa se é primo
int ehprimo (long long int n){
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2 ==0) return 0;
    for (int i=3; i<sqrt(n)+1; i+=2)
        if(n%i ==0) return 0;
    return 1; //Se retornar 1 é primo
}
//Inicializa o vetor com números aleatorios 
void inicializarVetor(int dim){
    for(int i=0; i<dim; i++){
        int num_rand = rand() / 1000;
        vetor[i] = num_rand;    
    }
}

void processaPrimos(int vetorEntrada[], float vetorSaida[], int dim) {
    for(int i=0; i<dim, i++) {
        if (ehPrimo(vetorEntrada[i])
            vetorSaida[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaida[i] = vetorEntrada[i];
    }
}

void *tarefa (void * arg){
    int ident = * (int *) arg;
    vetor[ident] = ident; //Identificador da thread

    

}

int main(int argc, char *argv[]) {
     pthread_t *tid;
     tArgs *args;
     double ini, fim;

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }

    dim = atoll(argv[1]); //converte a str para long long int
    nthreads = atoi(argv[2]);

    //aloca o vetor de entrada
    vetor = (int*) malloc(sizeof(int)*dim);
    if(vetor == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }

    //Preenche o vetor
    srand(time(NULL));
    inicializarVetor(dim);
 
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}

    //Criar as threads
    for(int =0; i<nthreads; i++){
        ident[i] = i;
        if(pthread_create(&tid[i], NULL, tarefa, (void *) &ident[i])){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;        
        }
    }    

 
}
