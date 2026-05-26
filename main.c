/*scritto da Jacopo Salimbene */
/* ==============================================================================
 * FILE: main.c
 * SCOPO: Gestione CLI e orchestrazione dei sottomoduli.
 * Strutture: Max-Heap (Code), Hash Table (Tecnici), BST (Storico).
 * ============================================================================== */
#include "condominio_tipi.h"
#include "richieste.h"
#include "tecnici.h"
#include "pianificazione.h"
#include "storico.h"

/*
 * ============================================================================
 * void inizializzaSistema(SistemaCondominio* sys);
 * Input: sys (contesto). Output: nessuno.
 * Descrizione: Configura lo stato iniziale vuoto di tutte le strutture dati.
 * Pre/Post: 'sys' != NULL -> heap_size=0, contatore=1, radici e bucket a NULL.
 * ============================================================================
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
 * ============================================================================
 * void liberaSistema(SistemaCondominio* sys);
 * Input: sys (contesto). Output: nessuno.
 * Descrizione: Dealloca ricorsivamente tutta la memoria dinamica del sistema.
 * Pre/Post: 'sys' inizializzato -> tutta la memoria di Heap, BST e Hash viene liberata.
 * ============================================================================
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
            
            // Necessario liberare l'agenda prima del tecnico per preservare i riferimenti ai nodi
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
    printf("[SISTEMA] Garbage Collection manuale terminata. Memoria liberata.\n");
}

/*
 * ============================================================================
 * int main();
 * Descrizione: Entry point che coordina l'esecuzione del menu CLI interattivo.
 * Postcondizioni: Ritorna 0 in caso di terminazione corretta.
 * ============================================================================
 */
