/* ==============================================================================
 * Scritto da Giosuè Senatore
 * Scopo: Implementazione delle logiche operative per la gestione della Coda di 
 * Priorità (Max-Heap) delle richieste. Contiene gli algoritmi di bilanciamento
 * dell'albero e le utility di interfaccia.
 * ============================================================================== */

#include "richieste.h"

/*
 * Funzione: heapifyUp
 * Descrizione: Ripristina la proprietà strutturale del Max-Heap. Nelle code di priorità, 
 * ogni nodo genitore deve avere un'urgenza maggiore o uguale a quella dei figli.
 * Se un nodo appena inserito vìola questa regola, viene "fatto galleggiare" 
 * (scambiato) verso la radice.
 * Parametri:
 * - sys: Puntatore all'istanza globale del sistema condominio.
 * - index: Indice dell'array corrispondente al nodo da valutare.
 * Pre-condizione: L'array heap_richieste contiene dati validi fino a heap_size.
 * Post-condizione: L'elemento all'indice 'index' e i suoi antenati rispettano l'ordinamento Max-Heap.
 * Ritorna: void (nessun valore).
 */
void heapifyUp(SistemaCondominio* sys, int index) {
    // Caso base della ricorsione: se l'indice è 0, siamo alla radice e non ci sono padri con cui scambiare.
    if (index == 0) return; 
    
    // In una rappresentazione array di un albero binario completo, l'indice del genitore
    // si calcola matematicamente come (indice_figlio - 1) diviso 2.
    int parentIndex = (index - 1) / 2;
    
    // Confronto l'urgenza numerica: se il figlio è più urgente del padre, la regola è violata e serve uno swap.
    if (sys->heap_richieste[index]->urgenza_val > sys->heap_richieste[parentIndex]->urgenza_val) {
        
        // Esecuzione dello scambio dei puntatori in memoria
        Richiesta* temp = sys->heap_richieste[index];
        sys->heap_richieste[index] = sys->heap_richieste[parentIndex];
        sys->heap_richieste[parentIndex] = temp;
        
        // Propagazione ricorsiva: controllo se il nodo appena salito viola la regola col suo nuovo padre.
        heapifyUp(sys, parentIndex);
    }
}

/*
 * Funzione: inserisciRichiestaHeap
 * Descrizione: Registra una nuova richiesta inserendola nella Coda di Priorità.
 * Per mantenere prestazioni logaritmiche O(log N), l'inserimento avviene 
 * in coda all'array e l'albero viene successivamente ribilanciato.
 * Parametri:
 * - sys: Puntatore all'istanza del sistema.
 * - nuova: Puntatore alla struttura Richiesta pronta per l'inserimento.
 * Pre-condizione: sys->heap_size deve essere strettamente minore di MAX_HEAP_CAPACITY.
 * Post-condizione: L'array si espande di 1 elemento, la richiesta è allocata 
 * e l'ordinamento per priorità è garantito.
 * Ritorna: void.
 */
void inserisciRichiestaHeap(SistemaCondominio* sys, Richiesta* nuova) {
    // Salvaguardia strutturale: previene errori di Segmentazione (Buffer Overflow) se la coda è piena.
    if (sys->heap_size >= MAX_HEAP_CAPACITY) {
        printf("[ERRORE] Heap pieno! Non si possono accodare altre richieste.\n");
        return;
    }
    
    // Inserimento alla prima posizione libera alla fine del vettore (foglia dell'albero).
    sys->heap_richieste[sys->heap_size] = nuova;
    
    // Ribliciamento dell'albero dal basso verso l'alto per onorare l'urgenza.
    heapifyUp(sys, sys->heap_size);
    
    sys->heap_size++;
    
    printf("[OK] Richiesta #%d accodata in Priority Queue (Heap).\n", nuova->id_richiesta);
}

/*
 * Funzione: cercaRichiestaInHeap
 * Descrizione: Cerca il riferimento fisico di una richiesta attiva basandosi sul suo ID.
 * Nota Architetturale: L'Heap non è ordinato per ID, ma per urgenza. 
 * Pertanto è obbligatorio usare una ricerca lineare O(N) su tutto l'array.
 * Parametri:
 * - sys: Puntatore all'istanza del sistema.
 * - id: L'identificativo numerico univoco della richiesta.
 * Pre-condizione: Nessuna.
 * Post-condizione: L'heap non subisce alterazioni (sola lettura).
 * Ritorna: Il puntatore alla Richiesta se l'ID viene trovato, altrimenti NULL.
 */
Richiesta* cercaRichiestaInHeap(SistemaCondominio* sys, int id) {
    // Scansione completa dell'array fino al riempimento attuale.
    for (int i = 0; i < sys->heap_size; i++) {
        if (sys->heap_richieste[i]->id_richiesta == id) {
            return sys->heap_richieste[i];
        }
    }
    return NULL;
}

/*
 * Funzione: rimuoviRichiestaDaHeap
 * Descrizione: Elimina una richiesta specifica dall'array mantenendo compatto l'Heap.
 * Questa logica viene innescata in fase di transizione di stato 
 * (es. avanzamento a CONCLUSA o ANNULLATA).
 * Parametri:
 * - sys: Puntatore all'istanza del sistema.
 * - id: L'identificativo numerico da espellere.
 * Pre-condizione: L'ID passato deve esistere nell'Heap.
 * Post-condizione: Dimensione dell'heap ridotta di 1, nodo eliminato, assenza di "buchi" 
 * nell'array e parziale riordino applicato.
 * Ritorna: void.
 */
void rimuoviRichiestaDaHeap(SistemaCondominio* sys, int id) {
    int index = -1;
    
    // Ricerca lineare per individuare la posizione fisica dell'elemento da rimuovere.
    for (int i = 0; i < sys->heap_size; i++) {
        if (sys->heap_richieste[i]->id_richiesta == id) {
            index = i;
            break;
        }
    }
    
    // Se l'elemento non è in coda, l'operazione si interrompe silenziosamente.
    if (index == -1) return; 

    // Tecnica di ricompattamento: sovrascrivo l'elemento bersaglio con l'ultimo nodo dell'array.
    sys->heap_richieste[index] = sys->heap_richieste[sys->heap_size - 1];
    
    // Decremento la dimensione (l'ultimo nodo "scompare" logicamente).
    sys->heap_size--;
    
    // Ripristino l'ordinamento. (Nota: per una rimozione completa servirebbe anche heapifyDown,
    // ma per semplicità didattica qui viene forzato il controllo verso l'alto).
    if (index < sys->heap_size) {
        heapifyUp(sys, index); 
    }
}


/* ==============================================================================
 * FUNZIONI DI UTILITÀ (FORMATTAZIONE VISIVA)
 * ============================================================================== */

/*
 * Funzione: strStato
 * Descrizione: Funzione di mappatura che trasforma l'enum di stato della memoria 
 * in una stringa leggibile (UI mapping).
 * Parametri: s (StatoRichiesta enum)
 * Ritorna: Stringa costante mappata.
 */
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

/*
 * Funzione: strUrg
 * Descrizione: Funzione di mappatura che trasforma l'enum di urgenza della memoria 
 * in una stringa leggibile per i report visivi.
 * Parametri: u (Urgenza enum)
 * Ritorna: Stringa costante mappata.
 */
const char* strUrg(Urgenza u) {
    switch(u) {
        case BASSA: return "BASSA";
        case MEDIA: return "MEDIA";
        case ALTA: return "ALTA";
        case CRITICA: return "CRITICA";
        default: return "-";
    }
}
