#ifndef RICHIESTE_H
#define RICHIESTE_H

#include "condominio_tipi.h"

// Prototipi delle funzioni per la gestione dell'Heap
void heapifyUp(SistemaCondominio* sys, int index);
void inserisciRichiestaHeap(SistemaCondominio* sys, Richiesta* nuova);
Richiesta* cercaRichiestaInHeap(SistemaCondominio* sys, int id);
void rimuoviRichiestaDaHeap(SistemaCondominio* sys, int id);

// Utility per convertire le Enum in stringhe testuali leggibili
const char* strStato(StatoRichiesta s);
const char* strUrg(Urgenza u);

#endif
