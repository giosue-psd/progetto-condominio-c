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

        switch(scelta) {
            case 1: {
                // Buffer temporanei per l'input da tastiera
                char app[50], tipo[50], desc[255];
                int u, st; Data d;

                // Spiegazione di " %[^\n]": La scansione salta gli spazi iniziali rimasti nel buffer (il \n precedente)
                // e legge un'intera frase finché non premiamo invio (cattura anche gli spazi interni come "Elettricista Urgente").
                printf("Appartamento: "); scanf(" %[^\n]", app);
                printf("Tipologia: "); scanf(" %[^\n]", tipo);
                printf("Descrizione: "); scanf(" %[^\n]", desc);
                printf("Data (GG MM AAAA): "); scanf("%d %d %d", &d.giorno, &d.mese, &d.anno);
                printf("Urgenza (0=BASSA, 1=MEDIA, 2=ALTA, 3=CRITICA): "); scanf("%d", &u);
                printf("Stato (0=APERTA, 1=PIANIF., 2=LAVORAZ.): "); scanf("%d", &st);

                // Allocazione struttura reale sulla RAM Dinamica (Heap del Sistema Operativo)
                Richiesta* nuova = (Richiesta*)malloc(sizeof(Richiesta));
                
                // Popolamento dei dati
                nuova->id_richiesta = sistema.contatore_richieste++; // Assegna e POI incrementa (suffisso)
                strcpy(nuova->appartamento, app);
                strcpy(nuova->tipologia, tipo);
                strcpy(nuova->descrizione, desc);
                nuova->data_richiesta = d;
                nuova->urgenza = (Urgenza)u;     // Casting esplicito da int a Enum
                nuova->urgenza_val = u;          // Salvo numerico per l'algoritmo matematico del Max-Heap
                nuova->stato = (StatoRichiesta)st;
                nuova->tecnico_assegnato = NULL; // Default

                // Invio alla struttura dati Heap
                inserisciRichiestaHeap(&sistema, nuova);
                break;
            }
            case 2: {
                char nome[100], spec[50]; int id, disp;

                printf("ID Tecnico: "); scanf("%d", &id);
                printf("Nome: "); scanf(" %[^\n]", nome);
                printf("Specializzazione: "); scanf(" %[^\n]", spec);
                printf("Disponibile (1=Si, 0=No): "); scanf("%d", &disp);

                // Passa "disp == 1" che restituisce true se l'utente ha digitato 1, altrimenti false
                registraTecnicoHash(&sistema, id, nome, spec, disp == 1);
                break;
            }
            case 3: {
                printf("\n=== RICHIESTE ATTIVE (Heap Array) ===\n");
                    if (sistema.heap_size == 0) {
                         printf("Nessuna richiesta in coda.\n");
                        }else{
                        // Stampa sequenziale l'array sottostante.
                        // NOTA: l'indice [0] contiene sempre il MAX assoluto. Gli altri seguono una struttura parzialmente ordinata.
                            for(int i = 0; i < sistema.heap_size; i++) {
                                Richiesta* r = sistema.heap_richieste[i];
                                // Operatore ternario di C: se tecnico è diverso da NULL stampane il nome, altrimenti scrivi "Nessuno"
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

                // Cerca nell'Heap
                Richiesta* r = cercaRichiestaInHeap(&sistema, id_r);
                if (r) {
                    r->stato = (StatoRichiesta)st;
                    printf("[OK] Stato aggiornato.\n");

                    // LOGICA DI ARCHIVIAZIONE TRANSAZIONALE
                    // Se l'intervento è finito (Stato 3 o 4), deve "morire" nell'Heap e "rinascere" nel BST Storico.
                    if (st == 3 || st == 4) {
                        archiviaNelloStorico(&sistema, r);      // 1. Aggiungo copia puntatore al BST
                        rimuoviRichiestaDaHeap(&sistema, id_r); // 2. Rimuovo puntatore dall'array Heap (senza fare la free)
                        printf("[INFO] Elemento estratto dalla Priority Queue e archiviato nel BST.\n");
                    }
                } else {
                    printf("[ERRORE] Richiesta non trovata tra le attive.\n");
                }
                break;
            }
            case 7: {
                int id_cerca;
                printf("ID Richiesta da cercare: "); scanf("%d", &id_cerca);

                // Provo prima a cercare nel BST dello Storico (Ricerca Binaria O(log N) -> Velocissima)
                Richiesta* r_st = cercaStoricoBST(sistema.bst_storico_root, id_cerca);
                if (r_st) {
                    printf("[BST STORICO] App: %s | Stato: %s\n", r_st->appartamento, strStato(r_st->stato));
                } else {
                    // Se non c'è, provo nell'Heap (Ricerca Lineare O(N) -> Più lenta)
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
                            // Dobbiamo scorrere fisicamente tutto l'array (tutti i 100 buckets)
                            for (int i = 0; i < HASH_SIZE; i++) {
                                Tecnico* t = sistema.hash_tecnici[i];
                                // E per ogni bucket, esplorare le eventuali collisioni incatenate
                                while (t != NULL) {
                                    int count = 0;
                                    InterventoPianificato* ag = t->agenda;
                                    // Conto quante volte scendo nella lista concatenata della sua agenda
                                    while(ag) { count++; ag = ag->next; }
                                    
                                    printf("- %s (#%d) | Interventi: %d | Residente al Bucket (Hash Index): %d\n",
                                           t->nome, t->id_tecnico, count, i);
                                    t = t->next;
                                }
                            }
                            break;
                        }
                        case 9: {
                            int attive = sistema.heap_size; // L'array sa già quanto è grande, tempo O(1)
                            int storiche = contaNodiBST(sistema.bst_storico_root); // Il BST non lo sa, va ricalcolato ricorsivamente, tempo O(N)
                            
                            printf("\n>>> REPORT GLOBALE <<<\n");
                            printf("- Interventi attivi (Heap): %d\n", attive);
                            printf("- Interventi archiviati (BST): %d\n", storiche);
                            break;
                        }



    }
    return 0; 
}
