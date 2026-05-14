//
// Created by alice flamigni on 04/03/26.
//

#include "Tempo.h"
#include <chrono>
#include <ncurses.h>

Tempo::Tempo() {
    tempoRimanente = 1000; // il tempo parte da 1000
    ultimoAggiornamento = std::chrono::steady_clock::now(); // inizializzo ultimoAggiornamento che memorizza l'istante
                                    // in cui è stato chiamato il costruttore nel main.
                                    // prende la variabile vuota e ci mette l'orario attuale (now())
}

void Tempo::aggiorna() { // chiamato continuamente nel ciclo while del main
    if (tempoRimanente > 0) { // se il tempo è ancora piu di 0
        std::chrono::steady_clock::time_point oraAttuale = std::chrono::steady_clock::now(); // crea e inizializza una nuova variabile sull'orario attuale;
                                        // registra l'istante corrente per poterlo sottrarre a 'ultimoAggiornamento' e calcolare il tempo trascorso

        auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(oraAttuale - ultimoAggiornamento).count();
        //prende orarioAttuale e gli sottrae l'istante di partenza in modo da avere un delta-tempo; duration_cast<std::chrono::seconds> casta il delta-tempo da nanosecondi a secondi
        // count() : funzione che "tira fuori" il numero dal calcolo del delta-tempo precedente: senza questa funzione
        // secondiTrascorsi non si potrebbe confrontare con un numero, siccome si tratta in se solo di un oggetto e non di un numero
        //auto: stabilisce automaticamente il tipo di variabile senza che me lo debba ricordare


        // se è passato almeno 1 secondo dall'ultimo aggiornamento
        if (secondiTrascorsi >= 1) { // se è passato almeno un secondo da quando ho chiamato il costruttore
                                                  //(cioe da quando ho inizializzato la variabile ultimoAggiornamento)
            tempoRimanente--; // diminuisco il tempo di 1 (faccio passare un secondo)
            ultimoAggiornamento = oraAttuale; // resetto il cronometro per iniziare a contare il prossimo secondo
            // se non azzerassi il conto (se non ripartissi da 0 ogni volta) sarebbero passati sempre >=1 sec dall'inizio,
            //quindi la condizione serebbe sempre vera. non è quello che voglio: devo aggiornare per verificare che sia passato un
            // secondo dall'ultima volta che ho contato e non dall'inizio
        }
    }
}

void Tempo::fineLivello() {
    tempoRimanente += 100;
    // il limite massimo torna a essere 1000
    if (tempoRimanente > 1000) {
        tempoRimanente = 1000;
    }
}

int Tempo::convertiInPunti() {
    int punti = tempoRimanente; // conversione 1:1
    tempoRimanente = 0;         // azzera il tempo una volta convertito
    return punti;
}

int Tempo::getTempo() {
    return tempoRimanente;
}

bool Tempo::isScaduto() {
    if (tempoRimanente<=0) return true;
    else return false;
}

void Tempo::disegna(WINDOW* win, int y, int x) { // prende coordinate della scritta time
    // Si aspetta che nel main siano stati definiti i COLOR_PAIR 1 e 2

    if (tempoRimanente > 100) { // se non è ancora nella situazione di "pericolo" (poco tempo)
        wattron(win, COLOR_PAIR(1)); // attiva nero su bianco (attron: accendi attributo, dice di attivare coppia di colori
        // numero 1 ==> vedi main)

        mvwprintw(win,y, x, " TIME: %03d ", tempoRimanente);
       wattroff(win, COLOR_PAIR(1)); // disattiva nero su bianco (molto imp disattivare attributo)

    }
    else { // se invece manca poco tempo (<100)
        wattron(win, COLOR_PAIR(2)); // attiva Rosso su Bianco
        mvwprintw(win, y, x, " TIME: %03d ", tempoRimanente);
        wattroff(win, COLOR_PAIR(2)); // disattiva Rosso su Bianco

    }
}
/* ---------------------- LEGENDA --------------------------------------
 *  % [rimepimento] [larghezza] [tipo] : schema per quando voglio disegnare tempo/punti/ecc
 *  con una buona formattazione.
 *  % : avvisa il programma che sta per arrivare un codice particolare e non va stampato un testo normale
 *  0 : carattere di riempimento.Se il numero è più piccolo della scatola, riempie i "buchi" vuoti con degli zeri (invece che con degli spazi invisibili). (poteva anche essere un altro carattere)
 *  3 : dice quanto è grande la scatola: 3 sta per "prenota 3 spazi, in cui andremo a mettere i numeri"; finche i numeri non hanno ancora 3 cifre, prima avremo degli 0
 *  d : tipo di dato (decimal); poteva essere f (float), c(char). decimal indica il tipo int
 */
