//
// Created by alice flamigni on 04/03/26.
//

#include "Tempo.h"
#include <ctime>

Tempo::Tempo() {
    secondiRimanenti = 500;
    ultimoSecondo = time(NULL);
}

void Tempo::aggiorna() {
    if (secondiRimanenti>0) {
        time_t orarioAttuale = time(NULL);

        if (orarioAttuale - ultimoSecondo >=1) {
            secondiRimanenti = secondiRimanenti - 1;
            ultimoSecondo = orarioAttuale;
        }
    }
}

int Tempo::getSecondi() {
    return secondiRimanenti;
}

bool Tempo::isTempoScaduto() {
    if (secondiRimanenti <= 0) {
        return true;
    }
    else return false;
}

void Tempo::livelloCompletato() {
    secondiRimanenti = secondiRimanenti +50;

    if (secondiRimanenti >= 500) {
        secondiRimanenti = 500;
    }
}

