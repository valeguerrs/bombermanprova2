#include "Tempo.hpp"

Tempo::Tempo(int tIniziale) {
    tempoRimanente = tIniziale;
    ultimoAggiornamento = std::chrono::steady_clock::now();
}

void Tempo::riprendiTimer() { ultimoAggiornamento = std::chrono::steady_clock::now(); }
void Tempo::aggiungiTempo(int secondiAggiuntivi) { tempoRimanente += secondiAggiuntivi; }

void Tempo::aggiorna() {
    if (tempoRimanente > 0) {
        std::chrono::steady_clock::time_point oraAttuale = std::chrono::steady_clock::now();
        auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(oraAttuale - ultimoAggiornamento).count();
        if (secondiTrascorsi >= 1) {
            tempoRimanente--;
            ultimoAggiornamento = oraAttuale;
        }
    }
}

void Tempo::fineLivello() {}
int Tempo::convertiInPunti() { int punti = tempoRimanente; tempoRimanente = 0; return punti; }
int Tempo::getTempo() { return tempoRimanente; }
bool Tempo::isScaduto() { return tempoRimanente <= 0; }

void Tempo::disegna(WINDOW* win, int y, int x) {
    if (tempoRimanente > 100) {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y, x, " TIME: %03d ", tempoRimanente);
        wattroff(win, COLOR_PAIR(1));
    } else {
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, y, x, " TIME: %03d ", tempoRimanente);
        wattroff(win, COLOR_PAIR(2));
    }
}