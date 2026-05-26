#include "storico.h"
//SCRITTO DA MANUEL VITOLO
/*
 * FUNZIONE: inserisciBST
 * COSA FA: Funzione ricorsiva. Naviga l'albero per trovare la posizione foglia corretta e ci aggancia il nuovo nodo.
 * REGOLE BST: I minori vanno a Sinistra (left), i maggiori vanno a Destra (right).
 * COMPLESSITÀ: O(log N)
 * * Parametri:
 * root: puntatore alla radice o al nodo corrente dell'albero.
 * r: puntatore alla richiesta da inserire.
 * * Pre-condizione:
 * r deve essere un puntatore a una struttura Richiesta validamente allocata.
 * * Post-condizione:
 * Viene allocato un nuovo NodoBST come foglia nella posizione corretta.
 * * Ritorna:
 * Il puntatore alla radice dell'albero (o sotto-albero) aggiornato.
 */

NodoBST* inserisciBST(NodoBST* root, Richiesta* r) {
    if (root == NULL) {
        NodoBST* nuovo = (NodoBST*)malloc(sizeof(NodoBST));
        nuovo->richiesta = r; 
        nuovo->left = NULL;
        nuovo->right = NULL;
        return nuovo;
    }
    
    if (r->id_richiesta <= root->richiesta->id_richiesta) {
        root->left = inserisciBST(root->left, r);
    } else if (r->id_richiesta > root->richiesta->id_richiesta) {
        root->right = inserisciBST(root->right, r);
    }
    
    return root; 
}

/*
 * FUNZIONE: archiviaNelloStorico (Wrapper)
 * COSA FA: Funzione wrapper che astrae e semplifica l'inserimento di una richiesta 
 * nello storico del condominio, nascondendo i dettagli dell'implementazione BST.
 * * Parametri:
 * sys: puntatore al sistema principale del condominio.
 * r: puntatore alla richiesta completata da archiviare.
 * * Pre-condizione:
 * sys e r non devono essere NULL.
 * * Post-condizione:
 * La radice bst_storico_root all'interno di sys viene riassegnata per 
 * includere il nuovo inserimento.
 * * Ritorna:
 * Nessun valore.
 */

void archiviaNelloStorico(SistemaCondominio* sys, Richiesta* r) {
    sys->bst_storico_root = inserisciBST(sys->bst_storico_root, r);
}

/*
 * FUNZIONE: cercaStoricoBST
 * COSA FA: Cerca in modo ricorsivo un ID specifico all'interno dello storico. Sfrutta 
 * la struttura del BST per escludere metà dei nodi a ogni passo logico.
 * * Parametri:
 * root: puntatore alla radice o al nodo corrente dell'albero.
 * id_target: l'identificativo numerico univoco della richiesta da cercare.
 * * Pre-condizione:
 * L'albero deve rispettare le regole di ordinamento del Binary Search Tree.
 * * Post-condizione:
 * Lo stato dei nodi e dell'albero rimane completamente inalterato.
 * * Ritorna:
 * Il puntatore alla richiesta se trovata, altrimenti NULL.
 */
Richiesta* cercaStoricoBST(NodoBST* root, int id_target) {
    if (root == NULL || root->richiesta->id_richiesta == id_target) {
        return (root != NULL) ? root->richiesta : NULL;
    }
    
    if (id_target < root->richiesta->id_richiesta) {
        return cercaStoricoBST(root->left, id_target);
    }
    
    return cercaStoricoBST(root->right, id_target);
}

/*
 * FUNZIONE: stampaBST
 * COSA FA: Esplorare l'albero in questo modo garantisce che gli elementi vengano stampati
 * in perfetto ordine crescente di ID, in modo automatico.
 * * Parametri:
 * root: puntatore alla radice dell'albero da esplorare.
 * * Pre-condizione:
 * Nessuna. Se root è NULL, la funzione termina in modo sicuro senza errori.
 * * Post-condizione:
 * I dati vengono visualizzati sul flusso di output standard.
 * * Ritorna:
 * Nessun valore.
 */
void stampaBST(NodoBST* root) {
    if (root != NULL) {
        /* [RISOLUZIONE ERRORE]: Per stampare in ordine crescente bisogna visitare prima il 
         * sotto-albero sinistro, poi il nodo corrente, poi il destro.
         * Nel codice originale c'era root->right che generava un ordine decrescente.
        */
        stampaBST(root->left); 
        
        const char* nome_tec = (root->richiesta->tecnico_assegnato) ? root->richiesta->tecnico_assegnato->nome : "Nessuno";
        
        printf("ID:%d|App:%s|Tipo:%s|Stato:%s|Tec: %s\n", root->richiesta->id_richiesta, root->richiesta->appartamento, root->richiesta->tipologia, strStato(root->richiesta->stato), nome_tec);
               
        // [RISOLUZIONE ERRORE]: Ora visitiamo il ramo destro per i valori maggiori.
        stampaBST(root->right); 
    }
}

/*
 * FUNZIONE: liberaBST 
 * COSA FA: Attraversa l'albero in Post-Order (Sinistra, Destra, Nodo) per deallocare 
 * ricorsivamente l'intera struttura partendo dalle foglie verso la radice, 
 * evitando memory leak.
 * * Parametri:
 * root: puntatore alla radice dell'albero.
 * * Pre-condizione:
 * root deve puntare a memoria validamente allocata nell'heap.
 * * Post-condizione:
 * L'albero e le strutture "richiesta" al suo interno vengono completamente 
 * rimosse dalla memoria.
 * * Ritorna:
 * Nessun valore.
 */
void liberaBST(NodoBST* root) {
    if (root == NULL) return;
    
    liberaBST(root->left);  
    liberaBST(root->right); 
    
    free(root->richiesta);             
    
    /*[RISOLUZIONE ERRORE]: Mancava la liberazione del nodo stesso. Senza questa riga
     * verrebbe liberata solo la struttura Richiesta, ma i vari "NodoBST" 
     * causerebbero un memory leak (perdita di memoria).
    */
    free(root);
}

// Visita ricorsiva per contare tutti i nodi
int contaNodiBST(NodoBST* root) {
    if (root == NULL) return 0;
    
    /* [RISOLUZIONE ERRORE]: Bisogna aggiungere "1 +" per conteggiare anche il nodo corrente.
     * Nel codice originale venivano sommati solo i figli senza contare il nodo stesso, 
     * ritornando invariabilmente 0.
    */
    return 1 + contaNodiBST(root->left) + contaNodiBST(root->right);
}
