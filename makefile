# Compilador e opções
CC      = gcc
CFLAGS  = -Wall -Wextra -pthread -g

# Todos os .c do projeto. Adicione aqui cada novo módulo
SRCS    = main.c pcb.c fila.c log.c processo.c

# Nome do executável exigido pelo enunciado
EXEC    = trabSO

# Arquivo de entrada padrão para os testes (pode ser trocado na linha de comando)
ENTRADA ?= casos_publicos_v1/entrada_01.txt

.PHONY: monoprocessador multiprocessador clean valgrind helgrind

# Versão monoprocessador
monoprocessador:
	$(CC) $(CFLAGS) -DMONOPROCESSADOR $(SRCS) -o $(EXEC)

# Versão multiprocessador
multiprocessador:
	$(CC) $(CFLAGS) -DMULTIPROCESSADOR $(SRCS) -o $(EXEC)

# Verifica vazamentos de memória
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXEC) $(ENTRADA)

# Verifica condições de corrida e ordem de travamento
helgrind:
	valgrind --tool=helgrind ./$(EXEC) $(ENTRADA)

# Apaga o executável e o log gerado
clean:
	rm -f $(EXEC) log_execucao_minikernel.txt