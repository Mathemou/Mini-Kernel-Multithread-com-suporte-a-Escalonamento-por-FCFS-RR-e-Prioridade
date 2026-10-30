#include <stdio.h>
#include "processo.h"
#include "fila.h"
#include <stdlib.h>
#include <unistd.h>    
#include <pthread.h>   
#include "log.h"       

int generator_done = 0;

void *thread_geradora(void *arg){
    ArgumentosGeradora *a = (ArgumentosGeradora *)arg;   /* recupera os argumentos */

    ordenar_processos_por_chegada(a->processos, a->qnt_processos);

    for (int i = 0; i < a->qnt_processos; i++) {
        PCB *p = a->processos[i];
        

        while (tempo_atual() < p->start_time) {
            usleep(1000);   /* dorme 1 ms */
        }

        /* 2. criar as num_threads threads do processo (pthread_create
              com um TCB alocado para cada, guardando em p->thread_ids[j]) */
        for (int j = 0; j < p->num_threads; j++) {
            TCB *t = malloc(sizeof(TCB));   /* um TCB para cada thread */
            t->pcb = p;
            t->thread_index = j;
            pthread_create(&p->thread_ids[j], NULL, thread_processo, t);
        }

        /* 3. inserir p na fila e acordar o escalonador */
        pthread_mutex_lock(&a->fila->mutex);
        inserir_no_fim(a->fila, p);

        log_registrar("Chegada PID %d", p->pid);

        pthread_cond_signal(&a->fila->cond);    /* "acorda, chegou processo" */
        pthread_mutex_unlock(&a->fila->mutex);

        
        /* 4. registrar no log: "Chegada PID p" */
    }

    /* 5. generator_done = 1  (sob mutex, pois o escalonador vai ler) */
    pthread_mutex_lock(&a->fila->mutex);
    generator_done = 1;
    pthread_cond_broadcast(&a->fila->cond); /* acorda o escalonador para ele ver que acabou */ // Não entendi muito bem iso, vou voltar aqui depois [???]
    pthread_mutex_unlock(&a->fila->mutex);
    return NULL;
}

void ordenar_processos_por_chegada(PCB **processos, int qnt_processos) { // Um insertion sortzinho básico né pae
    for (int i = 0; i < qnt_processos - 1; i++) {
        for (int j = 0; j < qnt_processos - i - 1; j++) {
            if (processos[j]->start_time > processos[j + 1]->start_time ||
                (processos[j]->start_time == processos[j + 1]->start_time && processos[j]->pid > processos[j + 1]->pid)) {
                PCB *temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

void *thread_processo(void *arg) {
    TCB *t = (TCB *)arg;
    free(t);          /* a thread libera o próprio TCB ao terminar */
    return NULL;
}