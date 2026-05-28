# Compilatore da utilizzare
CC = gcc

# Flag di compilazione: avvisi abilitati e informazioni di debug
CFLAGS = -Wall -Wextra -g

# Nomi degli eseguibili finali
TARGET_MAIN = condominio
TARGET_TEST = test_condominio

# File sorgente dei moduli condivisi (esclusi i file con il main)
SHARED_SRCS = pianificazione.c richieste.c storico.c tecnici.c
SHARED_OBJS = $(SHARED_SRCS:.c=.o)

# File header (aggiunti come dipendenza per ricompilare se modificati)
HEADERS = condominio_tipi.h pianificazione.h richieste.h storico.h tecnici.h

# Regola principale (eseguita digitando "make": compila entrambi gli eseguibili)
all: $(TARGET_MAIN) $(TARGET_TEST)

# Regola per la creazione dell'eseguibile principale
$(TARGET_MAIN): main.o $(SHARED_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_MAIN) main.o $(SHARED_OBJS)

# Regola per la creazione dell'eseguibile di test
$(TARGET_TEST): test.o $(SHARED_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_TEST) test.o $(SHARED_OBJS)

# Regola generica per la compilazione dei singoli file oggetto
# Gestisce sia i moduli condivisi sia main.o e test.o se gli header cambiano
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per la pulizia della directory (rimuove tutti gli oggetti e i due binari)
clean:
	rm -f *.o $(TARGET_MAIN) $(TARGET_TEST)