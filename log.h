#ifndef LOG_H
#define LOG_H

#include <sys/time.h>
#include <stdio.h>

extern struct timeval T0; // Variável global para armazenar o instante inicial do programa

void iniciar_tempo();

int tempo_atual(); // Tempo atual em ms

void log_inicializar(const char *politica);
void log_registrar(const char *formato, ...);
void log_gravar_arquivo(void);
void log_destruir(void);

#endif