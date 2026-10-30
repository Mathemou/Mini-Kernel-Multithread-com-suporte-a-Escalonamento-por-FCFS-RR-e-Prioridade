#ifndef PCB_H        /* "include guard": impede que o arquivo */
#define PCB_H        /* seja incluído duas vezes */

#include <pthread.h>

typedef enum { 
    READY, // Tá pronto
    RUNNING, // Tá rodando
    FINISHED // Acabou
} ProcessState;  

typedef struct { 
    int process_len; /* duração total (ms) */ 
    int remaining_time; /* tempo restante (ms) */ 
    int priority; /* 1 (mais alta) a 5 */ 
    int num_threads; 
    int start_time; /* chegada (ms) */ 
    int cpu_id; /* CPU atual; -1 se nenhuma */ 
    int first_dispatch; /* instante do 1º despacho; -1 se ainda não */ 
    int finish_time; /* instante de finalização; -1 se ainda não */ 
    ProcessState state; 
    pthread_mutex_t mutex; 
    pthread_cond_t cv; 
    pthread_t *thread_ids; 
    int pid; 
} PCB;  
 
typedef struct { 
    PCB *pcb; 
    int thread_index; 
} TCB;

void criar_pcb(PCB *pcb, int pid, int chegada, int duracao, int prioridade, int num_threads);
void destruir_pcb(PCB *pcb);

#endif