/* ==============================================================================
 * FILE: main.c (VERSIONE TEST AUTOMATIZZATI)
 * SCOPO: Eseguire tutti i test possibili sulle strutture dati (Heap, Hash, BST) 
 * e sulle logiche di business (Pianificazione) senza input manuale.
 * ============================================================================== */
#include "condominio_tipi.h"
#include "richieste.h"
#include "tecnici.h"
#include "pianificazione.h"
#include "storico.h"

// ==============================================================================
// FUNZIONI DI GESTIONE SISTEMA (Mantenute dal vecchio main)
// ==============================================================================

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
    printf("\n[SISTEMA] Garbage Collection manuale terminata. Memoria liberata senza leak.\n");
}

// ==============================================================================
// FUNZIONE HELPER PER AUTOMATIZZARE L'INSERIMENTO DELLE RICHIESTE
// ==============================================================================
void test_CreaRichiesta(SistemaCondominio* sys, const char* app, const char* tipo, const char* desc, Data d, Urgenza u) {
    Richiesta* nuova = (Richiesta*)malloc(sizeof(Richiesta));
    nuova->id_richiesta = sys->contatore_richieste++; 
    strcpy(nuova->appartamento, app);
    strcpy(nuova->tipologia, tipo);
    strcpy(nuova->descrizione, desc);
    nuova->data_richiesta = d;
    nuova->urgenza = u;
    nuova->urgenza_val = (int)u;
    nuova->stato = APERTA;
    nuova->tecnico_assegnato = NULL;
    inserisciRichiestaHeap(sys, nuova);
}

