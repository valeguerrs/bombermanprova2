//
// Created by alice flamigni on 08/03/26.
//

#include "Premio.h"
#include <chrono>
#include <ncurses.h>

Premio::Premio(int startX, int startY, int tipoPremio) { // costruttore
    x = startX; // assegna il parametro protected x al parametro del costruttore
    y = startY; // uguale a x ma per y
    tipo = tipoPremio; // uguale a x ma per tipo
    nascosto = true; // inizialmente il premio si trova sotto il muro distruttibile (inizializzo così)
    raccolto = false; // inizialmente il premio non è stato raccolto (inizializzo così)
    momentoRaccolta = std::chrono::steady_clock::time_point(); // inizializzo subito, altrimenti
                        // il sistema gli da un valore casuale (spazzatura) => inizializzo a 0
                        // mi assicuro che non ci sia un valore spazzatura
}

void Premio::scopri() { // funzione che scopre il premio una volta che viene distrutto il muro distruttibile sopra di lui
    nascosto = false; // il premio non è più nascosto
}

void Premio::raccogli() { // raccogliere il premio
    if (!nascosto && !raccolto) { // se il premio non è piu nascosto e non è stato ancora raccolto
        raccolto = true; // lo raccolgo
        momentoRaccolta = std::chrono::steady_clock::now(); // memorizza il momento esatto in cui il premio è stato raccolto (cosi successivamente
                                    // mi potrò chiedere se sono passati 10 secondi da quando ho raccolto il premio, in modo tale da far terminare l'effetto)
                                    // inizializzo for real
    }
}

bool Premio::isNascosto() { //true se il premio è ancora nascosto
    return nascosto;
}

bool Premio::isRaccolto() { // true se il premio è stato raccolto (coordinate del giocatore = coordinate premio)
    return raccolto;
}

bool Premio::isEffettoFinito() {
    if (raccolto) { // se ho raccolto il premio
        std::chrono::steady_clock::time_point orarioAttuale = std::chrono::steady_clock::now(); // memorizzo l'istante (adesso)

        auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - momentoRaccolta).count(); // guarda commenti tempo.cpp/ bomba.cpp
        if ((secondiTrascorsi) >= 10) { // se sono passati 10 secondi da quando ho raccolto il premio
            return true; // torno true
        }
    }
    return false; // altrimenti (non sono passati almeno 10 secondi) torno false e il premio continua ad avere il suo effetto
}

int Premio::getX() { return x; } // coordinata x premio
int Premio::getY() { return y; } // coordinata y premio
int Premio::getTipo() { return tipo; } // tipologia del premio


void Premio::disegnaPremio(WINDOW* win) { // funzione void che disegna il premio

    if (!nascosto && !raccolto) { // se il premio non è pouù nascosto e non l'ho ancora raccolto
        // stampa il simbolo corretto in base al tipo di premio -> grazie al tipo ENUM il nome del premio è assegnato ad un numero che il programma
        // legge cosi da poter disegnare il premio giusto. analizza la variabile di tipo intero "tipo", che puo essere i diversi tipi di premio;
        //in base a quella disegna il premio corretto -> grazie al tipo ENUM il nome del premio è assegnato ad un numero che il programma
        // legge cosi da poter disegnare il premio giusto. analizza la variabile di tipo intero "tipo", che puo essere i diversi tipi di premio;
        //in base a quella disegna il premio corretto
        if (tipo == AUMENTA_RAGGIO) {
            mvwprintw(win, y, x, "r");
        } else if (tipo == IMMUNITA) {
            mvwprintw(win, y, x, "$");
        } else if (tipo == DIMINUISCI_VELOCITA) {
            mvwprintw(win, y, x, "-");
        }
        else if (tipo == POSIZIONA_PIU_BOMBE) {
            mvwprintw(win, y,x,"+");
        }
    }
}





