#ifndef TASTIERA_HPP
#define TASTIERA_HPP

#include <curses.h>

// Costanti globali della tastiera visiva
constexpr int t_n_char = 39;
constexpr int t_n_colonne = 13;
constexpr int t_x_start = 5;
constexpr int t_y_start = 2;

// L'array della tastiera definito come inline per evitare errori del linker
constexpr char tastiera[39] = {
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    'b','a','c','k',' ',' ',' ',' ','s','t','a','r','t'
};

// Prototipi delle funzioni
void stampa_tastiera();
bool t_gestione_nome(int tasto_premuto, char carattere_sel, char nome_player[], int &indice_nome, int indice_corrente);
void t_gestione_cur(int &indice_corrente, int tasto_premuto);
void colora_back(int start_y, int start_x);
void colora_start(int start_y, int start_x);
void t_colora(int indice_corrente, int start_y, int start_x);
void inserisci_nome(char nome_player[4]);

#endif // TASTIERA_HPP