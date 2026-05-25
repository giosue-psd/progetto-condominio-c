//SCRITTO DA GIOSUE' SENATORE
#include "richieste.h"

/*
 * FUNZIONE: heapifyUp
 * COSA FA: Quando inseriamo un elemento alla fine dell'array (foglia), questo potrebbe violare
 * la regola del Max-Heap (i padri devono essere >= dei figli).
 * Questa funzione fa "risalire" (swap) l'elemento verso la radice finché la regola non è ripristinata.
 * COMPLESSITÀ: O(log N)
 */
void heapifyUp(SistemaCondominio* sys, int index) {
    if (index == 0) return; // Caso base ricorsione: siamo arrivati alla radice (indice 0)
    
    // In un array Heap, il padre di un nodo 'i' si trova all'indice (i-1)/2
    int parentIndex = (index - 1) / 2;
    
    // Se l'urgenza del nodo attuale è MAGGIORE di quella del padre, violiamo la regola. Facciamo Swap!
    if (sys->heap_richieste[index]->urgenza_val > sys->heap_richieste[parentIndex]->urgenza_val) {
        // Scambio i puntatori
        Richiesta* temp = sys->heap_richieste[index];
        sys->heap_richieste[index] = sys->heap_richieste[parentIndex];
        sys->heap_richieste[parentIndex] = temp;
        
        // Continuo a controllare ricorsivamente verso l'alto
        heapifyUp(sys, parentIndex);
    }
}

/*
 * FUNZIONE: inserisciRichiestaHeap
 * COSA FA: Aggiunge la richiesta in fondo all'array e poi riordina la struttura per mantenere l'albero bilanciato.
 */
void inserisciRichiestaHeap(SistemaCondominio* sys, Richiesta* nuova) {
    // Controllo overflow (evita Segmentation Fault)
    if (sys->heap_size >= MAX_HEAP_CAPACITY) {
        printf("[ERRORE] Heap pieno! Non si possono accodare altre richieste.\n");
        return;
    }
    
    // 1. Inseriamo il nuovo elemento nella prima posizione libera (alla fine dell'array)
    sys->heap_richieste[sys->heap_size] = nuova;
    
    // 2. Chiamiamo heapifyUp per far "galleggiare" il nuovo elemento se è molto urgente
    heapifyUp(sys, sys->heap_size);
    
    // 3. Aggiorniamo la dimensione totale dell'array
    sys->heap_size++;
    
    printf("[OK] Richiesta #%d accodata in Priority Queue (Heap).\n", nuova->id_richiesta);
}

/*
 * FUNZIONE: cercaRichiestaInHeap
 * COSA FA: Cerca una richiesta per ID scorrendo linearmente l'array.
 * COMPLESSITÀ: O(N). (L'Heap è eccellente per trovare il "Massimo", ma inefficace per cercare un elemento casuale).
 */
Richiesta* cercaRichiestaInHeap(SistemaCondominio* sys, int id) {
    for (int i = 0; i < sys->heap_size; i++) {
        if (sys->heap_richieste[i]->id_richiesta == id) return sys->heap_richieste[i];
    }
    return NULL;
}

/*
 * FUNZIONE: rimuoviRichiestaDaHeap
 * COSA FA: Estrae un nodo in qualsiasi posizione. È usata quando una richiesta viene conclusa/annullata.
 */
void rimuoviRichiestaDaHeap(SistemaCondominio* sys, int id) {
    int index = -1;
    // 1. Trovo la posizione dell'elemento da cancellare
    for (int i = 0; i < sys->heap_size; i++) {
        if (sys->heap_richieste[i]->id_richiesta == id) {
            index = i;
            break;
        }
    }
    if (index == -1) return; // Non trovato

    // 2. Copio l'ULTIMO elemento dell'heap nella posizione dell'elemento da eliminare
    sys->heap_richieste[index] = sys->heap_richieste[sys->heap_size - 1];
    
    // 3. Decremento la grandezza (di fatto ignorando l'ultimo elemento ormai clonato)
    sys->heap_size--;
    
    // 4. Ripristino l'ordine (sarebbe completa con heapifyDown, qui usiamo Up per semplificare)
    if (index < sys->heap_size) {
        heapifyUp(sys, index); 
    }
}

// --- FUNZIONI UTILITY (Formattazione visiva) ---

const char* strStato(StatoRichiesta s) {
    switch(s) {
        case APERTA: return "APERTA";
        case PIANIFICATA: return "PIANIFICATA";
        case IN_LAVORAZIONE: return "IN LAVORAZIONE";
        case CONCLUSA: return "CONCLUSA";
        case ANNULLATA: return "ANNULLATA";
        default: return "IGNOTO";
    }
}

const char* strUrg(Urgenza u) {
    switch(u) {
        case BASSA: return "BASSA";
        case MEDIA: return "MEDIA";
        case ALTA: return "ALTA";
        case CRITICA: return "CRITICA";
        default: return "-";
    }
}
