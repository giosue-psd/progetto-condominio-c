#ifndef STORICO_H
#define STORICO_H

//Scritto da Manuel Vitolo

#include "condominio_tipi.h"
#include "richieste.h"

/*
 * Funzione: inserisciBST
 * Inserisce in modo ricorsivo una richiesta nel Binary Search Tree (BST).
 * Mantiene l'ordinamento posizionando gli ID minori o uguali a sinistra
 * e quelli maggiori a destra.
 * * Parametri:
 * root: puntatore alla radice corrente dell'albero o del sotto-albero.
 * r: puntatore alla struttura Richiesta da inserire nell'albero.
 * * Pre-condizione:
 * r deve puntare a una richiesta validamente allocata in memoria.
 * * Post-condizione:
 * Viene allocato un nuovo NodoBST contenente la richiesta e inserito come
 * foglia nella posizione corretta dell'albero.
 * * Ritorna:
 * Il puntatore alla radice dell'albero aggiornato dopo l'inserimento.
 */
NodoBST* inserisciBST(NodoBST* root, Richiesta* r);

/*
 * Funzione: archiviaNelloStorico
 * Interfaccia di alto livello (wrapper) per registrare una richiesta
 * all'interno dell'albero binario dello storico del condominio.
 * * Parametri:
 * sys: puntatore alla struttura principale del sistema condominio.
 * r: puntatore alla richiesta da inserire nello storico.
 * * Pre-condizione:
 * sys e r devono essere puntatori validi e diversi da NULL.
 * * Post-condizione:
 * La radice dell'albero bst_storico_root viene aggiornata con l'inclusione
 * del nuovo nodo contenente la richiesta r.
 * * Ritorna:
 * Nessun valore (void).
 */
void archiviaNelloStorico(SistemaCondominio* sys, Richiesta* r);

/*
 * Funzione: cercaStoricoBST
 * Esegue una ricerca ricorsiva all'interno del BST per individuare una
 * specifica richiesta tramite il suo identificativo numerico.
 * * Parametri:
 * root: puntatore alla radice del BST in cui effettuare la ricerca.
 * id_target: valore intero dell'ID della richiesta da trovare.
 * * Pre-condizione:
 * L'albero deve rispettare le proprietà di ordinamento del BST.
 * * Post-condizione:
 * Lo stato e la struttura dell'albero rimangono inalterati.
 * * Ritorna:
 * Il puntatore alla struttura Richiesta se trovata, altrimenti NULL.
 */
Richiesta* cercaStoricoBST(NodoBST* root, int id_target);

/* Funzione: stampaBST
 * Mostra sul terminale i dettagli delle richieste eseguendo una visita 
 * In-Order inversa (Destra, Nodo, Sinistra), che produce un output 
 * ordinato in modo decrescente per ID.
 * * Parametri:
 * root: puntatore alla radice dell'albero da stampare.
 * * Pre-condizione:
 * Nessuna. Se root è NULL, la funzione termina senza produrre output.
 * * Post-condizione:
 * I dati delle richieste vengono stampati sul canale di output standard.
 * * Ritorna:
 * Nessun valore (void).
 */
void stampaBST(NodoBST* root);

/*
 * Funzione: liberaBST
 * Dealloca ricorsivamente l'intera memoria occupata dai nodi dell'albero 
 * binario e dalle relative strutture Richiesta associate.
 * * Parametri:
 * root: puntatore alla radice dell'albero o sotto-albero da liberare.
 * * Pre-condizione:
 * root deve puntare a memoria validamente allocata tramite malloc.
 * * Post-condizione:
 * Tutta la memoria dinamica associata al sotto-albero viene rilasciata,
 * prevenendo potenziali memory leak.
 * * Ritorna:
 * Nessun valore (void).
 */
void liberaBST(NodoBST* root);

/*
 * Funzione: contaNodiBST
 * Calcola ricorsivamente il numero totale di nodi (e quindi di richieste)
 * attualmente archiviati all'interno del BST.
 * * Parametri:
 * root: puntatore alla radice dell'albero da conteggiare.
 * * Pre-condizione:
 * Nessuna. Funziona correttamente anche se l'albero è vuoto (root == NULL).
 * * Post-condizione:
 * Lo stato e la struttura dell'albero rimangono del tutto inalterati.
 * * Ritorna:
 * Un valore intero maggiore o uguale a 0 che esprime il conteggio dei nodi.
 */
int contaNodiBST(NodoBST* root);

#endif
