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


/* La funzione "Anti Memory Leak". Richiamata all'uscita dal programma (Opzione 0) */
void liberaSistema(SistemaCondominio* sys) {
    // 1. Pulizia dell'Heap (Libero le strutture Richiesta puntate dall'array)
    for (int i = 0; i < sys->heap_size; i++) {
        free(sys->heap_richieste[i]);
    }
    sys->heap_size = 0;

    // 2. Pulizia del BST (Richiama la funzione ricorsiva Post-Order nel file storico.c)
    liberaBST(sys->bst_storico_root);
    sys->bst_storico_root = NULL;

    // 3. Pulizia della Tabella Hash e Agende interne (Liste concatenate di liste)
    for (int i = 0; i < HASH_SIZE; i++) {
        Tecnico* curr_t = sys->hash_tecnici[i];
        while (curr_t != NULL) {
            
            // PRIMA devo liberare tutta l'agenda concatenata di questo tecnico
            InterventoPianificato* curr_i = curr_t->agenda;
            while (curr_i != NULL) {
                InterventoPianificato* temp_i = curr_i;
                curr_i = curr_i->next;
                free(temp_i); // Libera il nodo appuntamento
            }
            
            // DOPO aver svuotato l'agenda, posso liberare la struttura Tecnico
            Tecnico* temp_t = curr_t;
            curr_t = curr_t->next;
            free(temp_t);
        }
        sys->hash_tecnici[i] = NULL;
    }
    printf("[SISTEMA] Garbace Collection manuale terminata. Memoria liberata.\n");
}

int main() {
    // L'istanza principale del database è allocata direttamente sullo Stack del main
    SistemaCondominio sistema;
    inizializzaSistema(&sistema);

    int scelta = -1;

    while (scelta != 0) {
        printf("\n======= MENU GESTIONE CONDOMINIO =======\n");
        printf("1. Inserisci Nuova Richiesta (O(log N) -> Max-Heap)\n");
        printf("2. Registra Nuovo Tecnico (O(1) -> Hash Table)\n");
        printf("3. Mostra Richieste Attive (Priority Queue)\n");
        printf("4. Mostra Storico Interventi (O(log N) -> BST)\n");
        printf("5. Pianifica Intervento\n");
        printf("6. Avanza Stato Intervento\n");
        printf("7. Ricerca Rapida tramite ID\n");
        printf("8. Monitoraggio Carico Lavoro Tecnici\n");
        printf("9. Genera Report Globale\n");
        printf("0. Esci e Libera Memoria\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &scelta);

    }
    return 0; 
}