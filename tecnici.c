#include "tecnici.h"
//SCRITTO DA MANUEL VITOLO
/*
 * FUNZIONE: hashFunction
 * COSA FA: È l'algoritmo di hashing (molto basilare). Prende l'ID (es. 105) 
 * e usa l'operatore Modulo (%) per ricavare un indice valido nell'array (0-99).
 * Es. 105 % 100 = 5. Il tecnico finirà nel bucket (cella) numero 5.
 * * Parametri:
 * id: identificativo numerico intero del tecnico.
 * * Pre-condizione:
 * id deve essere un valore intero positivo. HASH_SIZE deve essere definita.
 * * Post-condizione:
 * Ritorna un valore intero >= 0 e strettamente minore di HASH_SIZE,
 * che rappresenta un indice valido per la hash table.
 * * Ritorna:
 * L'indice calcolato in cui posizionare o cercare il tecnico.
 */

int hashFunction(int id) {
    return id % HASH_SIZE;
}

/*
 * FUNZIONE: registraTecnicoHash
 * COSA FA: Crea il tecnico, calcola il suo indice hash e lo inserisce nella tabella.
 * COMPLESSITÀ: O(1) (Tempo costante)
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
 * Viene allocata nuova memoria per il tecnico. Il tecnico è inserito 
 * nell'indice appropriato calcolato dalla funzione hash. In caso di 
 * collisione, il tecnico viene inserito in testa alla lista concatenata 
 * per garantire un inserimento in tempo O(1).
 * * Ritorna:
 * Nessun valore (void).
 */

void registraTecnicoHash(SistemaCondominio* sys, int id, const char* nome, const char* spec, bool disp) {
    // 1. Calcolo dove deve finire questo tecnico
    int indice = hashFunction(id);
    
    // 2. Allocazione dinamica sullo heap di sistema
    Tecnico* nuovo = (Tecnico*)malloc(sizeof(Tecnico));
    
    // 3. Inizializzazione dei campi
    nuovo->id_tecnico = id;
    strcpy(nuovo->nome, nome); // Copio la stringa nel buffer struct
    strcpy(nuovo->specializzazione, spec);
    nuovo->disponibile = disp;
    nuovo->agenda = NULL; // L'agenda inizialmente è vuota
    
    // 4. Gestione delle collisioni (Chaining)
    // Se alla cella 'index' c'è già un tecnico, il nuovo tecnico punterà a quello vecchio.
    // Inseriamo sempre in testa alla lista concatenata presente nel bucket (è l'operazione più veloce).
    nuovo->next = sys->hash_tecnici[indice];
    sys->hash_tecnici[indice] = nuovo;
    
    printf("[OK] Tecnico %s (#%d) inserito in Hash Table (Indice %d).\n", nome, id, indice);
}

/*
 * FUNZIONE: cercaTecnicoIdHash
 * COSA FA: Trova un tecnico istantaneamente usando la funzione hash.
 * COMPLESSITÀ: O(1) nel caso medio. O(N) nel caso pessimo (se tutti gli ID hanno stesso hash).
 * * Parametri:
 * sys: puntatore al sistema condominio che contiene i tecnici registrati.
 * id: l'identificativo numerico del tecnico da cercare.
 * * Pre-condizione:
 * sys deve essere un puntatore valido e la hash table inizializzata.
 * * Post-condizione:
 * Lo stato del sistema e della hash table rimane inalterato.
 * * Ritorna:
 * Il puntatore alla struttura Tecnico se l'ID viene trovato all'interno 
 * del bucket corrispondente, altrimenti restituisce NULL.
 */

Tecnico* cercaTecnicoIdHash(SistemaCondominio* sys, int id) {
    // 1. Calcolo in quale bucket si trova
    int indice = hashFunction(id);
    
    // 2. Vado direttamente in quella cella dell'array
    Tecnico* curr = sys->hash_tecnici[indice];
    
    // 3. Scorro la lista concatenata presente nella cella (in caso di collisioni)
    while (curr != NULL) {
        if (curr->id_tecnico == id) {
            return curr; // Trovato esattamente il tecnico desiderato
        }
        curr = curr->next;
    }
    
    return NULL;
}

