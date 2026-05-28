#ifndef TECNICI_H
#define TECNICI_H

#include "condominio_tipi.h"

/*
 * Funzione: hashFunction
 * Calcola l'indice di hash per un dato ID tecnico utilizzando l'operatore modulo.
 * * Parametri:
 * id: identificativo numerico intero del tecnico.
 * * Pre-condizione:
 * id deve essere un valore intero positivo. HASH_SIZE deve essere definita.
 * * Post-condizione:
 * Ritorna un valore intero >= 0 e strettamente minore di HASH_SIZE.
 * * Ritorna:
 * L'indice calcolato in cui posizionare o cercare il tecnico.
 */
int hashFunction(int id);

/*
 * Funzione: registraTecnicoHash
 * Crea una nuova istanza di un tecnico e la inserisce nella hash table.
 * * Parametri:
 * sys: puntatore al sistema condominio che contiene la hash table.
 * id: identificativo univoco assegnato al tecnico.
 * nome: stringa con il nome del tecnico.
 * spec: stringa che definisce la specializzazione del tecnico.
 * disp: booleano che indica l'attuale disponibilità lavorativa.
 * * Pre-condizione:
 * sys deve essere un puntatore valido e inizializzato. L'id non deve 
 * essere già presente per evitare duplicati logici.
 * * Post-condizione:
 * Viene allocata nuova memoria per il tecnico, che viene inserito 
 * nell'indice appropriato della hash table (con gestione collisioni).
 * * Ritorna:
 * Nessun valore (void).
 */
void registraTecnicoHash(SistemaCondominio* sys, int id, const char* nome, const char* spec, bool disp);

/*
 * Funzione: cercaTecnicoIdHash
 * Cerca un tecnico specifico all'interno della hash table tramite il suo ID.
 * * Parametri:
 * sys: puntatore al sistema condominio che contiene i tecnici registrati.
 * id: l'identificativo numerico del tecnico da cercare.
 * * Pre-condizione:
 * sys deve essere un puntatore valido e la hash table inizializzata.
 * * Post-condizione:
 * Lo stato del sistema e della hash table rimane inalterato.
 * * Ritorna:
 * Il puntatore alla struttura Tecnico se trovato, altrimenti restituisce NULL.
 */
Tecnico* cercaTecnicoIdHash(SistemaCondominio* sys, int id);

#endif
