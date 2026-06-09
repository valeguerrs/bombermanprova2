#ifndef PERSONAGGIO_HPP
#define PERSONAGGIO_HPP

#include <curses.h>
#include "costanti.hpp"

class personaggio {
protected:
    int x, y;
    char character;
public:
    personaggio(int startX, int startY, char s);
    void muovi(int dx, int dy, char mappa[righe][colonne]);
    void disegna(WINDOW* win);
    int getX();
    int getY();
    void setPos(int newX, int newY);
};

#endif