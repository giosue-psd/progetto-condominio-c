/*
 * Scritto da Giosuè Senatore
 * Scopo: Definizione dei prototipi per la gestione del ciclo di vita delle richieste 
 * di intervento nel condominio. Le richieste attive sono gestite tramite 
 * una struttura dati di Coda di Priorità (Max-Heap) per garantire che 
 * le urgenze critiche vengano elaborate per prime.
 */

#ifndef RICHIESTE_H
#define RICHIESTE_H

#include "condominio_tipi.h"

/* ==============================================================================
 * SPECIFICHE SINTATTICHE E SEMANTICHE DELLE FUNZIONI HEAP
 * ============================================================================== */

/*
 * Funzione: heapifyUp
 * Descrizione: Ripristina la proprietà del Max-Heap spostando verso l'alto (verso 
 * la radice) un nodo inserito se la sua urgenza è maggiore di quella del padre.
 * * Parametri:
 * sys: puntatore all'istanza centrale del SistemaCondominio.
 * index: intero che rappresenta l'indice nell'array del nodo da far "galleggiare".
 * * Pre-condizione:
 * 'sys' non deve essere NULL. 'index' deve essere un indice valido 
 * all'interno della dimensione attuale dell'heap (0 <= index < sys->heap_size).
 * * Post-condizione:
 * L'array heap_richieste è parzialmente riordinato in modo che l'elemento 
 * all'indice originale rispetti la regola gerarchica del Max-Heap.
 * * Ritorna:
 * Nessun valore (void).
 */
void heapifyUp(SistemaCondominio* sys, int index);

/*
 * Funzione: inserisciRichiestaHeap
 * Descrizione: Accoda una nuova richiesta nel sistema inserendola prima in fondo 
 * all'array e poi riordinando l'albero (tramite heapifyUp) per 
 * posizionarla correttamente in base alla sua urgenza.
 * * Parametri:
 * sys: puntatore all'istanza centrale del SistemaCondominio.
 * nuova: puntatore alla struttura Richiesta appena allocata da inserire.
 * * Pre-condizione:
 * 'sys' e 'nuova' non devono essere NULL. L'array dell'heap non deve 
 * aver superato la MAX_HEAP_CAPACITY.
 * * Post-condizione:
 * La richiesta è inserita nella coda, 'sys->heap_size' è incrementato di 1, 
 * e la struttura Max-Heap è bilanciata.
 * * Ritorna:
 * Nessun valore (void).
 */
void inserisciRichiestaHeap(SistemaCondominio* sys, Richiesta* nuova);

/*
 * Funzione: cercaRichiestaInHeap
 * Descrizione: Trova il puntatore fisico a una richiesta attiva cercandola tramite 
 * il suo identificativo univoco (ID).
 * * Parametri:
 * sys: puntatore all'istanza centrale del SistemaCondominio.
 * id: numero intero identificativo della richiesta da cercare.
 * * Pre-condizione:
 * 'sys' non deve essere NULL. L'ID deve essere un intero positivo.
 * * Post-condizione:
 * Nessuna modifica interna alla struttura dati (operazione in sola lettura).
 * * Ritorna:
 * Il puntatore alla struttura Richiesta corrispondente all'ID se trovata, 
 * altrimenti restituisce NULL.
 */
Richiesta* cercaRichiestaInHeap(SistemaCondominio* sys, int id);

/*
 * Funzione: rimuoviRichiestaDaHeap
 * Descrizione: Estrae e rimuove dall'Heap una specifica richiesta (generalmente 
 * richiamata quando questa viene conclusa o annullata) e garantisce 
 * il ripristino dell'ordinamento.
 * * Parametri:
 * sys: puntatore all'istanza centrale del SistemaCondominio.
 * id: numero intero identificativo della richiesta da rimuovere.
 * * Pre-condizione:
 * 'sys' non deve essere NULL. La richiesta con l'ID specificato 
 * deve essere presente all'interno dell'array dell'heap.
 * * Post-condizione:
 * La richiesta è rimossa logicamente e 'sys->heap_size' è decrementato di 1. 
 * L'elemento in coda viene spostato per coprire il buco e riordinato per 
 * mantenere la validità del Max-Heap.
 * * Ritorna:
 * Nessun valore (void).
 */
void rimuoviRichiestaDaHeap(SistemaCondominio* sys, int id);


/* ==============================================================================
 * SPECIFICHE SINTATTICHE E SEMANTICHE DELLE UTILITY DI FORMATTAZIONE
 * ============================================================================== */

/*
 * Funzione: strStato
 * Descrizione: Traduce un valore enumerativo 'StatoRichiesta' in una stringa di 
 * caratteri leggibile per facilitare l'output a schermo nei menu.
 * * Parametri:
 * s: Valore di tipo enum StatoRichiesta.
 * * Pre-condizione:
 * Nessuna (accetta il parametro passato per valore).
 * * Post-condizione:
 * Nessuna modifica di stato (funzione pura).
 * * Ritorna:
 * Un puntatore a una stringa costante (es. "IN LAVORAZIONE").
 */
const char* strStato(StatoRichiesta s);

/*
 * Funzione: strUrg
 * Descrizione: Traduce un valore enumerativo 'Urgenza' in una stringa di caratteri 
 * leggibile, molto utile per stampare il riepilogo in ordine di priorità.
 * * Parametri:
 * u: Valore di tipo enum Urgenza.
 * * Pre-condizione:
 * Nessuna.
 * * Post-condizione:
 * Nessuna modifica di stato (funzione pura).
 * * Ritorna:
 * Un puntatore a una stringa costante (es. "CRITICA", "ALTA").
 */
const char* strUrg(Urgenza u);

#endif
