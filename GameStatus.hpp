#ifndef GAMESTATUS_HPP
#define GAMESTATUS_HPP

#include <curses.h>

class GameStatus {
protected:
    int vite;
    int punti;

public:
    GameStatus();
    void perdiVita();
    int getVite();
    bool isGiocoFinito();
    void uccidiNemico(int puntiOttenuti);
    void convertiTempoInPunti(int secondiRimanenti);
    int getPunti();
    void aggiungiPuntiNemico();
    void aggiungiPuntiChaser();
    void aggiungiPuntiShooter();
    void aggiungiPuntiLivello(int livelloSuperato);
    void disegnaStatus(WINDOW* win, int y, int x);
};

#endif