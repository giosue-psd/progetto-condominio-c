#include "storico.h"

/*
 * FUNZIONE: inserisciBST
 * COSA FA: Funzione ricorsiva. Naviga l'albero per trovare la posizione foglia corretta e ci aggancia il nuovo nodo.
 * REGOLE BST: I minori vanno a Sinistra (left), i maggiori vanno a Destra (right).
 * COMPLESSITÀ: O(log N)
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
 */
void archiviaNelloStorico(SistemaCondominio* sys, Richiesta* r) {
    sys->bst_storico_root = inserisciBST(sys->bst_storico_root, r);
}

/*
 * FUNZIONE: cercaStoricoBST
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
 */
void stampaBST(NodoBST* root) {
    if (root != NULL) {

        stampaBST(root->right); 
        
        const char* nome_tec = (root->richiesta->tecnico_assegnato) ? root->richiesta->tecnico_assegnato->nome : "Nessuno";
        

        printf("ID:%d|App:%s  | Tipo:%s|Stato:%s|Tec: %s\n",
               root->richiesta->id_richiesta, root->richiesta->appartamento, 
               root->richiesta->tipologia, strStato(root->richiesta->stato), nome_tec);
               
        stampaBST(root->left); 
    }
}

/*
 * FUNZIONE: liberaBST 
 */
void liberaBST(NodoBST* root) {
    if (root == NULL) return;
    
    liberaBST(root->left);  
    liberaBST(root->right); 
    
 
    free(root->richiesta);             
}

// Visita ricorsiva per contare tutti i nodi
int contaNodiBST(NodoBST* root) {
    if (root == NULL) return 0;
    
    return contaNodiBST(root->left) + contaNodiBST(root->right);
}