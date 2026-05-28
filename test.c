#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "condominio_tipi.h"
#include "richieste.h"
#include "tecnici.h"
#include "pianificazione.h"
#include "storico.h"

// Funzioni di utilità per inizializzare e liberare la memoria copiate dal tuo main originale
void inizializzaSistema(SistemaCondominio* sys) {
    sys->heap_size = 0;             
    sys->bst_storico_root = NULL;   
    sys->contatore_richieste = 1;   
    for (int i = 0; i < HASH_SIZE; i++) {
        sys->hash_tecnici[i] = NULL;
    }
}

void liberaSistema(SistemaCondominio* sys) {
    for (int i = 0; i < sys->heap_size; i++) {
        free(sys->heap_richieste[i]);
    }
    sys->heap_size = 0;
    liberaBST(sys->bst_storico_root);
    sys->bst_storico_root = NULL;
    for (int i = 0; i < HASH_SIZE; i++) {
        Tecnico* curr_t = sys->hash_tecnici[i];
        while (curr_t != NULL) {
            InterventoPianificato* curr_i = curr_t->agenda;
            while (curr_i != NULL) {
                InterventoPianificato* temp_i = curr_i;
                curr_i = curr_i->next;
                free(temp_i);
            }
            Tecnico* temp_t = curr_t;
            curr_t = curr_t->next;
            free(temp_t);
        }
        sys->hash_tecnici[i] = NULL;
    }
}

int main() {
    SistemaCondominio sistema;
    inizializzaSistema(&sistema);

    printf("\n================ AVVIO SUITE DI TEST ================\n");

    // --- TEST 1: Verifica della registrazione delle richieste ---
    printf("\n[TEST 1] Inserimento Richieste (Heap) e Bilanciamento\n");
    
    Richiesta* r1 = (Richiesta*)malloc(sizeof(Richiesta));
    r1->id_richiesta = sistema.contatore_richieste++;
    strcpy(r1->appartamento, "Piano 1"); strcpy(r1->tipologia, "Elettrico");
    r1->urgenza = BASSA; r1->urgenza_val = 0; r1->stato = APERTA; r1->tecnico_assegnato = NULL;
    inserisciRichiestaHeap(&sistema, r1);

    Richiesta* r2 = (Richiesta*)malloc(sizeof(Richiesta));
    r2->id_richiesta = sistema.contatore_richieste++;
    strcpy(r2->appartamento, "Piano 2"); strcpy(r2->tipologia, "Idraulico");
    r2->urgenza = MEDIA; r2->urgenza_val = 1; r2->stato = APERTA; r2->tecnico_assegnato = NULL;
    inserisciRichiestaHeap(&sistema, r2);

    Richiesta* r3 = (Richiesta*)malloc(sizeof(Richiesta));
    r3->id_richiesta = sistema.contatore_richieste++;
    strcpy(r3->appartamento, "Piano 3"); strcpy(r3->tipologia, "Elettrico");
    r3->urgenza = CRITICA; r3->urgenza_val = 3; r3->stato = APERTA; r3->tecnico_assegnato = NULL;
    inserisciRichiestaHeap(&sistema, r3);

    printf(">> Radice dell'Heap attuale (Max Priorita'): ID %d | Urgenza: %s\n", 
           sistema.heap_richieste[0]->id_richiesta, strUrg(sistema.heap_richieste[0]->urgenza));

    // --- TEST 2: Test della registrazione dei tecnici ---
    printf("\n[TEST 2] Registrazione Tecnici e Collisioni (Hash Table)\n");
    // ID 105 e 205 causeranno una collisione intenzionale nell'indice 5
    registraTecnicoHash(&sistema, 105, "Mario Rossi", "Elettrico", true);
    registraTecnicoHash(&sistema, 205, "Luigi Verdi", "Idraulico", true);
    registraTecnicoHash(&sistema, 305, "Gino Gialli", "Elettrico", false); // Tecnico in ferie

    // --- TEST 3: Verifica dell’assegnazione corretta ---
    printf("\n[TEST 3] Assegnazione (Competenze e Disponibilita')\n");
    Data d = {20, 10, 2024};
    FasciaOraria f1 = {900, 1100};
    
    printf(">> Tentativo Elettrico su Idraulico: ");
    pianificaIntervento(&sistema, r1->id_richiesta, 205, d, f1); // Fallisce
    
    printf(">> Tentativo Tecnico Non Disponibile: ");
    pianificaIntervento(&sistema, r1->id_richiesta, 305, d, f1); // Fallisce

    // --- TEST 4: Test della pianificazione e gestione conflitti ---
    printf("\n[TEST 4] Pianificazione e Conflitti Orari\n");
    printf(">> Assegnazione Valida: ");
    pianificaIntervento(&sistema, r1->id_richiesta, 105, d, f1); // Passa
    
    FasciaOraria f2 = {1000, 1200}; // Si sovrappone a f1 (900-1100)
    printf(">> Tentativo Conflitto Orario: ");
    pianificaIntervento(&sistema, r3->id_richiesta, 105, d, f2); // Fallisce

    // --- TEST 5: Verifica aggiornamento stato ---
    printf("\n[TEST 5] Aggiornamento Stato e Transazione (Heap -> BST)\n");
    r1->stato = CONCLUSA; // Modifico manualmente per test
    archiviaNelloStorico(&sistema, r1);
    rimuoviRichiestaDaHeap(&sistema, r1->id_richiesta);
    printf(">> Richiesta ID %d spostata. Dimensione Heap attuale: %d\n", r1->id_richiesta, sistema.heap_size);

    // --- TEST 6: Test della ricerca e filtri ---
    printf("\n[TEST 6] Ricerca in strutture multiple\n");
    Richiesta* cerca_attiva = cercaRichiestaInHeap(&sistema, 2);
    if(cerca_attiva) printf(">> ID 2 trovato in HEAP. Stato: %s\n", strStato(cerca_attiva->stato));
    
    Richiesta* cerca_storico = cercaStoricoBST(sistema.bst_storico_root, 1);
    if(cerca_storico) printf(">> ID 1 trovato in BST. Stato: %s\n", strStato(cerca_storico->stato));

    // --- TEST 7: Verifica dello storico interventi ---
    printf("\n[TEST 7] Stampa Storico (Controllo Ordinamento)\n");
    // Inserisco una richiesta fittizia per confermare l'ordinamento numerico
    Richiesta* r4 = (Richiesta*)malloc(sizeof(Richiesta));
    r4->id_richiesta = 99; strcpy(r4->appartamento, "Piano 10"); 
    strcpy(r4->tipologia, "Test"); r4->stato = CONCLUSA; r4->tecnico_assegnato = NULL;
    archiviaNelloStorico(&sistema, r4);
    
    stampaBST(sistema.bst_storico_root); // Dovrebbe stampare prima ID 1, poi ID 99

    // --- TEST 8: Test della generazione dei report ---
    printf("\n[TEST 8] Generazione Report Globale\n");
    printf(">> Interventi attivi (Heap O(1)): %d\n", sistema.heap_size);
    printf(">> Interventi archiviati (BST O(N)): %d\n", contaNodiBST(sistema.bst_storico_root));

    printf("\n================ FINE TEST =================\n");
    
    liberaSistema(&sistema);
    return 0;
}