#include <stdlib.h>
#include "pcb.h"

 void criar_pcb(PCB *pcb, int pid, int chegada, int duracao, int prioridade, int num_threads){
    pcb->pid = pid;
    pcb->process_len = duracao;
    pcb->remaining_time = duracao;
    pcb->priority = prioridade;
    pcb->num_threads = num_threads;
    pcb->start_time = chegada;
    pcb->cpu_id = -1;
    pcb->first_dispatch = -1;
    pcb->finish_time = -1;
    pcb->state = READY;
    pcb->cpu_id = -1; // Aqui começa -1, por recomendação do professor, mas depois vai ser atualizado para o id da CPU que está rodando o processo
    pcb->first_dispatch = -1; // Mesma coisa aqui
    pcb->finish_time = -1; // E aqui também
    pcb->thread_ids = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    pthread_mutex_init(&pcb->mutex, NULL); 
    pthread_cond_init(&pcb->cv, NULL);
 }

 void destruir_pcb(PCB *pcb){
    pthread_mutex_destroy(&pcb->mutex);
    pthread_cond_destroy(&pcb->cv);
    free(pcb->thread_ids);
    free(pcb);
 }