int main() {
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

        switch(scelta) {
            case 1: {
                char app[50], tipo[50], desc[255];
                int u, st; Data d;

                // Lo spazio iniziale prima del format consuma eventuali residui di newline (\n) nel buffer
                printf("Appartamento: "); scanf(" %[^\n]", app);
                printf("Tipologia: "); scanf(" %[^\n]", tipo);
                printf("Descrizione: "); scanf(" %[^\n]", desc);
                printf("Data (GG MM AAAA): "); scanf("%d %d %d", &d.giorno, &d.mese, &d.anno);
                printf("Urgenza (0=BASSA, 1=MEDIA, 2=ALTA, 3=CRITICA): "); scanf("%d", &u);
                printf("Stato (0=APERTA, 1=PIANIF., 2=LAVORAZ.): "); scanf("%d", &st);

                Richiesta* nuova = (Richiesta*)malloc(sizeof(Richiesta));
                if (!nuova) { return 1; }
                
                nuova->id_richiesta = sistema.contatore_richieste++; 
                strcpy(nuova->appartamento, app);
                strcpy(nuova->tipologia, tipo);
                strcpy(nuova->descrizione, desc);
                nuova->data_richiesta = d;
                nuova->urgenza = (Urgenza)u;     
                nuova->urgenza_val = u;          
                nuova->stato = (StatoRichiesta)st;
                nuova->tecnico_assegnato = NULL; 

                inserisciRichiestaHeap(&sistema, nuova);
                break;
            }
            case 2: {
                char nome[100], spec[50]; int id, disp;

                printf("ID Tecnico: "); scanf("%d", &id);
                printf("Nome: "); scanf(" %[^\n]", nome);
                printf("Specializzazione: "); scanf(" %[^\n]", spec);
                printf("Disponibile (1=Si, 0=No): "); scanf("%d", &disp);

                registraTecnicoHash(&sistema, id, nome, spec, disp == 1);
                break;
            }
            case 3: {
                printf("\n=== RICHIESTE ATTIVE (Heap Array) ===\n");
                if (sistema.heap_size == 0) {
                     printf("Nessuna richiesta in coda.\n");
                } else {
                    for(int i = 0; i < sistema.heap_size; i++) {
                        Richiesta* r = sistema.heap_richieste[i];
                        const char* nome_tec = (r->tecnico_assegnato) ? r->tecnico_assegnato->nome : "Nessuno";
                        printf("[%d] ID: %d | Urgenza: %s | Tipo: %s | Tec: %s\n",
                            i, r->id_richiesta, strUrg(r->urgenza), r->tipologia, nome_tec);
                    }
                }
                break;
            }
            case 4: {
                printf("\n=== STORICO INTERVENTI (BST In-Order Traversal) ===\n");
                if (sistema.bst_storico_root == NULL) {
                    printf("Lo storico e' vuoto.\n");
                } else {
                    stampaBST(sistema.bst_storico_root);
                }
                break;
            }
            case 5: {
                int id_r, id_t, oi, of; Data d;
                printf("ID Richiesta: "); scanf("%d", &id_r);
                printf("ID Tecnico: "); scanf("%d", &id_t);
                printf("Data (GG MM AAAA): "); scanf("%d %d %d", &d.giorno, &d.mese, &d.anno);
                printf("Fascia (es. 900 1100): "); scanf("%d %d", &oi, &of);

                FasciaOraria f = {oi, of};
                pianificaIntervento(&sistema, id_r, id_t, d, f);
                break;
            }
            case 6: {
                int id_r, st;
                printf("ID Richiesta da modificare: "); scanf("%d", &id_r);
                printf("Nuovo stato (1=PIANIF, 2=IN_LAV, 3=CONCLUSA, 4=ANNULLATA): "); scanf("%d", &st);

                Richiesta* r = cercaRichiestaInHeap(&sistema, id_r);
                if (r) {
                    r->stato = (StatoRichiesta)st;
                    printf("[OK] Stato aggiornato.\n");

                    // Spostamento transazionale: i nodi chiusi lasciano la coda prioritaria ed entrano nell'archivio storico
                    if (st == 3 || st == 4) {
                        archiviaNelloStorico(&sistema, r);      
                        rimuoviRichiestaDaHeap(&sistema, id_r); 
                        printf("[INFO] Elemento estratto dalla Priority Queue e archiviato nel BST.\n");
                    }
                } else {
                    printf("[ERRORE] Richiesta non trouvata tra le attive.\n");
                }
                break;
            }
            case 7: {
                int id_cerca;
                printf("ID Richiesta da cercare: "); scanf("%d", &id_cerca);

                Richiesta* r_st = cercaStoricoBST(sistema.bst_storico_root, id_cerca);
                if (r_st) {
                    printf("[BST STORICO] App: %s | Stato: %s\n", r_st->appartamento, strStato(r_st->stato));
                } else {
                    Richiesta* r_att = cercaRichiestaInHeap(&sistema, id_cerca);
                    if (r_att) {
                        printf("[HEAP ATTIVE] App: %s | Urgenza: %s\n", r_att->appartamento, strUrg(r_att->urgenza));
                    } else {
                        printf("Nessuna corrispondenza trovata in nessun DB.\n");
                    }
                }
                break;
            }
            case 8: {
                printf("\n=== CARICO LAVORO (Scansione Hash Table) ===\n");
                for (int i = 0; i < HASH_SIZE; i++) {
                    Tecnico* t = sistema.hash_tecnici[i];
                    while (t != NULL) {
                        int count = 0;
                        InterventoPianificato* ag = t->agenda;
                        while(ag) { count++; ag = ag->next; }
                        
                        printf("- %s (#%d) | Interventi: %d | Residente al Bucket (Hash Index): %d\n",
                               t->nome, t->id_tecnico, count, i);
                        t = t->next;
                    }
                }
                break;
            }
            case 9: {
                int attive = sistema.heap_size; 
                int storiche = contaNodiBST(sistema.bst_storico_root); 
                
                printf("\n>>> REPORT GLOBALE <<<\n");
                printf("- Interventi attivi (Heap): %d\n", attive);
                printf("- Interventi archiviati (BST): %d\n", storiche);
                break;
            }
            case 0:
                printf("Chiusura in corso. Invocazione Memory Cleaner...\n");
                liberaSistema(&sistema); 
                break;
            default:
                printf("Scelta errata. Inserire numero valido.\n");
        }
    }
    return 0; 
}
