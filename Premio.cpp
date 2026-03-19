//
// Created by alice flamigni on 08/03/26.
//

#include "Premio.h"
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

Premio::Premio(int startX, int startY) {
    x = startX;
    y = startY;
    nascosto = true;
    raccolto = false;
    momentoRaccolta = 0;
}

void Premio::scopri() {
    nascosto = false;
}

void Premio::raccogli() {
    if (!nascosto && !raccolto) {
        raccolto = true;
        momentoRaccolta = time(NULL);
    }
}

bool Premio::isNascosto() {
    return nascosto;
}

bool Premio::isRaccolto() {
    return raccolto;
}

bool Premio::isEffettoFinito() {
    if (raccolto) {
        time_t orarioAttuale = time(NULL);
        if (orarioAttuale-momentoRaccolta >= 10) {
            return true;
        }
    }
    else return false;
}

int Premio::getX() {
    return x;
}

int Premio::getY() {
    return y;
}

void Premio::disegnaPremio() {

    if (!nascosto && !raccolto) { // se il premio non è stato raccolto e il muro
        mvprintw(y,x, "!"); // sotto cui stava è stato rotto, stampo un "!" per indicare il premio
    }
}

void Premio::cancella() {
    mvprintw(y,x," "); // cancello il premio stampando uno spazio al suo posto
}








