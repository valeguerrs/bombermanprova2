#ifndef BOMBA_HPP
#define BOMBA_HPP

#include <chrono>
#include <curses.h>
#include "costanti.hpp"

class Bomba {
protected:
    int x, y, raggioScoppio;
    bool is_esplosa, finita, muri_distrutti;
    int raggioSu, raggioGiu, raggioDx, raggioSx;
    std::chrono::steady_clock::time_point tempoPiazzamento, tempoEsplosione;

public:
    Bomba(int startX, int startY, int raggioOra, char mappa[righe][colonne]);
    int getX(); int getY();
    int getRaggioSu(); int getRaggioGiu();
    int getRaggioDx(); int getRaggioSx();
    bool isEsplosa_funzione();
    bool isFinita();
    void esplodiMuri(char mappa[righe][colonne]);
    void disegnaBomba(WINDOW* win);
    void disegnaEsplosione(WINDOW* win);
};

#endif