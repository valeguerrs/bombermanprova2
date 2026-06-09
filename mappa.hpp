#ifndef MAPPA_HPP
#define MAPPA_HPP

#include <curses.h>
#include "costanti.hpp"

class Tempo;

class livello {
protected:
    char griglia[righe][colonne];
    int portaleX;
    int portaleY;

public:
    livello();
    livello(int n_muri);
    void creaMuri(int n);
    void scegliPortale();
    int getPortaleX();
    int getPortaleY();
    char (*getGriglia())[colonne];
    void disegna(WINDOW* win);
};

struct gestore_livelli {
    livello lv_attuale;
    gestore_livelli *precedente;
    gestore_livelli *successivo;
    Tempo* timerLivello;

    gestore_livelli();
    ~gestore_livelli();
};

#endif