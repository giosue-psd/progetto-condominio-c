#include "pianificazione.h"

/*
 * FUNZIONE: pianificaIntervento
 * COSA FA: Applica la logica di business. Prende un tecnico e una richiesta e li "lega" tra loro, 
 * ma solo se non ci sono conflitti di orario e se le competenze corrispondono.
 */
bool pianificaIntervento(SistemaCondominio* sys, int id_richiesta, int id_tecnico, Data d, FasciaOraria fascia) {
    
    // Ricerca super veloce! Hash in O(1) e ricerca nell'array limitato dell'Heap
    Richiesta* r = cercaRichiestaInHeap(sys, id_richiesta);
    Tecnico* t = cercaTecnicoIdHash(sys, id_tecnico);

    // Controlli di base
    if (!r) { printf("[ERRORE] Richiesta non trovata tra le attive.\n"); return false; }
    if (!t) { printf("[ERRORE] Tecnico non trovato.\n"); return false; }
    if (!t->disponibile) { printf("[ERRORE] Tecnico in ferie/malattia.\n"); return false; }

    // Controllo Specializzazione (strcmp ritorna 0 se le stringhe sono identiche)
    if (strcmp(r->tipologia, t->specializzazione) != 0) {
        printf("[ERRORE] Incompatibilita' specializzazione (Req: %s, Tec: %s)!\n", r->tipologia, t->specializzazione);
        return false;
    }

    // Ricerca conflitti di orario nella lista concatenata dell'agenda del tecnico
    InterventoPianificato* curr = t->agenda;
    while (curr != NULL) {
        // Se c'è già un appuntamento nello STESSO GIORNO
        if (curr->data_intervento.anno == d.anno &&
            curr->data_intervento.mese == d.mese &&
            curr->data_intervento.giorno == d.giorno) {

            // Algoritmo di sovrapposizione temporale di 2 segmenti: (StartA < EndB) && (StartB < EndA)
            if ((fascia.ora_inizio < curr->fascia.ora_fine) && (curr->fascia.ora_inizio < fascia.ora_fine)) {
                printf("[ERRORE] Conflitto orario: Il tecnico %s e' occupato in quella fascia.\n", t->nome);
                return false; // Interrompo subito
            }
        }
        curr = curr->next;
    }

    // Nessun conflitto! Creo il nuovo elemento per l'agenda
    InterventoPianificato* nuovo = (InterventoPianificato*)malloc(sizeof(InterventoPianificato));
    nuovo->richiesta = r;
    nuovo->data_intervento = d;
    nuovo->fascia = fascia;
    
    // Inserisco il nuovo appuntamento in testa all'agenda del tecnico (O(1))
    nuovo->next = t->agenda;
    t->agenda = nuovo;

    // Aggiorno i flag nella richiesta
    r->stato = PIANIFICATA;
    r->tecnico_assegnato = t;

    printf("[OK] Intervento #%d assegnato a %s con successo.\n", id_richiesta, t->nome);
    return true;
}
