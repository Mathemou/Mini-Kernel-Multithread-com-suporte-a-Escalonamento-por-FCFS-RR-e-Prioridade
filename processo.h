#ifndef PROCESSO_H        /* "include guard": impede que o arquivo */
#define PROCESSO_H        /* seja incluído duas vezes */
#include "pcb.h"
#include "fila.h"

extern int generator_done; // Variavel que vai dizer se a thread gerado acabou

typedef struct {
    Fila *fila;
    PCB **processos;
    int qnt_processos;
} ArgumentosGeradora;

void *thread_geradora(void *arg);   /* toda função de thread tem essa assinatura */

void ordenar_processos_por_chegada(PCB **processos, int qnt_processos);

void *thread_processo(void *arg);

#endif