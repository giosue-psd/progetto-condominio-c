#ifndef STORICO_H
#define STORICO_H

#include "condominio_tipi.h"
#include "richieste.h"

// Prototipi
NodoBST* inserisciBST(NodoBST* root, Richiesta* r);
void archiviaNelloStorico(SistemaCondominio* sys, Richiesta* r);
Richiesta* cercaStoricoBST(NodoBST* root, int id_target);
void stampaBST(NodoBST* root);
void liberaBST(NodoBST* root);
int contaNodiBST(NodoBST* root);

#endif
