#ifndef MENU_HPP
#define MENU_HPP

#include <curses.h>

// Costanti globali del menu
const int m_x_start = 3;
const int m_y_start = 0;
const int m_intervallo = 1;

// inline evita l'errore di "multiple definition" se l'header viene incluso in più file .cpp
constexpr char *menu_op[3] = {"play", "rank", "exit"};

// Prototipi delle funzioni
void m_stampa(int m_indice);
int gestione_tasto_premuto(int m_indice, int tasto_premuto);
void menu();

#endif // MENU_HPP