// ==============================================================================
// FUNZIONE MAIN - ESECUZIONE TEST
// ==============================================================================
int main() {
    SistemaCondominio sistema;
    inizializzaSistema(&sistema);

    printf("\n********************************************************\n");
    printf("   AVVIO SUITE DI TEST AUTOMATIZZATI - SISTEMA CONDOMINIO   \n");
    printf("********************************************************\n");

    // ---------------------------------------------------------
    // TEST 1: INSERIMENTO TECNICI (Test Hash Table & Collisioni)
    // ---------------------------------------------------------
    printf("\n>>> TEST 1: REGISTRAZIONE TECNICI E COLLISIONI HASH\n");
    // ID 5 e ID 105 finiranno nello stesso bucket (5 % 100 = 5 e 105 % 100 = 5). Testiamo la lista concatenata!
    registraTecnicoHash(&sistema, 5, "Mario Rossi", "Idraulico", true);
    registraTecnicoHash(&sistema, 105, "Luigi Verdi", "Elettricista", true);
    registraTecnicoHash(&sistema, 12, "Gino Fabbro", "Fabbro", false); // NON disponibile
    registraTecnicoHash(&sistema, 33, "Piero Tubi", "Idraulico", true);

    // ---------------------------------------------------------
    // TEST 2: INSERIMENTO RICHIESTE (Test Max-Heap Priority)
    // ---------------------------------------------------------
    printf("\n>>> TEST 2: INSERIMENTO RICHIESTE (Test Ordinamento Max-Heap)\n");
    Data d1 = {10, 10, 2024};
    Data d2 = {11, 10, 2024};
    
    // Inseriamo con priorità miste per vedere se l'Heap fa "galleggiare" (HeapifyUp) le urgenze critiche
    test_CreaRichiesta(&sistema, "Piano 1 - Int 2", "Idraulico", "Perdita lavandino", d1, BASSA);    // ID 1
    test_CreaRichiesta(&sistema, "Piano 2 - Int 4", "Elettricista", "Blackout totale", d1, CRITICA); // ID 2
    test_CreaRichiesta(&sistema, "Piano Terra", "Fabbro", "Serratura bloccata", d2, ALTA);           // ID 3
    test_CreaRichiesta(&sistema, "Piano 3 - Int 6", "Idraulico", "Tubo rotto", d1, MEDIA);           // ID 4

    printf("\nVerifica Max-Heap attuale (In cima (Indice 0) DEVE esserci la richiesta CRITICA (ID 2)):\n");
    for(int i = 0; i < sistema.heap_size; i++) {
        printf("Indice [%d]: ID %d | Urgenza: %s | App: %s\n", 
               i, sistema.heap_richieste[i]->id_richiesta, strUrg(sistema.heap_richieste[i]->urgenza), sistema.heap_richieste[i]->appartamento);
    }

    // ---------------------------------------------------------
    // TEST 3: PIANIFICAZIONE E GESTIONE ERRORI
    // ---------------------------------------------------------
    printf("\n>>> TEST 3: PIANIFICAZIONE E CONTROLLO SICUREZZE\n");
    FasciaOraria f1 = {900, 1100};  // 09:00 - 11:00
    FasciaOraria f2 = {1000, 1200}; // 10:00 - 12:00 (In conflitto con f1)
    FasciaOraria f3 = {1400, 1600}; // 14:00 - 16:00 (Nessun conflitto)

    printf("\n- TEST 3A (Fallimento - Tecnico in ferie/malattia):\n");
    pianificaIntervento(&sistema, 3, 12, d2, f1); // ID 3 a Gino Fabbro (ID 12, disp=false)

    printf("\n- TEST 3B (Fallimento - Specializzazione Errata):\n");
    pianificaIntervento(&sistema, 2, 5, d1, f1); // Blackout (Elettrico) a Mario Rossi (Idraulico)

    printf("\n- TEST 3C (Successo - Assegnazione Corretta):\n");
    pianificaIntervento(&sistema, 2, 105, d1, f1); // Blackout (ID 2) a Luigi Verdi (ID 105) Elettricista
    pianificaIntervento(&sistema, 1, 5, d1, f1);   // Lavandino (ID 1) a Mario Rossi (ID 5) Idraulico

    printf("\n- TEST 3D (Fallimento - Conflitto Orario):\n");
    pianificaIntervento(&sistema, 4, 5, d1, f2); // Tubo (ID 4) a Mario (ID 5) nello stesso giorno, orario sovrapposto

    printf("\n- TEST 3E (Risoluzione Conflitto - Nuova fascia oraria):\n");
    pianificaIntervento(&sistema, 4, 5, d1, f3); // Tubo (ID 4) a Mario (ID 5) al pomeriggio (Ok)

    // ---------------------------------------------------------
    // TEST 4: AVANZAMENTO STATO E ARCHIVIAZIONE NEL BST
    // ---------------------------------------------------------
    printf("\n>>> TEST 4: TRANSIZIONI DI STATO E STORICO (BST)\n");
    
    // Aggiorno ID 2 in lavorazione (Resta nell'Heap)
    Richiesta* r2 = cercaRichiestaInHeap(&sistema, 2);
    if(r2) r2->stato = IN_LAVORAZIONE;
    printf("[INFO] Richiesta #2 aggiornata a IN_LAVORAZIONE.\n");

    // Aggiorno ID 1 a CONCLUSA -> Viene tolta dall'Heap e messa nel BST!
    Richiesta* r1 = cercaRichiestaInHeap(&sistema, 1);
    if(r1) {
        r1->stato = CONCLUSA;
        archiviaNelloStorico(&sistema, r1);
        rimuoviRichiestaDaHeap(&sistema, 1);
        printf("[INFO] Richiesta #1 CONCLUSA: rimossa dall'Heap, inserita nel BST Storico.\n");
    }

    // Aggiorno ID 3 ad ANNULLATA -> Viene tolta dall'Heap e messa nel BST!
    Richiesta* r3 = cercaRichiestaInHeap(&sistema, 3);
    if(r3) {
        r3->stato = ANNULLATA;
        archiviaNelloStorico(&sistema, r3);
        rimuoviRichiestaDaHeap(&sistema, 3);
        printf("[INFO] Richiesta #3 ANNULLATA: rimossa dall'Heap, inserita nel BST Storico.\n");
    }

    // ---------------------------------------------------------
    // TEST 5: RICERCHE E REPORT
    // ---------------------------------------------------------
    printf("\n>>> TEST 5: REPORT E RICERCHE\n");
    
    printf("\n--- CERCA ELEMENTO (ID 1, archiviato) ---\n");
    Richiesta* search_bst = cercaStoricoBST(sistema.bst_storico_root, 1);
    if(search_bst) printf("Trovato in Storico: App %s, Stato %s\n", search_bst->appartamento, strStato(search_bst->stato));

    printf("\n--- STAMPA STORICO BST (Ordinato per ID) ---\n");
    stampaBST(sistema.bst_storico_root);

    printf("\n--- CARICO LAVORO TECNICI (Hash Table) ---\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        Tecnico* t = sistema.hash_tecnici[i];
        while (t != NULL) {
            int count = 0;
            InterventoPianificato* ag = t->agenda;
            while(ag) { count++; ag = ag->next; }
            if (count > 0 || !t->disponibile) { // Stampo solo se hanno lavori o sono non disponibili
                printf("Tecnico: %s | Interventi Assegnati: %d | Disponibile: %s\n", 
                       t->nome, count, t->disponibile ? "Si" : "No");
            }
            t = t->next;
        }
    }

    printf("\n>>> FINE TEST. AVVIO RILASCIO MEMORIA.\n");
    liberaSistema(&sistema);

    return 0;
}