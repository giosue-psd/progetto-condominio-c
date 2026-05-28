/* * Progetto: Sistema Gestione Condominio
 * Autore: Jacopo Salimbene
 *
 * Descrizione: Questo modulo espone l'interfaccia pubblica per fissare
 * gli interventi manutentivi. Isola la logica di assegnazione temporale
 * per centralizzare i controlli anti-sovrapposizione dei tecnici.
 */

#ifndef PIANIFICAZIONE_H
#define PIANIFICAZIONE_H

#include "condominio_tipi.h"
#include "richieste.h"
#include "tecnici.h"

/*
 * * Funzione: pianificaIntervento
 * * Scopo: Registra l'assegnazione di un tecnico a una specifica richiesta.
 * Inserisce l'evento nell'agenda globale per bloccare lo slot orario.
 *
 * * Parametri:
 * sys: Struttura centrale del condominio; memorizza il nuovo stato.
 * id_richiesta: Chiave della richiesta; deve essere in stato 'aperta'.
 * id_tecnico: Chiave del tecnico; deve possedere la qualifica richiesta.
 * d: Giorno concordato per l'esecuzione del lavoro.
 * fascia: Periodo del giorno destinato all'intervento.
 *
 * * Ritorno:
 * true: Assegnazione riuscita e slot orario validato con successo.
 * false: Conflitto di orario del tecnico o ID non trovati nel sistema.
 */
bool pianificaIntervento(SistemaCondominio* sys, int id_richiesta, int id_tecnico, Data d, FasciaOraria fascia);

#endif
