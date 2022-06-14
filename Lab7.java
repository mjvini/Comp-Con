// Disciplina: Computacao Concorrente
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 2 - Laboratório: 7
// Atividade 1

import java.util.Random;

//Class da estrutura de dados (recurso) compartilhado entre as threads
public class EncontrarPar {

    private int tamanhoVetor;
    private int contPar = 0; //Contador de números pares
    private int nThreads; //Número de threads
    private int[] vetor; //Vetor



    //Construtor. Passa o contPar, o número de threads e o vetor
    public EncontrarPar(int tamanhoVetor,int nThreads, int[] vetor){
        this.tamanhoVetor = tamanhoVetor;
        this.nThreads = nThreads;
        this.vetor = vetor;
    }

    //Conta os pares
    public synchronized void setContPar(int id){
        int inicio, fim;
        int tamBloco = this.tamanhoVetor/this.nThreads;
        inicio = id*tamBloco;
        if(id == nThreads - 1) fim = this.tamanhoVetor;
        else fim = inicio + tamBloco;

        for(int i=inicio; i<fim; i++){
            //Checa se o número do vetor é par
            if(this.vetor[i] % 2 == 0){
                //System.out.println("\n Número: "+ this.vetor[i]+" é par"); //Todo tirar esse print!!
                contPar++;
            }
        }

    }

    //Retornar o contPar
    public synchronized int getContPar(){return this.contPar;}
}
//Class que estende Thread e implementa a tareda de cada thread do programa
class T extends Thread{

    //Identificador da thread
    private int id;
    //Objeto compartilhado com outras threads
    private EncontrarPar contPar;


    //Construtor
    public T(int tid, EncontrarPar contPar){
        this.id = tid;
        this.contPar = contPar;
    }


    //Método main da thread
    public void run(){
        int id = this.id;
        System.out.println("Thread " + id + " iniciou");
        //Passa o id da thread para a função setContaPar
        this.contPar.setContPar(id);
    }
}
class TContaPar {

    //Número de threads
    static final int N = 4;
    static final int TAMANHO_VETOR = 100;

    public static void main(String[] args) {

        //Cria um vetor com números aleatórios de 0 até o TAMANHO_VETOR
        Random rand = new Random();
        int[] vetorNumeroAleatorio = new int[TAMANHO_VETOR];
        for(int i=0; i<TAMANHO_VETOR; i++){
            vetorNumeroAleatorio[i] = rand.nextInt(TAMANHO_VETOR);
            //System.out.println("Vetor["+i+"] = " + vetorNumeroAleatorio[i]); //Todo tirar esse print!!
        }

        //Reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N];

        //Cria uma instância do recurso compartilhado entre as threads
        EncontrarPar contPar =
                new EncontrarPar(TAMANHO_VETOR, N, vetorNumeroAleatorio);

        //Cria as threads da aplicação
        for (int i = 0; i < threads.length; i++) {
            threads[i] = new T(i, contPar);
        }

        //Inicia as threads
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        //Espera pelo termino de todas as threads
        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }

        System.out.println("Total de números pares no vetor é: "+
                contPar.getContPar());
    }
}
