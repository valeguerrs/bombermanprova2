#ifndef PREMIO_HPP
#define PREMIO_HPP

#include <chrono>
#include <curses.h>
#include "costanti.hpp"

class Premio {
protected:
    int x, y, tipo;
    bool nascosto, raccolto;
    std::chrono::steady_clock::time_point momentoRaccolta;

public:
    Premio(int startX, int startY, int tipoPremio);
    void scopri();
    void raccogli();
    bool isNascosto();
    bool isRaccolto();
    bool isEffettoFinito();
    int getX();
    int getY();
    int getTipo();
    void disegnaPremio(WINDOW* win);
};

#endif