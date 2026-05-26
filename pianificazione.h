/* scritto da Jacopo Salimbene */
#ifndef PIANIFICAZIONE_H
#define PIANIFICAZIONE_H

#include "condominio_tipi.h"
#include "richieste.h"
#include "tecnici.h"
/*prototipo */
bool pianificaIntervento(SistemaCondominio* sys, int id_richiesta, int id_tecnico, Data d, FasciaOraria fascia);

#endif
