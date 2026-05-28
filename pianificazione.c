/*scritto da Jacopo Salimbene */
#include "pianificazione.h"

/*
 * ============================================================================
 * SPECIFICA SINTATTICA
 * ============================================================================
 * bool pianificaIntervento(SistemaCondominio* sys, int id_richiesta, 
 * int id_tecnico, Data d, FasciaOraria fascia);
 *
 * Input: sys (contesto), id_richiesta/id_tecnico (ID entità), d/fascia (tempo).
 * Output: bool (esito dell'operazione).
 *
 * ============================================================================
 * SPECIFICA SEMANTICA
 * ============================================================================
 * Descrizione: Assegna una richiesta a un tecnico, inserendola in agenda.
 *
 * Precondizioni:
 * - Richiesta e tecnico esistenti e validi nel sistema.
 * - Tecnico disponibile e con specializzazione coincidente con la tipologia.
 * - Assenza di sovrapposizioni orarie nell'agenda del tecnico per la data 'd'.
 *
 * Postcondizioni:
 * - Successo: Allocazione 'InterventoPianificato', inserimento in 't->agenda',
 * stato richiesta 'PIANIFICATA', associazione al tecnico e ritorno true.
 * - Fallimento: Strutture dati invariate, log diagnostico e ritorno false.
 * ============================================================================
 */
bool pianificaIntervento(SistemaCondominio* sys, int id_richiesta, int id_tecnico, Data d, FasciaOraria fascia) {
    
    Richiesta* r = cercaRichiestaInHeap(sys, id_richiesta);
    Tecnico* t = cercaTecnicoIdHash(sys, id_tecnico);

    if (!r) { printf("[ERRORE] Richiesta non trovata tra le attive.\n"); return false; }
    if (!t) { printf("[ERRORE] Tecnico non trovato.\n"); return false; }
    if (!t->disponibile) { printf("[ERRORE] Tecnico in ferie/malattia.\n"); return false; }

    if (strcmp(r->tipologia, t->specializzazione) != 0) {
        printf("[ERRORE] Incompatibilita' specializzazione (Req: %s, Tec: %s)!\n", r->tipologia, t->specializzazione);
        return false;
    }

    InterventoPianificato* curr = t->agenda;
    while (curr != NULL) {
        if (curr->data_intervento.anno == d.anno &&
            curr->data_intervento.mese == d.mese &&
            curr->data_intervento.giorno == d.giorno) {

            // Intersezione temporale tra intervalli aperti: (InizioA < FineB) AND (InizioB < FineA)
            if ((fascia.ora_inizio < curr->fascia.ora_fine) && (curr->fascia.ora_inizio < fascia.ora_fine)) {
                printf("[ERRORE] Conflitto orario: Il tecnico %s e' occupato in quella fascia.\n", t->nome);
                return false; 
            }
        }
        curr = curr->next;
    }

    InterventoPianificato* nuovo = (InterventoPianificato*)malloc(sizeof(InterventoPianificato));
    if (!nuovo) { return false; } 
    
    nuovo->richiesta = r;
    nuovo->data_intervento = d;
    nuovo->fascia = fascia;
    
    // Inserimento in testa per garantire prestazioni in tempo costante O(1)
    nuovo->next = t->agenda;
    t->agenda = nuovo;

    r->stato = PIANIFICATA;
    r->tecnico_assegnato = t;

    printf("[OK] Intervento #%d assegnato a %s con successo.\n", id_richiesta, t->nome);
    return true;
}


