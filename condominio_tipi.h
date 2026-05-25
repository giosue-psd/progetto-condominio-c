//SCRITTO DA GIOSUE' SENATORE
#ifndef CONDOMINIO_TIPI_H // Guardia d'inclusione: impedisce che questo file venga incluso più volte causando errori di compilazione
#define CONDOMINIO_TIPI_H

#include <stdio.h>   // Per le funzioni di input/output base (printf, scanf)
#include <stdlib.h>  // Per la gestione della memoria dinamica (malloc, free) e funzioni standard
#include <stdbool.h> // Per il tipo booleano (true/false) introdotto nel C99
#include <string.h>  // Per la manipolazione delle stringhe (strcpy, strcmp)

// --- COSTANTI GLOBALI ---
// Definiamo le dimensioni massime delle nostre strutture dati principali
#define HASH_SIZE 100         // Numero di celle (bucket) nella Tabella Hash per i tecnici
#define MAX_HEAP_CAPACITY 1000 // Dimensione massima dell'array che ospiterà l'Heap delle richieste

// --- ENUMERAZIONI E TIPI BASE ---
// Le enum assegnano un nome leggibile a valori interi sequenziali, partendo di default da 0.
typedef enum { BASSA = 0, MEDIA = 1, ALTA = 2, CRITICA = 3 } Urgenza;
typedef enum { APERTA = 0, PIANIFICATA = 1, IN_LAVORAZIONE = 2, CONCLUSA = 3, ANNULLATA = 4 } StatoRichiesta;

// Struttura per gestire le date in modo strutturato invece che come singola stringa
typedef struct Data {
    int giorno;
    int mese;
    int anno;
} Data;

// Struttura per gestire intervalli di tempo in modo numerico (es. 900 = 09:00, 1100 = 11:00)
typedef struct FasciaOraria {
    int ora_inizio;
    int ora_fine;
} FasciaOraria;

// --- DICHIARAZIONI FORWARD ---
// Queste dichiarazioni informano il compilatore che queste struct esistono. 
// Sono necessarie perché InterventoPianificato, Tecnico e Richiesta si riferiscono a vicenda (dipendenze circolari).
struct Tecnico;
struct Richiesta;

// --- STRUTTURE COMPLESSE ---

// Nodo della lista concatenata che rappresenta l'agenda di un singolo tecnico
typedef struct InterventoPianificato {
    struct Richiesta* richiesta;        // Puntatore alla richiesta fisica (evita di duplicare i dati della richiesta)
    Data data_intervento;               // Quando avviene l'intervento
    FasciaOraria fascia;                // Orario dell'intervento
    struct InterventoPianificato* next; // Puntatore al prossimo impegno in agenda (Lista Concatenata)
} InterventoPianificato;

// Entità Tecnico (Gestita tramite Tabella Hash)
typedef struct Tecnico {
    int id_tecnico;                     // Chiave primaria (usata per calcolare l'indice dell'Hash)
    char nome[100];                     // Array di caratteri per il nome
    char specializzazione[50];          // Array di caratteri per la specializzazione (es. "Idraulico")
    bool disponibile;                   // Flag booleano: true se attivo, false se in ferie/malattia
    InterventoPianificato* agenda;      // Testa della lista concatenata degli interventi assegnati a questo tecnico
    struct Tecnico* next;               // Puntatore essenziale per la "Risoluzione delle Collisioni" (Chaining) nell'Hash Table
} Tecnico;

// Entità Richiesta (Gestita tramite Max-Heap e poi spostata in BST)
typedef struct Richiesta {
    int id_richiesta;                   // Chiave primaria autoincrementante
    char appartamento[50];              // Identificativo appartamento (es. "Piano 1, Interno 2")
    char tipologia[50];                 // Tipologia problema (es. "Elettrico")
    char descrizione[255];              // Testo lungo descrittivo
    Data data_richiesta;                // Data di sottomissione
    Urgenza urgenza;                    // Valore Enum (per la leggibilità)
    int urgenza_val;                    // Valore intero copiato dall'Enum, fondamentale per calcolare le priorità nel Max-Heap
    StatoRichiesta stato;               // Stato di avanzamento (Enum)
    Tecnico* tecnico_assegnato;         // Puntatore al tecnico che se ne occuperà (se NULL, non ancora assegnato)
} Richiesta;

// NODO PER L'ALBERO BINARIO DI RICERCA (BST) - Storico
typedef struct NodoBST {
    Richiesta* richiesta;               // Dati effettivi contenuti nel nodo (Puntatore alla richiesta originale)
    struct NodoBST* left;               // Puntatore al figlio sinistro (ID minori)
    struct NodoBST* right;              // Puntatore al figlio destro (ID maggiori)
} NodoBST;

// SISTEMA CENTRALE AGGIORNATO (L'orchestratore di tutto il database in RAM)
typedef struct SistemaCondominio {
    // 1. Coda di Priorità (Max-Heap) basata su array per le richieste attive. O(log N) per l'inserimento.
    Richiesta* heap_richieste[MAX_HEAP_CAPACITY]; // Array di puntatori
    int heap_size;                                // Quanti elementi sono attualmente validi nell'array dell'Heap

    // 2. Albero Binario di Ricerca (BST) per lo Storico. O(log N) per la ricerca.
    NodoBST* bst_storico_root;                    // Puntatore alla "Radice" dell'albero

    // 3. Tabella Hash per i tecnici. O(1) nel caso medio per l'accesso.
    Tecnico* hash_tecnici[HASH_SIZE];             // Array di puntatori alla testa di liste concatenate (bucket)

    // Contatore globale usato per assegnare ID univoci e sequenziali alle nuove richieste
    int contatore_richieste;
} SistemaCondominio;

#endif // Fine della guardia d'inclusione
