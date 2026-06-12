#include "tastiera.hpp"
#include "costanti.hpp" // Aggiunto per MY_RED_TESTO
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;


void stampa_tastiera() {
    for (int i = 0; i < t_n_char; i++) {
        int riga = i / t_n_colonne;
        int col = i % t_n_colonne;
        mvaddch(t_y_start + riga, t_x_start + col, tastiera[i]);
    }
}

bool t_gestione_nome(int tasto_premuto, char carattere_sel, char nome_player[], int &indice_nome, int indice_corrente) {
    if (tasto_premuto == ' ') {
        // Se siamo sulla riga 2 (indice_corrente >= 26, ovvero BACK e START)
        if (indice_corrente >= 26) {
            int col = indice_corrente % t_n_colonne;

            // Caso START (da colonna 6 a 12)
            if (col >= 6) {
                return false; // Esce dal ciclo, nome confermato
            }

            // Caso BACK (da colonna 0 a 5)
            if (col <= 5) {
                if (indice_nome > 0) {
                    indice_nome -= 1;             // Torna indietro
                    nome_player[indice_nome] = '_'; // Ripristina il cursore visivo
                }
                else {
                    return false;
                }
                return true;
            }
        }

        // Se non siamo su tasti speciali, inseriamo il carattere
        if (indice_nome < 3) {
            nome_player[indice_nome] = carattere_sel;
            indice_nome += 1;
        }
    }
    return true;
}

void t_gestione_cur(int &indice_corrente, int tasto_premuto) {
    int max_righe = (t_n_char + t_n_colonne - 1) / t_n_colonne;
    int riga_attuale = indice_corrente / t_n_colonne;
    int col_attuale = indice_corrente % t_n_colonne;

    // Tasto SU
    if (tasto_premuto == KEY_UP) {
        if (riga_attuale > 0) {
            indice_corrente -= t_n_colonne;
        }
    }
    // Tasto GIÙ
    if (tasto_premuto == KEY_DOWN) {
        if (riga_attuale < max_righe - 1) {
            // Verifica se la nuova posizione esiste nell'array tastiera
            if (indice_corrente + t_n_colonne < t_n_char) {
                indice_corrente += t_n_colonne;
            }
        }
    }
    // Tasto SINISTRA
    if (tasto_premuto == KEY_LEFT) {
        if (col_attuale > 0) {
            if (riga_attuale < 2) {
                indice_corrente--;
            } else {
                // Salto logico da START a BACK
                if (col_attuale >= 6) {
                    indice_corrente = (riga_attuale * t_n_colonne) + 5;
                } else {
                    indice_corrente--;
                }
            }
        }
    }
    // Tasto DESTRA
    if (tasto_premuto == KEY_RIGHT) {
        if (col_attuale < t_n_colonne - 1) {
            if (riga_attuale < 2) {
                indice_corrente++;
            } else {
                // Salto logico da BACK a START
                if (col_attuale <= 5) {
                    indice_corrente = (riga_attuale * t_n_colonne) + 6;
                } else {
                    indice_corrente++;
                }
            }
        }
    }
}

void colora_back(int start_y, int start_x) {
    color_set(MY_RED_TESTO, NULL);
    for (int i = 0; i < 6; i++) {
        mvaddch(start_y + 2, start_x + i, tastiera[26 + i]);
    }
    color_set(0, NULL);
}

void colora_start(int start_y, int start_x) {
    color_set(MY_RED_TESTO, NULL);
    for (int i = 0; i < 7; i++) {
        mvaddch(start_y + 2, start_x + 6 + i, tastiera[32 + i]);
    }
    color_set(0, NULL);
}

void t_colora(int indice_corrente, int start_y, int start_x) {
    if (indice_corrente >= 26 && indice_corrente <= 31) {
        colora_back(start_y, start_x);
    }
    else if (indice_corrente >= 32 && indice_corrente <= 38) {
        colora_start(start_y, start_x);
    }
    else {
        color_set(MY_RED_TESTO, NULL);
        mvaddch(start_y + indice_corrente / t_n_colonne, start_x + indice_corrente % t_n_colonne, tastiera[indice_corrente]);
        color_set(0, NULL);
    }
}

void inserisci_nome(char nome_player[4]) {
    nome_player[0] = '_';
    nome_player[1] = '_';
    nome_player[2] = '_';
    nome_player[3] = '\0';
    int indice_player = 0;

    // Rimosso init_pair locale per non sovrascrivere quelli di main.cpp

    int indice_corrente = 0;

    bool continua = true;
    while (continua) {
        erase();

        stampa_tastiera();
        mvaddstr(t_y_start + 4, t_x_start, nome_player);

        char carattere_selezionato = tastiera[indice_corrente];

        t_colora(indice_corrente, t_y_start, t_x_start);

        refresh();

        int tasto_premuto = getch();
        t_gestione_cur(indice_corrente, tasto_premuto);
        continua = t_gestione_nome(tasto_premuto, carattere_selezionato, nome_player, indice_player, indice_corrente);
    }
}