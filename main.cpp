#include <iostream>
#include <curses.h>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include "menu.hpp"

#include "costanti.hpp"
#include "gamestatus.hpp"
#include "tempo.hpp"
#include "bomba.hpp"
#include "premio.hpp"
#include "personaggio.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "mappa.hpp"

using namespace std;

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Inizializzazione globale dei colori basata su costanti.hpp
    if (has_colors()) {
        start_color();
        init_pair(MY_RED, 1, 1);
        init_pair(MY_YELLOW, 3, 3);
        init_pair(MY_GRAY, 7, 7);
        init_pair(MY_GREEN, 2, 2);
        init_pair(MY_BLUE, 6, 6);
        init_pair(MY_RED_TESTO, 1, 0); // Testo rosso su sfondo nero
    }

    menu();

    endwin();
    return 0;
}