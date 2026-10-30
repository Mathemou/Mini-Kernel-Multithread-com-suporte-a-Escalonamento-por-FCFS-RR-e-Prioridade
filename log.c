#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include "log.h"

#define TAM_LINHA 128

struct timeval T0;


/* Estado do log: só este arquivo enxerga (static) */
static char **linhas = NULL;        /* vetor de linhas */
static int qnt_linhas = 0;          /* quantas linhas já foram guardadas */
static int capacidade = 0;          /* espaço alocado no vetor */
static char politica_log[16];       /* "FCFS", "RR" ou "PRIORIDADE" */
static pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;

void iniciar_tempo() {
    gettimeofday(&T0, NULL); // Armazena o instante inicial do programa na variável global T0
}

int tempo_atual() {
    struct timeval agora;
    gettimeofday(&agora, NULL); // Armazena o instante atual na variável local agora

    long tempo_decorrido = (agora.tv_sec - T0.tv_sec) * 1000 + (agora.tv_usec - T0.tv_usec) / 1000; // Calcula o tempo decorrido em milissegundos
    return (int)tempo_decorrido; // Retorna o tempo decorrido em milissegundos
}

void log_inicializar(const char *politica) {
    strncpy(politica_log, politica, sizeof(politica_log) - 1);
    politica_log[sizeof(politica_log) - 1] = '\0';
    capacidade = 64;
    qnt_linhas = 0;
    linhas = malloc(capacidade * sizeof(char *));
}

/* Uso igual ao printf: log_registrar("Chegada PID %d", pid);
   A função acrescenta "[t=XXms] [POL] " na frente. */
void log_registrar(const char *formato, ...) {
    char mensagem[TAM_LINHA];

    /* Monta a mensagem a partir do formato e dos argumentos (como o printf faz) */
    va_list args;
    va_start(args, formato);
    vsnprintf(mensagem, sizeof(mensagem), formato, args);
    va_end(args);

    pthread_mutex_lock(&mutex_log);

    /* O tempo é lido DENTRO do mutex: assim as linhas ficam em ordem cronológica */
    char *linha = malloc(TAM_LINHA + 32);
    snprintf(linha, TAM_LINHA + 32, "[t=%dms] [%s] %s", tempo_atual(), politica_log, mensagem);

    /* Se o vetor encheu, dobra o tamanho */
    if (qnt_linhas == capacidade) {
        capacidade *= 2;
        linhas = realloc(linhas, capacidade * sizeof(char *));
    }
    linhas[qnt_linhas++] = linha;

    pthread_mutex_unlock(&mutex_log);
}

/* Chamar só no final, depois de todos os pthread_join */
void log_gravar_arquivo(void) {
    FILE *arq = fopen("log_execucao_minikernel.txt", "w");
    if (arq == NULL) return;
    for (int i = 0; i < qnt_linhas; i++) {
        fprintf(arq, "%s\n", linhas[i]);
    }
    fclose(arq);
}

void log_destruir(void) {
    for (int i = 0; i < qnt_linhas; i++) {
        free(linhas[i]);
    }
    free(linhas);
    linhas = NULL;
    qnt_linhas = 0;
}