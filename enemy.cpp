#include "enemy.hpp"
#include <cstdlib>
#include <cmath>

enemy::enemy(int x, int y) : personaggio(x, y, 'O') {
    vivo = true;
    direzione = rand() % 4;
    ultimoMovimento = std::chrono::steady_clock::now();
}

void enemy::muovirandom(char mappa[righe][colonne], bool rallentato) {
    if (!vivo) return;

    // QUI NESSUN STATIC! Si crea la variabile del momento attuale
    auto adesso = std::chrono::steady_clock::now();
    int delay = rallentato ? 800 : 650;

    // Usa la variabile della classe (ultimoMovimento), non una locale!
    if (std::chrono::duration_cast<std::chrono::milliseconds>(adesso - ultimoMovimento).count() > delay) {

        int vecchio_x = x;
        int vecchio_y = y;

        switch (direzione) {
            case 0: muovi(1, 0, mappa); break;
            case 1: muovi(-1, 0, mappa); break;
            case 2: muovi(0, 1, mappa); break;
            case 3: muovi(0, -1, mappa); break;
        }

        if (x == vecchio_x && y == vecchio_y) {
            direzione = rand() % 4;
        }

        // Aggiorna il timer personale del nemico
        ultimoMovimento = adesso;
    }
}

void enemy::disegnanemico(WINDOW* win) { if (vivo) { disegna(win); } }

chaser::chaser(int x, int y) : enemy(x, y) {
    character = 'C';
    raggioAttivazione = 8;
    ultimoMovimento = std::chrono::steady_clock::now();
}

void chaser::aggiorna(char mappa[righe][colonne], int pX, int pY, punto scia[], bool lento) {
    if (!vivo) return;
    auto ora = std::chrono::steady_clock::now();
    int delay = lento ? 800 : 500;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(ora - ultimoMovimento).count() >= delay) {
        float dist = sqrt(pow(x - pX, 2) + pow(y - pY, 2));

        if (dist < raggioAttivazione) {
            punto target = scia[0];
            int dx = 0, dy = 0;
            if (target.x > x) dx = 1; else if (target.x < x) dx = -1;
            if (target.y > y) dy = 1; else if (target.y < y) dy = -1;
            muovi(dx, dy, mappa);
        } else {
            int vecchio_x = x;
            int vecchio_y = y;

            if (direzione == 0) muovi(1, 0, mappa);
            else if (direzione == 1) muovi(-1, 0, mappa);
            else if (direzione == 2) muovi(0, 1, mappa);
            else if (direzione == 3) muovi(0, -1, mappa);

            if (x == vecchio_x && y == vecchio_y) {
                direzione = rand() % 4;
            }
        }
        ultimoMovimento = ora;
    }
}

shooter::shooter(int x, int y) : enemy(x, y) {
    character = 'S';
    ultimoSparo = std::chrono::steady_clock::now();
    ultimoMovimento = std::chrono::steady_clock::now();
    inPausa = false;
}

void shooter::aggiorna(int pX, int pY, char mappa[righe][colonne], proiettile proiettili[], int maxP) {
    if (!vivo) return;
    auto ora = std::chrono::steady_clock::now();

    if (inPausa) {
        if (std::chrono::duration_cast<std::chrono::seconds>(ora - ultimoSparo).count() >= 2) inPausa = false;
    }

    if (!inPausa && std::chrono::duration_cast<std::chrono::seconds>(ora - ultimoSparo).count() >= 3) {
        if (x == pX || y == pY) {
            if (visioneLibera(pX, pY, mappa)) {
                for (int i = 0; i < maxP; i++) {
                    if (!proiettili[i].attivo) {
                        proiettili[i].x = x; proiettili[i].y = y;
                        proiettili[i].dx = (pX > x) ? 1 : (pX < x ? -1 : 0);
                        proiettili[i].dy = (pY > y) ? 1 : (pY < y ? -1 : 0);
                        proiettili[i].attivo = true;
                        ultimoSparo = ora;
                        inPausa = true;
                        break;
                    }
                }
            }
        }
    }

    if (!inPausa && std::chrono::duration_cast<std::chrono::milliseconds>(ora - ultimoMovimento).count() >= 1000) {
        muoviCasuale(mappa);
        ultimoMovimento = ora;
    }
}

bool shooter::visioneLibera(int pX, int pY, char mappa[righe][colonne]) {
    if (y == pY) {
        int start = (x < pX) ? x + 1 : pX + 1; int end = (x < pX) ? pX : x;
        for (int i = start; i < end; i++) { if (mappa[y][i] == muro || mappa[y][i] == muro_distruttibile) return false; }
    } else {
        int start = (y < pY) ? y + 1 : pY + 1; int end = (y < pY) ? pY : y;
        for (int i = start; i < end; i++) { if (mappa[i][x] == muro || mappa[i][x] == muro_distruttibile) return false; }
    }
    return true;
}

void shooter::muoviCasuale(char mappa[righe][colonne]) {
    int nx = x, ny = y;

    if (direzione == 0) ny--;
    else if (direzione == 1) ny++;
    else if (direzione == 2) nx++;
    else if (direzione == 3) nx--;

    if (nx >= 0 && nx < colonne && ny >= 0 && ny < righe && mappa[ny][nx] == vuoto) {
        x = nx;
        y = ny;
    } else {
        direzione = rand() % 4;
    }
}