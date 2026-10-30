#ifndef FILA_H
#define FILA_H
#include "pcb.h"

typedef struct {
    pthread_cond_t cond;   /* o escalonador dorme aqui quando a fila está vazia */
    PCB ** itens; // Vetor de ponteiros para PCB
    int tamanho; // Quantos processos estão na fila
    int capacidade; // Capacidade máxima da fila
    pthread_mutex_t mutex; // Mutex para proteger o acesso à fila
} Fila;

void inicializar_fila(Fila * f, int capacidade);

void destruir_fila(Fila * f);

void inserir_no_fim(Fila * f, PCB *pcb);

PCB * remover_do_inicio(Fila * f);

PCB * remover_do_fim(Fila * f);

void remover_pcb(Fila * f, PCB *pcb);

int tamanho_fila(Fila * f);

void imprimir_fila(Fila * f);

#endif