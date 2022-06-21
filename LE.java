// Disciplina: Computacao Concorrente
// Aluno: Marcus Vinícius Torres de Oliveira
// DRE: 118142223
// Módulo 2 - Laboratório: 8


public class LE {
    private int leit, escr;

    // Construtor
    LE() {
        this.leit = 0; //leitores lendo (0 ou mais)
        this.escr = 0; //escritor escrevendo (0 ou 1)
    }

    // Entrada para leitores
    public synchronized void EntraLeitor (int id) {
        try {
            while (this.escr > 0) {
                //if (this.escr > 0) {
                System.out.println ("leitorBloqueado("+id+")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.leit++;  //registra que ha mais um leitor lendo
            System.out.println ("leitorLendo("+id+")");
        } catch (InterruptedException e) { }
    }

    // Saida para leitores
    public synchronized void SaiLeitor (int id) {
        this.leit--; //registra que um leitor saiu
        if (this.leit == 0)
            this.notify(); //libera escritor (caso exista escritor bloqueado)
        System.out.println ("leitorSaindo("+id+")");
    }

    // Entrada para escritores
    public synchronized void EntraEscritor (int id) {
        try {
            while ((this.leit > 0) || (this.escr > 0)) {
                //if ((this.leit > 0) || (this.escr > 0)) {
                System.out.println ("escritorBloqueado("+id+")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.escr++; //registra que ha um escritor escrevendo
            System.out.println ("escritorEscrevendo("+id+")");
        } catch (InterruptedException e) { }
    }

    // Saida para escritores
    public synchronized void SaiEscritor (int id) {
        this.escr--; //registra que o escritor saiu
        notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println ("escritorSaindo("+id+")");
    }
}

//--------------------------------------------------------
//Thread T1
// Escritor
class Escritor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads

    VariavelGlobal variavel; //Variável que será lida/escrita pelas threads
    // Construtor
    Escritor (int id, int delayTime, LE m, VariavelGlobal variavel) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
        this.variavel = variavel;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraEscritor(this.id);
                //System.out.println("Valor da variável(T1): " + this.variavel.getVariavelGlobal()+"\n"); //Todo apagar esse print!!!
                this.variavel.setVariavelGlobal(this.variavel.getVariavelGlobal() + 1);
                //System.out.println("Valor da variável depois(T1): " + this.variavel.getVariavelGlobal()+"\n"); //Todo apagar esse print!!!
                this.monitor.SaiEscritor(this.id);
                sleep(this.delay); //atraso bobo...
            }
        } catch (InterruptedException e) { return; }
    }
}

//Aplicacao de exemplo--------------------------------------------------------
//Thread T2
//Leitor
class Leitor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads

    VariavelGlobal variavel; //Variável que será lida/escrita pelas threads

    // Construtor
    Leitor (int id, int delayTime, LE m, VariavelGlobal variavel) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
        this.variavel = variavel;
    }

    static boolean ehPar(int n){
        if(n % 2 == 0){
            return true;
        }else{
            return false;
        }
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id);
                //System.out.println("Valor da variável (T2): "+this.variavel.getVariavelGlobal()+"\n"); //Todo apagar esse print!!!
                if(ehPar(this.variavel.getVariavelGlobal())){
                    System.out.println("A variável é par\n");
                }else{
                    System.out.println("A variável é ímpar\n");
                }
                this.monitor.SaiLeitor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) { return; }
    }
}
//Thread T3
//Leito/Escritor
class LeitorEscritor extends Thread{
    int id; //identificador da thread
    int delay; //atraso bobo...
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads

    VariavelGlobal variavel; //Variável que será lida/escrita pelas threads

    // Construtor
    LeitorEscritor (int id, int delayTime, LE m, VariavelGlobal variavel) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
        this.variavel = variavel;
    }

    public void run(){
        double j=777777777.7;
        int i;
        try{
            for(;;) { //Loop infinito
                this.monitor.EntraLeitor(this.id);
                System.out.println("Valor da variável antes (T3): "+this.variavel.getVariavelGlobal()+"\n");
                for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de leitura
                this.monitor.SaiLeitor(this.id);
                sleep(this.delay);
                this.monitor.EntraEscritor(this.id);
                this.variavel.setVariavelGlobal(this.id); //escreve o valor do identificador da thread na variável
                //System.out.println("valor da variável modificado (T3): "+this.variavel.getVariavelGlobal()+"\n"); //Todo apagar esse print!!!
                this.monitor.SaiEscritor(this.id);
                sleep(this.delay);
            }
        }
        catch (InterruptedException e){return;}
    }
}
//Class da variável global que vai ser modificada pelas threads
class VariavelGlobal {
    private int VariavelGlobal;

    VariavelGlobal(){
        this.VariavelGlobal = 0;
    }

    public int getVariavelGlobal() {
        return VariavelGlobal;
    }

    public void setVariavelGlobal(int NovoValorDaVariavelGlobal){
        this.VariavelGlobal = NovoValorDaVariavelGlobal;
    }
}

//--------------------------------------------------------
// Classe principal
class EhParOuImpar {
    static final int L = 4;
    static final int E = 3;

    static final int LE = 2;

    public static void main (String[] args) {
        int i;
        LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
        Leitor[] l = new Leitor[L];       // Threads leitores
        Escritor[] e = new Escritor[E];   // Threads escritores
        LeitorEscritor [] le = new LeitorEscritor[LE]; //Threads letores/escritores
        VariavelGlobal variavelGlobal = new VariavelGlobal(); //Variável que vai ser modificada pelas threads


        //inicia o log de saida
        System.out.println ("Início do Log: \n");
        //System.out.println ("le = verificaLE.LE()");

        for (i=0; i<L; i++) {
            l[i] = new Leitor(i+1, (i+1)*500, monitor, variavelGlobal);
            l[i].start();
        }
        for (i=0; i<E; i++) {
            e[i] = new Escritor(i+1, (i+1)*500, monitor, variavelGlobal);
            e[i].start();
        }

        //Se aparecer duas threads leitoras ou escritoras com o mesmo id, é porque essa
        //thread (LeitorEscritor) possui o mesmo id's que elas. Por isso, pode aparecer
        //duas threads que estão lendo ou escrevendo com o mesmo id. Poderia começar com
        //id+5, mas não quis confundir.
        for (i = 0; i<LE; i++){
            le[i] = new LeitorEscritor(i+1, (i+1)*500, monitor, variavelGlobal);
            le[i].start();
        }
    }
}

