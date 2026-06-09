#ifndef TEMPO_HPP
#define TEMPO_HPP

#include <chrono>
#include <curses.h>

class Tempo {
protected:
    int tempoRimanente;
    std::chrono::steady_clock::time_point ultimoAggiornamento;

public:
    Tempo(int tIniziale);
    void riprendiTimer();
    void aggiungiTempo(int secondiAggiuntivi);
    void aggiorna();
    void fineLivello();
    int convertiInPunti();
    int getTempo();
    bool isScaduto();
    void disegna(WINDOW* win, int y, int x);
};

#endif