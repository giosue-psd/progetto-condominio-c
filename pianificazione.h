/* * Autore: Jacopo Salimbene
 * File: pianificazione.h
 * Descrizione: Interfaccia per la gestione e la pianificazione degli 
 * interventi di manutenzione all'interno del condominio.
 */

#ifndef PIANIFICAZIONE_H
#define PIANIFICAZIONE_H // Direttiva di inclusione condizionale (Inclusion Guard) per evitare inclusioni multiple del file header

/* --- Inclusioni delle dipendenze del modulo --- */
#include "condominio_tipi.h"  // Contiene la definizione della struttura principale 'SistemaCondominio'
#include "richieste.h"         // Contiene le definizioni e i tipi relativi alle richieste di intervento
#include "tecnici.h"           // Contiene le definizioni e i tipi relativi all'anagrafica dei tecnici

/**
 * @brief Pianifica un intervento di manutenzione nel sistema assegnando un tecnico e una data.
 * * @param sys          Puntatore alla struttura principale del sistema condominio (modificata internamente).
 * @param id_richiesta Identificativo univoco della richiesta di intervento da pianificare.
 * @param id_tecnico   Identificativo univoco del tecnico a cui assegnare l'intervento.
 * @param d            Struttura contenente la data stabilita per l'intervento (passata per valore).
 * @param fascia       Fascia oraria della giornata scelta per l'intervento (es. Mattina/Pomeriggio).
 * * @return true        Se la pianificazione è andata a buon fine (richiesta e tecnico validi, nessuna sovrapposizione).
 * @return false       Se si sono verificati errori o conflitti di disponibilità (es. tecnico già occupato).
 */
bool pianificaIntervento(SistemaCondominio* sys, int id_richiesta, int id_tecnico, Data d, FasciaOraria fascia);

#endif // PIANIFICAZIONE_H Fine della guardia di inclusione
