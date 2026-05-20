/* ==============================================================================
 * FILE: main.c
 * SCOPO: Gestire l'interfaccia a riga di comando (CLI) e orchestrare tutti i sottomoduli.
 * Strutture: Max-Heap (Code), Hash Table (Tecnici), BST (Storico).
 * ============================================================================== */
#include "condominio_tipi.h"
#include "richieste.h"
#include "tecnici.h"
#include "pianificazione.h"
#include "storico.h"

/* Inizializza tutto a ZERO e NULL. È vitale in C, altrimenti i puntatori conterrebbero spazzatura di RAM (garbage) */
void inizializzaSistema(SistemaCondominio* sys) {
    sys->heap_size = 0;             
    sys->bst_storico_root = NULL;   
    sys->contatore_richieste = 1;   
    
    // Pulisco tutti i bucket della Hash Table
    for (int i = 0; i < HASH_SIZE; i++) {
        sys->hash_tecnici[i] = NULL;
    }
}
int main() {

    return 0; 
}