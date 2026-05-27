# Compilatore da utilizzare
CC = gcc

# Flag di compilazione: avvisi abilitati e informazioni di debug
CFLAGS = -Wall -Wextra -g

# Nome dell'eseguibile finale
TARGET = condominio

# File sorgente
SRCS = main.c pianificazione.c richieste.c storico.c tecnici.c

# File oggetto (sostituisce l'estensione .c con .o nella lista SRCS)
OBJS = $(SRCS:.c=.o)

# File header (aggiunti come dipendenza per ricompilare se modificati)
HEADERS = condominio_tipi.h pianificazione.h richieste.h storico.h tecnici.h

# Regola principale (quella eseguita di default digitando "make")
all: $(TARGET)

# Regola per la creazione dell'eseguibile
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regola per la compilazione dei singoli file oggetto
# Se uno dei .c o degli header cambia, il file .o relativo viene ricompilato
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per la pulizia della directory
clean:
	rm -f $(OBJS) $(TARGET)