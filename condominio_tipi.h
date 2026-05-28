/* ==============================================================================
 * Scritto da  Giosuè Senatore
 * Scopo: Definizione dei tipi di dato astratti (ADT), delle costanti globali 
 * e delle enumerazioni utilizzate in tutto il sistema. Costituisce 
 * l'ossatura dell'intero database in RAM.
 * ============================================================================== */

#ifndef CONDOMINIO_TIPI_H
#define CONDOMINIO_TIPI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ==============================================================================
 * COSTANTI GLOBALI
 * ============================================================================== */
#define HASH_SIZE 100         // Dimensione della Tabella Hash (numero di bucket)
#define MAX_HEAP_CAPACITY 1000 // Capacità massima della coda di priorità (array-based)

/* ==============================================================================
 * ENUMERAZIONI E TIPI BASE
 * ============================================================================== */

/*
 * Enumerazione: Urgenza
 * Semantica: Definisce i livelli di priorità delle richieste. I valori interi associati 
 * (0-3) sono determinanti per il calcolo del bilanciamento nel Max-Heap.
 */
typedef enum { BASSA = 0, MEDIA = 1, ALTA = 2, CRITICA = 3 } Urgenza;

/*
 * Enumerazione: StatoRichiesta
 * Semantica: Modella la macchina a stati finiti (FSM) del ciclo di vita di una richiesta.
 */
typedef enum { APERTA = 0, PIANIFICATA = 1, IN_LAVORAZIONE = 2, CONCLUSA = 3, ANNULLATA = 4 } StatoRichiesta;

/*
 * Struttura: Data
 * Descrizione: Rappresenta una data cronologica per la schedulazione.
 * Campi: giorno (int), mese (int), anno (int).
 */
typedef struct Data {
    int giorno;
    int mese;
    int anno;
} Data;

/*
 * Struttura: FasciaOraria
 * Descrizione: Definisce un intervallo di tempo giornaliero per l'intervento.
 * Invariante Semantica: 'ora_inizio' deve essere sempre strettamente minore di 'ora_fine'.
 */
typedef struct FasciaOraria {
    int ora_inizio;
    int ora_fine;
} FasciaOraria;


/* ==============================================================================
 * DICHIARAZIONI FORWARD (Risoluzione dipendenze circolari)
 * ============================================================================== */
struct Tecnico;
struct Richiesta;


/* ==============================================================================
 * SPECIFICHE SINTATTICHE E SEMANTICHE DELLE STRUTTURE COMPLESSE
 * ============================================================================== */

/*
 * Struttura: InterventoPianificato
 * Descrizione: Nodo di una lista concatenata semplice che rappresenta il singolo 
 * appuntamento nell'agenda di un Tecnico.
 * Campi:
 * - richiesta: Puntatore alla richiesta fisica (previene ridondanza dei dati).
 * - data_intervento: Struttura Data che indica il giorno fissato.
 * - fascia: Intervallo orario impegnato.
 * - next: Puntatore al nodo successivo nella lista dell'agenda.
 */
typedef struct InterventoPianificato {
    struct Richiesta* richiesta;
    Data data_intervento;
    FasciaOraria fascia;
    struct InterventoPianificato* next;
} InterventoPianificato;

/*
 * Struttura: Tecnico
 * Descrizione: Entità che modella l'operatore. Strutturata per essere archiviata 
 * come nodo all'interno di una Tabella Hash con risoluzione delle collisioni a catena (Chaining).
 * Campi:
 * - id_tecnico: Chiave primaria intera (soggetta a funzione di hashing).
 * - nome: Stringa descrittiva (max 99 char + terminatore).
 * - specializzazione: Competenza tecnica richiesta per il matching (max 49 char).
 * - disponibile: Flag di reperibilità. Se 'false', preclude nuove pianificazioni.
 * - agenda: Puntatore di testa alla lista concatenata degli interventi.
 * - next: Puntatore per incatenare i tecnici residenti nello stesso bucket hash.
 */
typedef struct Tecnico {
    int id_tecnico;
    char nome[100];
    char specializzazione[50];
    bool disponibile;
    InterventoPianificato* agenda;
    struct Tecnico* next;
} Tecnico;

/*
 * Struttura: Richiesta
 * Descrizione: Entità core del sistema. Gestisce i dati del problema segnalato.
 * Il suo ciclo di vita prevede l'allocazione dinamica, la permanenza nel Max-Heap 
 * (finché attiva) e la successiva migrazione nell'albero BST (quando storicizzata).
 * Campi:
 * - id_richiesta: Chiave primaria generata autoincrementalmente dal sistema.
 * - urgenza_val: Cast intero dell'enum Urgenza, vitale per confronti aritmetici nel Max-Heap.
 * - tecnico_assegnato: Puntatore diretto alla struttura Tecnico associata (NULL se APERTA).
 * - (Altri campi anagrafici e di stato: appartamento, tipologia, descrizione, ecc.)
 */
typedef struct Richiesta {
    int id_richiesta;
    char appartamento[50];
    char tipologia[50];
    char descrizione[255];
    Data data_richiesta;
    Urgenza urgenza;
    int urgenza_val;
    StatoRichiesta stato;
    Tecnico* tecnico_assegnato;
} Richiesta;

/*
 * Struttura: NodoBST
 * Descrizione: Nodo costitutivo dell'Albero Binario di Ricerca (BST) usato come Storico.
 * Invariante Semantica: Per ogni nodo N, tutti i nodi del sottoalbero N->left hanno 
 * id_richiesta <= N->richiesta->id_richiesta. Tutti quelli in N->right sono >.
 * Campi:
 * - richiesta: Puntatore alla struttura dati elaborata.
 * - left / right: Puntatori di navigazione dell'albero.
 */
typedef struct NodoBST {
    Richiesta* richiesta;
    struct NodoBST* left;
    struct NodoBST* right;
} NodoBST;

/*
 * Struttura: SistemaCondominio
 * Descrizione: Root struct e orchestratore globale. Contiene i puntatori d'ingresso 
 * di tutte le strutture dati in memoria e la meta-informazione per farle comunicare.
 * Campi:
 * - heap_richieste: Array per la coda di priorità. Limite fisico imposto da MAX_HEAP_CAPACITY.
 * - heap_size: Contatore degli elementi validi nell'array heap (0 <= size < CAPACITY).
 * - bst_storico_root: Puntatore alla radice dell'albero binario di ricerca.
 * - hash_tecnici: Array di teste di lista per la risoluzione hash (dimensione HASH_SIZE).
 * - contatore_richieste: Generatore centralizzato per chiavi primarie univoche.
 */
typedef struct SistemaCondominio {
    Richiesta* heap_richieste[MAX_HEAP_CAPACITY];
    int heap_size;
    NodoBST* bst_storico_root;
    Tecnico* hash_tecnici[HASH_SIZE];
    int contatore_richieste;
} SistemaCondominio;

#endif
