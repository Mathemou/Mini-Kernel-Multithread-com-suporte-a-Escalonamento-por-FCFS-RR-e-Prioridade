#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

/*Quem chamar estar com o f->mutex travado*/

void inicializar_fila(Fila * f, int capacidade){
    pthread_cond_init(&f->cond, NULL);
    f->itens = (PCB **)malloc(capacidade * sizeof(PCB *));
    f->tamanho = 0;
    f->capacidade = capacidade;
    pthread_mutex_init(&f->mutex, NULL);
}

void destruir_fila(Fila * f){
    pthread_cond_destroy(&f->cond);
    pthread_mutex_destroy(&f->mutex);
    free(f->itens);
}

void inserir_no_fim(Fila * f, PCB *pcb){
    //pthread_mutex_lock(&f->mutex); // Tem que travar o mutex antes de acessar a fila, pra evitar condições de corrida
    if(f->tamanho < f->capacidade){ // Se tiver cheia ela nãõ adiciona nada, talvez seja bom devolver um erro aqui? Posso perguntar pro professor [???]
        f->itens[f->tamanho] = pcb;
        f->tamanho++;
    }
    //pthread_mutex_unlock(&f->mutex); // Destrava
}

PCB * remover_do_inicio(Fila * f){
    //pthread_mutex_lock(&f->mutex);
    PCB * pcb = NULL;
    if(f->tamanho > 0){ // Primeiro verifica se há algum elemento na fila
        pcb = f->itens[0];
        for(int i =1; i < f->tamanho; i++){
            f->itens[i-1] = f->itens[i];
        }
        f->tamanho--;
    }
    //pthread_mutex_unlock(&f->mutex);
    return pcb;
}

PCB * remover_do_fim(Fila * f){
    //pthread_mutex_lock(&f->mutex);
    PCB * pcb = NULL;
    if(f->tamanho > 0){
        pcb = f->itens[f->tamanho - 1];
        f->tamanho--;
    }
    //pthread_mutex_unlock(&f->mutex);
    return pcb;
}

void remover_pcb(Fila * f, PCB *pcb){
    (void)f;     /* TODO: implementar */
    (void)pcb;
}

int tamanho_fila(Fila * f){
    //pthread_mutex_lock(&f->mutex);
    int tamanho = f->tamanho;
    //pthread_mutex_unlock(&f->mutex);
    return tamanho;
}

void imprimir_fila(Fila * f){
    //pthread_mutex_lock(&f->mutex);
    printf("Fila (tamanho: %d):\n", f->tamanho);
    for(int i = 0; i < f->tamanho; i++){
        printf("Processo %d: PID: %d, Chegada: %d, Duração: %d, Prioridade: %d, Número de threads: %d\n", i+1, f->itens[i]->pid, f->itens[i]->start_time, f->itens[i]->process_len, f->itens[i]->priority, f->itens[i]->num_threads);
    }
    //pthread_mutex_unlock(&f->mutex);
}