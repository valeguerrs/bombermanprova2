#include "mappa.hpp"
#include "tempo.hpp"
#include <cstdlib>



livello::livello() {
    portaleX = -1; portaleY = -1;
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) { griglia[i][j] = vuoto; }
    }
}

livello::livello(int n_muri) : livello() {
    for (int i = 0; i < righe; i++) { griglia[i][0] = muro; griglia[i][colonne - 1] = muro; }
    for (int j = 0; j < colonne; j++) { griglia[0][j] = muro; griglia[righe - 1][j] = muro; }
    for (int i = 1; i < righe - 1; i += 2) {
        for (int j = 4; j < colonne - 1; j += 5) { griglia[i][j] = muro; }
    }
    creaMuri(n_muri); scegliPortale();
}

void livello::creaMuri(int n) {
    int count = 0;
    while (count < n) {
        int y = rand() % righe; int x = rand() % colonne;
        if ((y == 1 && x == 1) || (y == 1 && x == 2) || (y == 2 && x == 1)) continue;
        if (griglia[y][x] == vuoto) { griglia[y][x] = muro_distruttibile; count++; }
    }
}

void livello::scegliPortale() {
    int contaDistruttibili = 0;
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) { if (griglia[i][j] == muro_distruttibile) contaDistruttibili++; }
    }
    if (contaDistruttibili == 0) return;
    int target = rand() % contaDistruttibili; int corrente = 0;
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            if (griglia[i][j] == muro_distruttibile) {
                if (corrente == target) { portaleX = j; portaleY = i; return; }
                corrente++;
            }
        }
    }
}

int livello::getPortaleX() { return portaleX; }
int livello::getPortaleY() { return portaleY; }
char (*livello::getGriglia())[colonne] { return griglia; }


void livello::disegna(WINDOW* win) {
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            char carattereDaDisegnare = griglia[i][j];

            if (carattereDaDisegnare == muro_distruttibile) {
                wcolor_set(win, MY_RED, NULL);
                // Il cast (unsigned char) impedisce al numero di diventare negativo
                mvwaddch(win, i, j, ' ');
                wcolor_set(win, 0, NULL);

            } else if (carattereDaDisegnare == muro) {
                wcolor_set(win, MY_GRAY, NULL);
                mvwaddch(win, i, j, ' ');
                wcolor_set(win, 0, NULL);
            } else {
                mvwaddch(win, i, j, carattereDaDisegnare);
            }
        }
    }

    if (portaleX != -1 && portaleY != -1 && griglia[portaleY][portaleX] == vuoto) {
        mvwprintw(win, portaleY, portaleX, "P");
    }
}

gestore_livelli::gestore_livelli() { precedente = NULL; successivo = NULL; timerLivello = NULL; }
gestore_livelli::~gestore_livelli() { if (timerLivello != NULL) delete timerLivello; }