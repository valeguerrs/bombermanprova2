#include "menu.hpp"
#include "game.hpp"
#include "tastiera.hpp"
#include "classifica.hpp"
#include "costanti.hpp" // Aggiunto per MY_RED_TESTO
#include <iostream>

using namespace std;

void m_stampa(int m_indice) {
    for (int i = 0; i < 3; i++) {
        if (i == m_indice) {
            color_set(MY_RED_TESTO, NULL); // Funziona sullo schermo standard
            mvprintw(m_intervallo * (i + 1) + m_y_start, m_x_start, menu_op[i]);
            color_set(0, NULL); // Ripristina il colore normale
        } else {
            mvprintw(m_intervallo * (i + 1) + m_y_start, m_x_start, menu_op[i]);
        }
    }
}

int gestione_tasto_premuto(int m_indice, int tasto_premuto) {
    switch (tasto_premuto) {
        case KEY_UP:
            if (m_indice > 0) {
                m_indice--;
            }
            break;
        case KEY_DOWN:
            if (m_indice < 2) {
                m_indice++;
            }
            break;
        case ' ':
            if (m_indice == 0) {
                //gioca
                int punteggio = game();
                char nome [4];
                inserisci_nome(nome);
                erase();
                aggiorna_classifica(nome, punteggio);
            }
            if (m_indice == 1) {
                //classifica
                erase();
                NodoClassifica* classifica = leggiClassifica();
                stampaLista(classifica);
            }
            break;
    }
    return m_indice;
}

void menu() {
    int m_indice = 0;
    bool continua = true;
    while (continua) {
        //cancella
        erase();

        // stampa
        m_stampa(m_indice);

        // refresh
        refresh();

        // tasto premuto
        int tasto_premuto = getch();
        m_indice = gestione_tasto_premuto(m_indice, tasto_premuto);

        // continua
        if (m_indice == 2 && tasto_premuto == ' ') {
            continua = false;
        }
    }
}