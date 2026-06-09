#ifndef COSTANTI_HPP
#define COSTANTI_HPP
#include <curses.h>

const int righe = 20;
const int colonne = 40;
const char muro = 'X';
const char muro_distruttibile = '#';
const char vuoto = ' ';

#define MY_RED 1
#define MY_YELLOW 2
#define MY_GRAY 3
#define MY_GREEN 4
#define MY_BLUE 5
#define MY_RED_TESTO 6

struct punto { 
    int x, y; 
};

struct proiettile {
    int x, y;
    int dx, dy;
    bool attivo;
};

enum TipoPremio_enum {
    AUMENTA_RAGGIO = 0,
    IMMUNITA = 1,
    DIMINUISCI_VELOCITA = 2,
    POSIZIONA_PIU_BOMBE = 3
};

#endif