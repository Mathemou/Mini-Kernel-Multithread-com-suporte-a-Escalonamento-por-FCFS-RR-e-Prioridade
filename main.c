#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "pcb.h"
#include "log.h"
#include "fila.h"
#include "processo.h"

int main(int argc, char *argv[]) {
    if (argc < 2){
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    };
    FILE *arquivo_entrada = fopen(argv[1], "r");

    if(arquivo_entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    char linha[100];

    int qnt_cpus;
    int quantum;
    char politica[16]; // POde ser FCFS, RR ou PRIORIDADE
    int qnt_processos;
    
    // Pra ler as primeiras 5 linhas do arquivo de entrada
    for(int i =0; i<=4;i++){
        fgets(linha, sizeof(linha), arquivo_entrada); // Lê uma linha do arquivo de entrada e armazena na variável linha
        if(i == 0){ // A primeira linha, que contém a quantidade de CPUs
            char * quantidade_cpus = linha + 5;
            sscanf(quantidade_cpus, "%d", &qnt_cpus);
        } else if (i ==1){ // A segunda linha, que contém o quantum
            char * quantidade_quantum = linha + 8;
            sscanf(quantidade_quantum, "%d", &quantum);
        } else if (i == 2){ // A terceira linha, que contém a política de escalonamento
            char * tipo_politica = linha +  9;
            sscanf(tipo_politica, "%s", politica);
        } else if (i == 3){ // A quarta linha, que contém o tipo de fila (???)
            // Acho que pode ser ignorado, todas as filas são globais
        } else if (i == 4){ // A quinta linha, que contém a quantidade de processos
            char * quantidade_processos = linha + 10;
            sscanf(quantidade_processos, "%d", &qnt_processos);
        }
    }

    //printf("Dados da entrada:\nQuantidade de CPUs: %d\nQuantum: %d\nPolítica de escalonamento: %s\nQuantidade de processos: %d\n======================\t======================\n\n", qnt_cpus, quantum, politica, qnt_processos);

    PCB *processos[qnt_processos]; // Cria um array de ponteiros para PCB, com tamanho igual à quantidade de processos

    // As linhas seguintes descrevem os BCP/PCB (São as mesma coisa!!!)
    for(int i = 0; i < qnt_processos; i++) {
        fgets(linha, sizeof(linha), arquivo_entrada);
        int pid;
        int chegada;
        int duracao;
        int prioridade;
        int num_threads;
        sscanf(linha, "%d %d %d %d %d", &pid, &chegada, &duracao, &prioridade, &num_threads);

        PCB *novo_pcb = (PCB *)malloc(sizeof(PCB));
        criar_pcb(novo_pcb, pid, chegada, duracao, prioridade, num_threads);
        processos[i] = novo_pcb; // Armazena o ponteiro para o PCB recém-criado no array de processos
    }


    /*DEBUG DOS PROCESSOS LIDOS*/
    /*for(int i = 0; i < qnt_processos; i++) {
        PCB *pcb_atual = processos[i];
        printf("Processo %d:\nPID: %d\nChegada: %d\nDuração: %d\nPrioridade: %d\nNúmero de threads: %d\n======================\t======================\n\n", i+1, pcb_atual->pid, pcb_atual->start_time, pcb_atual->process_len, pcb_atual->priority, pcb_atual->num_threads);
    }*/

    log_inicializar(politica); // Inicializa o log com a política de escalonamento lida do arquivo de entrada
    iniciar_tempo(); // Inicia a contagem de tempo do programa

    // Vamos testar as filas, inserindo e removendo processos

    Fila fila;

    inicializar_fila(&fila, qnt_processos); // Inicializa a fila com capacidade igual à quantidade de processos

    /*for(int i = 0; i < qnt_processos; i++) {
        inserir_no_fim(&fila, processos[i]);
    }*/

    pthread_t geradora;

    ArgumentosGeradora args = {
        .fila = &fila,
        .processos = processos,
        .qnt_processos = qnt_processos
    };

    pthread_create(&geradora, NULL, thread_geradora, &args);

    pthread_join(geradora, NULL);
    
    /* Espera todas as threads de todos os processos terminarem */
    for (int i = 0; i < qnt_processos; i++) {
        for (int j = 0; j < processos[i]->num_threads; j++) {
            pthread_join(processos[i]->thread_ids[j], NULL);
        }
    }


    //printf("Tempo final: %d ms\n", tempo_atual());
    

    /*LIBERANDO MEMÓRIA-----------------------------------------------------*/
    log_gravar_arquivo();
    log_destruir();


    destruir_fila(&fila);

    for(int i = 0; i < qnt_processos; i++) {
        destruir_pcb(processos[i]);
    }

    

    fclose(arquivo_entrada);
    return 0;
}


