#include "gamestatus.hpp"

GameStatus::GameStatus() { vite = 5; punti = 0; }
void GameStatus::perdiVita() { if (vite > 0) vite--; }
int GameStatus::getVite() { return vite; }
bool GameStatus::isGiocoFinito() { return vite <= 0; }
void GameStatus::uccidiNemico(int puntiOttenuti) { punti += puntiOttenuti; }
void GameStatus::convertiTempoInPunti(int secondiRimanenti) { if (secondiRimanenti > 0) punti += secondiRimanenti; }
int GameStatus::getPunti() { return punti; }
void GameStatus::aggiungiPuntiNemico() { uccidiNemico(25); }
void GameStatus::aggiungiPuntiChaser() { uccidiNemico(50); }
void GameStatus::aggiungiPuntiShooter() { uccidiNemico(100); }

void GameStatus::aggiungiPuntiLivello(int livelloSuperato) {
    if (livelloSuperato == 1) punti += 25;
    else if (livelloSuperato == 2) punti += 50;
    else if (livelloSuperato == 3) punti += 75;
    else if (livelloSuperato == 4) punti += 100;
    else if (livelloSuperato == 5) punti += 150;
}

void GameStatus::disegnaStatus(WINDOW* win, int y, int x) {
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, y, x, " VITE: %d | PUNTI: %04d ", vite, punti);
    wattroff(win, COLOR_PAIR(1));
}