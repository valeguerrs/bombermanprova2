#include "Bomba.hpp"

Bomba::Bomba(int startX, int startY, int raggioOra, char mappa[righe][colonne]) {
    x = startX; y = startY; raggioScoppio = raggioOra;
    is_esplosa = false; finita = false; muri_distrutti = false;
    raggioSu = raggioGiu = raggioDx = raggioSx = 0;
    tempoPiazzamento = std::chrono::steady_clock::now();
    mappa[y][x] = '!';
}

int Bomba::getX() { return x; }
int Bomba::getY() { return y; }
int Bomba::getRaggioSu() { return raggioSu; }
int Bomba::getRaggioGiu() { return raggioGiu; }
int Bomba::getRaggioDx() { return raggioDx; }
int Bomba::getRaggioSx() { return raggioSx; }

bool Bomba::isEsplosa_funzione() {
    if (!is_esplosa && !finita) {
        auto orarioAttuale = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - tempoPiazzamento).count() >= 3) {
            is_esplosa = true; tempoEsplosione = std::chrono::steady_clock::now();
        }
    }
    return is_esplosa;
}

bool Bomba::isFinita() {
    if (is_esplosa && !finita) {
        auto orarioAttuale = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - tempoEsplosione).count() >= 1) finita = true;
    }
    return finita;
}

void Bomba::esplodiMuri(char mappa[righe][colonne]) {
    if (is_esplosa && !muri_distrutti) {
        mappa[y][x] = vuoto;
        for(int i = 1; i <= raggioScoppio; i++) { if(x + i < colonne) { if(mappa[y][x + i] == muro) break; raggioDx=i; if(mappa[y][x + i] == muro_distruttibile) { mappa[y][x + i] = vuoto; break; } } }
        for(int i = 1; i <= raggioScoppio; i++) { if(x - i >= 0) { if(mappa[y][x - i] == muro) break; raggioSx=i; if(mappa[y][x - i] == muro_distruttibile) { mappa[y][x - i] = vuoto; break; } } }
        for(int i = 1; i <= raggioScoppio; i++) { if(y + i < righe) { if(mappa[y + i][x] == muro) break; raggioGiu=i; if(mappa[y + i][x] == muro_distruttibile) { mappa[y + i][x] = vuoto; break; } } }
        for(int i = 1; i <= raggioScoppio - 1; i++) { if(y - i >= 0) { if(mappa[y - i][x] == muro) break; raggioSu=i; if(mappa[y - i][x] == muro_distruttibile) { mappa[y - i][x] = vuoto; break; } } }
        muri_distrutti = true;
    }
}

void Bomba::disegnaBomba(WINDOW* win) { if (!is_esplosa && !finita) mvwaddch(win, y, x, '!'); }

void Bomba::disegnaEsplosione(WINDOW* win) {
    if (is_esplosa && !finita) {
        mvwaddch(win, y, x, '*');
        for (int i = 1; i <= raggioSu; i++)  mvwaddch(win, y - i, x, '|');
        for (int i = 1; i <= raggioGiu; i++) mvwaddch(win, y + i, x, '|');
        for (int i = 1; i <= raggioSx; i++)  mvwaddch(win, y, x - i, '-');
        for (int i = 1; i <= raggioDx; i++)  mvwaddch(win, y, x + i, '-');
    }
}