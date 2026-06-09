#include "personaggio.hpp"

personaggio::personaggio(int startX, int startY, char s) { x = startX; y = startY; character = s; }

void personaggio::muovi(int dx, int dy, char mappa[righe][colonne]) {
    int nextX = x + dx;
    if (nextX >= 0 && nextX < colonne && mappa[y][nextX] == vuoto) x = nextX;
    int nextY = y + dy;
    if (nextY >= 0 && nextY < righe && mappa[nextY][x] == vuoto) y = nextY;
}

void personaggio::disegna(WINDOW* win) { mvwaddch(win, y, x, character); }
int personaggio::getX() { return x; }
int personaggio::getY() { return y; }
void personaggio::setPos(int newX, int newY) { x = newX; y = newY; }