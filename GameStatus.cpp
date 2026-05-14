//
// Created by alice flamigni on 04/03/26.
//

#include "GameStatus.h"
#include <ncurses.h>

GameStatus::GameStatus() { // costruttore che inizializza vite a 5 e punti a 0
    vite = 5;
    punti = 0;
}

void GameStatus::perdiVita() {
    if (vite > 0) {
        vite = vite - 1;
    }
}
int GameStatus::getVite() {
    return vite;
}

bool GameStatus::isGiocoFinito() {
    if (vite <= 0) {
        return true;
    }
    else {
        return false;
    }
}

void GameStatus::uccidiNemico(int puntiOttenuti) { // si potrebbe fare senza questa funzione, ma siccome avremo almeno due tipi di nemici conviene
                                                    // non fare come per i livelli ma usare una funzione ausiliaria in modo che quando avrò i tipi di nemici
                                                // potrò aggiungere diversi punti in base alla tipologia di nemico ucciso
    punti = punti + puntiOttenuti;
}

void GameStatus::convertiTempoInPunti (int secondiRimanenti) {
    if (secondiRimanenti > 0) {
        punti = punti + secondiRimanenti;
    }
}

int GameStatus::getPunti() {
    return punti;
}


void GameStatus::aggiungiPuntiNemico_BASE() {
    // richiama il metodo di base che aggiunge punti se uccidi un nemico passandogli 50 punti
    uccidiNemico(50);
}

void GameStatus::aggiungiPuntiLivello() {
    // usa la stessa logica di uccidiNemico per aggiungere i 40 punti del livello
    punti = punti + 40;
}

void GameStatus::disegnaStatus(WINDOW* win, int y, int x) {
    // Usa il COLOR_PAIR(1) che nel main è definito come nero su bianco
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, y, x, " VITE: %d | PUNTI: %04d ", vite, punti);
    wattroff(win, COLOR_PAIR(1));
}




