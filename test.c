#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "condominio_tipi.h"
#include "richieste.h"
#include "tecnici.h"
#include "pianificazione.h"
#include "storico.h"

/*
 * Funzione: inizializzaSistema
 * Prepara lo stato iniziale del sistema azzerando i contatori e 
 * inizializzando a NULL i puntatori delle strutture dati (Heap, BST, Hash).
 * Svolge un ruolo cruciale per evitare comportamenti indefiniti all'avvio.
 * * Parametri:
 * sys: puntatore alla struttura principale del sistema condominio.
 * * Pre-condizione:
 * sys deve essere un puntatore a una struttura allocata in memoria.
 * * Post-condizione:
 * Il sistema è pronto per accettare in sicurezza nuove richieste e tecnici.
 * * Ritorna:
 * Nessun valore (void).
 */
void inizializzaSistema(SistemaCondominio* sys) {
    sys->heap_size = 0;             
    sys->bst_storico_root = NULL;   
    sys->contatore_richieste = 1;   
    for (int i = 0; i < HASH_SIZE; i++) {
        sys->hash_tecnici[i] = NULL;
    }
}

/*
 * Funzione: liberaSistema
 * Dealloca in modo sicuro e sistematico tutta la memoria allocata
 * dinamicamente durante l'esecuzione del programma (Heap, BST e Hash).
 * * Parametri:
 * sys: puntatore alla struttura del sistema da deallocare.
 * * Pre-condizione:
 * sys deve puntare a un sistema precedentemente inizializzato.
 * * Post-condizione:
 * Tutta la memoria dinamica viene liberata, prevenendo memory leak. 
 * I puntatori interni vengono ripristinati a NULL per sicurezza.
 * * Ritorna:
 * Nessun valore (void).
 */
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

    /*
     * TEST 1: Verifica inserimento nell'Heap
     * Obiettivo: Assicurarsi che l'albero garantisca l'estrazione a priorità
     * massima (valutando urgenza_val) mantenendo il bilanciamento.
     */
    printf("\n[TEST 1] Inserimento Richieste (Heap) e Bilanciamento\n");
    
    Richiesta* r1 = (Richiesta*)malloc(sizeof(Richiesta));
    r1->id_richiesta = sistema.contatore_richieste++;
    strcpy(r1->appartamento, "Piano 1"); strcpy(r1->tipologia, "Elettrico");
    r1->urgenza = BASSA; r1->urgenza_val = 0; r1->stato = APERTA; 
    r1->tecnico_assegnato = NULL;
    inserisciRichiestaHeap(&sistema, r1);

    Richiesta* r2 = (Richiesta*)malloc(sizeof(Richiesta));
    r2->id_richiesta = sistema.contatore_richieste++;
    strcpy(r2->appartamento, "Piano 2"); strcpy(r2->tipologia, "Idraulico");
    r2->urgenza = MEDIA; r2->urgenza_val = 1; r2->stato = APERTA; 
    r2->tecnico_assegnato = NULL;
    inserisciRichiestaHeap(&sistema, r2);

    Richiesta* r3 = (Richiesta*)malloc(sizeof(Richiesta));
    r3->id_richiesta = sistema.contatore_richieste++;
    strcpy(r3->appartamento, "Piano 3"); strcpy(r3->tipologia, "Elettrico");
    r3->urgenza = CRITICA; r3->urgenza_val = 3; r3->stato = APERTA; 
    r3->tecnico_assegnato = NULL;
    inserisciRichiestaHeap(&sistema, r3);

    printf(">> Radice dell'Heap attuale (Max Priorita'): ID %d | Urgenza: %s\n", 
           sistema.heap_richieste[0]->id_richiesta, 
           strUrg(sistema.heap_richieste[0]->urgenza));

    /*
     * TEST 2: Inserimento Hash Table e gestione collisioni
     * Obiettivo: Gli ID scelti per Mario e Luigi (105 e 205) causeranno 
     * di proposito una collisione (se HASH_SIZE è 100), per testare 
     * l'efficacia del chaining nella funzione di registrazione.
     */
    printf("\n[TEST 2] Registrazione Tecnici e Collisioni (Hash Table)\n");
    registraTecnicoHash(&sistema, 105, "Mario Rossi", "Elettrico", true);
    registraTecnicoHash(&sistema, 205, "Luigi Verdi", "Idraulico", true);
    registraTecnicoHash(&sistema, 305, "Gino Gialli", "Elettrico", false); 

    /*
     * TEST 3: Logica di assegnazione
     * Obiettivo: Validare che il sistema rifiuti assegnazioni illogiche,
     * come mansioni non compatibili con la specializzazione o tecnici in ferie.
     */
    printf("\n[TEST 3] Assegnazione (Competenze e Disponibilita')\n");
    Data d = {20, 10, 2024};
    FasciaOraria f1 = {900, 1100};
    
    printf(">> Tentativo Elettrico su Idraulico: ");
    pianificaIntervento(&sistema, r1->id_richiesta, 205, d, f1); 
    
    printf(">> Tentativo Tecnico Non Disponibile: ");
    pianificaIntervento(&sistema, r1->id_richiesta, 305, d, f1); 

    /*
     * TEST 4: Sovrapposizione oraria
     * Obiettivo: Verificare che l'agenda respinga tentativi di pianificazione
     * in cui gli orari si intersecano per il medesimo tecnico.
     */
    printf("\n[TEST 4] Pianificazione e Conflitti Orari\n");
    printf(">> Assegnazione Valida: ");
    pianificaIntervento(&sistema, r1->id_richiesta, 105, d, f1); 
    
    FasciaOraria f2 = {1000, 1200}; 
    printf(">> Tentativo Conflitto Orario: ");
    pianificaIntervento(&sistema, r3->id_richiesta, 105, d, f2); 

    /*
     * TEST 5: Migrazione dei dati (Heap -> Storico BST)
     * Obiettivo: Accertare che una richiesta conclusa abbandoni le code attive
     * per essere correttamente travasata nell'albero binario di sola lettura.
     */
    printf("\n[TEST 5] Aggiornamento Stato e Transazione (Heap -> BST)\n");
    
    // Forzatura manuale dello stato necessaria per avviare il test di 
    // archiviazione bypassando il normale ciclo di vita della richiesta.
    r1->stato = CONCLUSA; 
    archiviaNelloStorico(&sistema, r1);
    rimuoviRichiestaDaHeap(&sistema, r1->id_richiesta);
    
    printf(">> Richiesta ID %d spostata. Dimensione Heap attuale: %d\n", 
           r1->id_richiesta, sistema.heap_size);

    /*
     * TEST 6: Integrità delle ricerche
     * Obiettivo: Assicurarsi che le funzioni restituiscano l'esatto puntatore
     * esplorando strutture eterogenee come Array (Heap) e Alberi (BST).
     */
    printf("\n[TEST 6] Ricerca in strutture multiple\n");
    Richiesta* cerca_attiva = cercaRichiestaInHeap(&sistema, 2);
    if(cerca_attiva) {
        printf(">> ID 2 trovato in HEAP. Stato: %s\n", 
               strStato(cerca_attiva->stato));
    }
    
    Richiesta* cerca_storico = cercaStoricoBST(sistema.bst_storico_root, 1);
    if(cerca_storico) {
        printf(">> ID 1 trovato in BST. Stato: %s\n", 
               strStato(cerca_storico->stato));
    }

    /*
     * TEST 7: Attraversamento dell'albero binario
     * Obiettivo: Controllare visivamente che l'attraversamento scelto nella 
     * stampa rispetti l'ordinamento previsto per le chiavi archiviate.
     */
    printf("\n[TEST 7] Stampa Storico (Controllo Ordinamento)\n");
    
    Richiesta* r4 = (Richiesta*)malloc(sizeof(Richiesta));
    r4->id_richiesta = 99; strcpy(r4->appartamento, "Piano 10"); 
    strcpy(r4->tipologia, "Test"); r4->stato = CONCLUSA; 
    r4->tecnico_assegnato = NULL;
    
    archiviaNelloStorico(&sistema, r4);
    stampaBST(sistema.bst_storico_root); 

    /*
     * TEST 8: Consolidamento complessivo dei dati
     * Obiettivo: Verificare che il totale logico dei nodi isolati coincida 
     * con il flusso delle operazioni generate finora nel sistema.
     */
    printf("\n[TEST 8] Generazione Report Globale\n");
    printf(">> Interventi attivi (Heap O(1)): %d\n", sistema.heap_size);
    printf(">> Interventi archiviati (BST O(N)): %d\n", 
           contaNodiBST(sistema.bst_storico_root));

    printf("\n================ FINE TEST =================\n");
    
    liberaSistema(&sistema);
    return 0;
}
