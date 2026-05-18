#ifndef TECNICI_H
#define TECNICI_H

#include "condominio_tipi.h"

// Prototipi delle funzioni pubbliche
int hashFunction(int id);
void registraTecnicoHash(SistemaCondominio* sys, int id, const char* nome, const char* spec, bool disp);
Tecnico* cercaTecnicoIdHash(SistemaCondominio* sys, int id);

#endif
