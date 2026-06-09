#include <iostream>
#include <curses.h>
#include <cstdlib>
#include <cmath>
#include <chrono>


#include "costanti.hpp"
#include "gamestatus.hpp"
#include "tempo.hpp"
#include "bomba.hpp"
#include "premio.hpp"
#include "personaggio.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "mappa.hpp"

using namespace std;

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(50);
    keypad(stdscr, TRUE);

    if (!has_colors()) {
        printw("No colors\n");
        getch();
        endwin();
        return 0;
    }



    unsigned int seme = std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now().time_since_epoch()
                        ).count();
    srand(seme);

    initscr();
    start_color();

    if (!has_colors()) {
        printw("No colors\n");
        getch();
        endwin();
        return 0;
    }

    start_color();   // 2. Accendi la gestione dei colori

    // 3. ECCO DOVE VANNO! Li definisci UNA SOLA VOLTA qui per tutto il gioco.
    init_pair(MY_RED, 1, 1);       // MY_RED era 1
    init_pair(MY_YELLOW, 3, 3);    // MY_YELLOW era 2
    init_pair(MY_GRAY, 7, 7);      // Adatta i numeri a quelli che preferisci
    init_pair(MY_GREEN, 2, 2);
    init_pair(MY_BLUE, 6, 6);
    init_pair(MY_RED_TESTO, 1,0);

    WINDOW* win = newwin(righe + 4, colonne, 0, 0);


    int numeroLivello = 1;
    gestore_livelli* lv = new gestore_livelli;
    lv->lv_attuale = livello(100);
    lv->timerLivello = new Tempo(102);

    char(*mappa)[colonne] = lv->lv_attuale.getGriglia();

    Premio* premi_livello[800];
    for (int i = 0; i < 800; i++) premi_livello[i] = NULL;
    int num_premi = 0;

    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            if (mappa[i][j] == muro_distruttibile) {
                if (rand() % 100 < 30) {
                    premi_livello[num_premi] = new Premio(j, i, rand() % 4);
                    num_premi++;
                }
            }
        }
    }

    player player1(1, 1);

    int ex, ey, cx, cy, sx, sy;
    do { ex = rand() % (colonne - 2) + 1; ey = rand() % (righe - 2) + 1; } while ((ex < 5 && ey < 5) || mappa[ey][ex] != vuoto);
    enemy enemy1(ex, ey);
    do { cx = rand() % (colonne - 2) + 1; cy = rand() % (righe - 2) + 1; } while ((cx < 5 && cy < 5) || mappa[cy][cx] != vuoto);
    chaser chaser1(cx, cy);
    do { sx = rand() % (colonne - 2) + 1; sy = rand() % (righe - 2) + 1; } while ((sx < 5 && sy < 5) || mappa[sy][sx] != vuoto);
    shooter shooter1(sx, sy);

    const int MAX_PROIETTILI = 10;
    proiettile proiettili[MAX_PROIETTILI];
    for (int i = 0; i < MAX_PROIETTILI; i++) proiettili[i].attivo = false;

    punto scia[1];
    GameStatus* statusPartita = new GameStatus();

    const int MAX_BOMBE = 5;
    Bomba* arrayBombe[MAX_BOMBE];
    for (int i = 0; i < MAX_BOMBE; i++) arrayBombe[i] = NULL;

    bool giocoInCorso = true;
    int navigaLivello = 0;

    auto tempoUltimoRespawn = std::chrono::steady_clock::now();
    bool invulnerabileRespawn = false;

    while (giocoInCorso) {
        werase(win);

        if (invulnerabileRespawn) {
            auto oraAttuale = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(oraAttuale - tempoUltimoRespawn).count() >= 3) {
                invulnerabileRespawn = false;
            }
        }

        if (lv->timerLivello != NULL) lv->timerLivello->aggiorna();

        if ((lv->timerLivello != NULL && lv->timerLivello->isScaduto()) || statusPartita->isGiocoFinito()) {
            giocoInCorso = false;
            continue;
        }

        lv->lv_attuale.disegna(win);

        bool power_raggio = false, power_immunita = false, power_lento = false, power_piubombe = false;
        for (int i = 0; i < num_premi; i++) {
            if (premi_livello[i] != NULL) {
                if (premi_livello[i]->isNascosto() && mappa[premi_livello[i]->getY()][premi_livello[i]->getX()] == vuoto) {
                    premi_livello[i]->scopri();
                }
                premi_livello[i]->disegnaPremio(win);
                if (!premi_livello[i]->isNascosto() && !premi_livello[i]->isRaccolto()) {
                    if (player1.getX() == premi_livello[i]->getX() && player1.getY() == premi_livello[i]->getY()) {
                        premi_livello[i]->raccogli();
                    }
                }
                if (premi_livello[i]->isRaccolto() && !premi_livello[i]->isEffettoFinito()) {
                    int t = premi_livello[i]->getTipo();
                    if (t == AUMENTA_RAGGIO) power_raggio = true;
                    else if (t == IMMUNITA) power_immunita = true;
                    else if (t == DIMINUISCI_VELOCITA) power_lento = true;
                    else if (t == POSIZIONA_PIU_BOMBE) power_piubombe = true;
                }
            }
        }

        bool nemico1_vicino = false, chaser1_vicino = false;
        for (int i = 0; i < MAX_BOMBE; i++) {
            if (arrayBombe[i] != NULL && !arrayBombe[i]->isEsplosa_funzione()) {
                if (sqrt(pow(enemy1.getX() - arrayBombe[i]->getX(), 2) + pow(enemy1.getY() - arrayBombe[i]->getY(), 2)) < 4.0) nemico1_vicino = true;
                if (sqrt(pow(chaser1.getX() - arrayBombe[i]->getX(), 2) + pow(chaser1.getY() - arrayBombe[i]->getY(), 2)) < 4.0) chaser1_vicino = true;
            }
        }

        for (int i = 0; i < MAX_BOMBE; i++) {
            if (arrayBombe[i] != NULL) {
                if (arrayBombe[i]->isEsplosa_funzione()) {
                    arrayBombe[i]->esplodiMuri(mappa);
                    if (!arrayBombe[i]->isFinita()) {
                        int bx = arrayBombe[i]->getX(), by = arrayBombe[i]->getY();
                        int rU = arrayBombe[i]->getRaggioSu(), rD = arrayBombe[i]->getRaggioGiu();
                        int rR = arrayBombe[i]->getRaggioDx(), rL = arrayBombe[i]->getRaggioSx();

                        int px = player1.getX(), py = player1.getY();
                        if (((px == bx && py >= by - rU && py <= by + rD) || (py == by && px >= bx - rL && px <= bx + rR))) {
                            if (!power_immunita && !invulnerabileRespawn) {
                                statusPartita->perdiVita(); player1.setPos(1, 1); invulnerabileRespawn = true;
                                tempoUltimoRespawn = std::chrono::steady_clock::now(); napms(200);
                            }
                        }

                        int nx = enemy1.getX(), ny = enemy1.getY();
                        if (enemy1.vivo && ((nx == bx && ny >= by - rU && ny <= by + rD) || (ny == by && nx >= bx - rL && nx <= bx + rR))) {
                            enemy1.vivo = false; statusPartita->aggiungiPuntiNemico();
                            if (lv->timerLivello) lv->timerLivello->aggiungiTempo(25);
                        }

                        nx = chaser1.getX(); ny = chaser1.getY();
                        if (chaser1.vivo && ((nx == bx && ny >= by - rU && ny <= by + rD) || (ny == by && nx >= bx - rL && nx <= bx + rR))) {
                            chaser1.vivo = false; statusPartita->aggiungiPuntiChaser();
                            if (lv->timerLivello) lv->timerLivello->aggiungiTempo(40);
                        }

                        nx = shooter1.getX(); ny = shooter1.getY();
                        if (shooter1.vivo && ((nx == bx && ny >= by - rU && ny <= by + rD) || (ny == by && nx >= bx - rL && nx <= bx + rR))) {
                            shooter1.vivo = false; statusPartita->aggiungiPuntiShooter();
                            if (lv->timerLivello) lv->timerLivello->aggiungiTempo(50);
                        }
                    }
                }
                if (arrayBombe[i]->isFinita()) { delete arrayBombe[i]; arrayBombe[i] = NULL; }
            }
        }

        scia[0] = {player1.getX(), player1.getY()};
        enemy1.muovirandom(mappa, power_lento || nemico1_vicino);
        chaser1.aggiorna(mappa, player1.getX(), player1.getY(), scia, power_lento || chaser1_vicino);
        shooter1.aggiorna(player1.getX(), player1.getY(), mappa, proiettili, MAX_PROIETTILI);

        static auto lastBulletUpdate = std::chrono::steady_clock::now();
        auto nowBullet = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(nowBullet - lastBulletUpdate).count() >= 210) {
            for (int i = 0; i < MAX_PROIETTILI; i++) {
                if (proiettili[i].attivo) {
                    proiettili[i].x += proiettili[i].dx;
                    proiettili[i].y += proiettili[i].dy;
                    if (mappa[proiettili[i].y][proiettili[i].x] != vuoto) proiettili[i].attivo = false;

                    if (proiettili[i].x == player1.getX() && proiettili[i].y == player1.getY()) {
                        if (!power_immunita && !invulnerabileRespawn) {
                            statusPartita->perdiVita(); player1.setPos(1, 1); invulnerabileRespawn = true;
                            tempoUltimoRespawn = std::chrono::steady_clock::now(); napms(200);
                        }
                        proiettili[i].attivo = false;
                    }
                }
            }
            lastBulletUpdate = nowBullet;
        }

        for (int i = 0; i < MAX_PROIETTILI; i++) {
            for (int i = 0; i < MAX_PROIETTILI; i++) {
                if (proiettili[i].attivo) {
                    // 1. Imposta il colore per la finestra (es. MY_RED o MY_GRAY)
                    wcolor_set(win, MY_RED_TESTO, NULL);

                    // 2. Stampa il carattere pulito senza operatori di mezzo
                    mvwaddch(win, proiettili[i].y, proiettili[i].x, '.');

                    // 3. Ripristina subito il colore neutro di default (0)
                    wcolor_set(win, 0, NULL);
                }
            }
            //if (proiettili[i].attivo) mvwaddch(win, proiettili[i].y, proiettili[i].x, '.' | COLOR_PAIR(MY_RED_TESTO));
        }

        if (!power_immunita && !invulnerabileRespawn) {
            bool presoFisicamente = false;
            if (enemy1.vivo && player1.getX() == enemy1.getX() && player1.getY() == enemy1.getY()) presoFisicamente = true;
            if (chaser1.vivo && player1.getX() == chaser1.getX() && player1.getY() == chaser1.getY()) presoFisicamente = true;
            if (shooter1.vivo && player1.getX() == shooter1.getX() && player1.getY() == shooter1.getY()) presoFisicamente = true;

            if (presoFisicamente) {
                statusPartita->perdiVita(); player1.setPos(1, 1); invulnerabileRespawn = true;
                tempoUltimoRespawn = std::chrono::steady_clock::now(); napms(200);
            }
        }

        bool eseguiCambioInAvanti = false, eseguiCambioIndietro = false, assegnaPunti = false;
        bool tuttiNemiciMorti = !enemy1.vivo && !chaser1.vivo && !shooter1.vivo;
        int portX = lv->lv_attuale.getPortaleX(), portY = lv->lv_attuale.getPortaleY();

        if (tuttiNemiciMorti && portX != -1 && portY != -1 && mappa[portY][portX] == vuoto) {
            if (player1.getX() == portX && player1.getY() == portY) { eseguiCambioInAvanti = true; assegnaPunti = true; }
        }

        if (navigaLivello == 1) { eseguiCambioInAvanti = true; assegnaPunti = false; navigaLivello = 0; }
        else if (navigaLivello == -1) { eseguiCambioIndietro = true; assegnaPunti = false; navigaLivello = 0; }

        if (eseguiCambioIndietro) {
            if (lv->precedente != NULL) {
                numeroLivello--; lv = lv->precedente; mappa = lv->lv_attuale.getGriglia(); player1.setPos(1, 1);
                for (int i = 0; i < MAX_BOMBE; i++) { if (arrayBombe[i]) { delete arrayBombe[i]; arrayBombe[i] = NULL; } }
                for (int i = 0; i < MAX_PROIETTILI; i++) proiettili[i].attivo = false;
                for (int i = 0; i < num_premi; i++) { if (premi_livello[i]) { delete premi_livello[i]; premi_livello[i] = NULL; } }
                num_premi = 0;
                for (int i = 0; i < righe; i++) {
                    for (int j = 0; j < colonne; j++) {
                        if (mappa[i][j] == muro_distruttibile) {
                            if (rand() % 100 < 30) { premi_livello[num_premi] = new Premio(j, i, rand() % 4); num_premi++; }
                        }
                    }
                }

                do { ex = rand() % (colonne - 2) + 1; ey = rand() % (righe - 2) + 1; } while ((ex < 5 && ey < 5) || mappa[ey][ex] != vuoto);
                enemy1.setPos(ex, ey); enemy1.vivo = true;
                do { cx = rand() % (colonne - 2) + 1; cy = rand() % (righe - 2) + 1; } while ((cx < 5 && cy < 5) || mappa[cy][cx] != vuoto);
                chaser1.setPos(cx, cy); chaser1.vivo = true;
                do { sx = rand() % (colonne - 2) + 1; sy = rand() % (righe - 2) + 1; } while ((sx < 5 && sy < 5) || mappa[sy][sx] != vuoto);
                shooter1.setPos(sx, sy); shooter1.vivo = true;

                invulnerabileRespawn = true; tempoUltimoRespawn = std::chrono::steady_clock::now();
                if (lv->timerLivello) lv->timerLivello->riprendiTimer();
                continue;
            }
        }

        if (eseguiCambioInAvanti) {
            if (assegnaPunti) { statusPartita->aggiungiPuntiLivello(numeroLivello); }
            if (numeroLivello < 5) {
                numeroLivello++;
                if (lv->successivo == NULL) {
                    gestore_livelli* nuovoNodo = new gestore_livelli;
                    nuovoNodo->lv_attuale = livello(100 - (numeroLivello * 10)); nuovoNodo->precedente = lv;
                    int tNuovo = 200;
                    if (numeroLivello == 2) tNuovo = 250; else if (numeroLivello == 3) tNuovo = 300; else if (numeroLivello == 4) tNuovo = 400; else if (numeroLivello == 5) tNuovo = 500;
                    nuovoNodo->timerLivello = new Tempo(tNuovo);
                    lv->successivo = nuovoNodo;
                }
                lv = lv->successivo; mappa = lv->lv_attuale.getGriglia(); player1.setPos(1, 1);
                for (int i = 0; i < MAX_BOMBE; i++) { if (arrayBombe[i]) { delete arrayBombe[i]; arrayBombe[i] = NULL; } }
                for (int i = 0; i < MAX_PROIETTILI; i++) proiettili[i].attivo = false;
                for (int i = 0; i < num_premi; i++) { if (premi_livello[i]) { delete premi_livello[i]; premi_livello[i] = NULL; } }
                num_premi = 0;
                for (int i = 0; i < righe; i++) {
                    for (int j = 0; j < colonne; j++) {
                        if (mappa[i][j] == muro_distruttibile) {
                            if (rand() % 100 < 30) { premi_livello[num_premi] = new Premio(j, i, rand() % 4); num_premi++; }
                        }
                    }
                }

                do { ex = rand() % (colonne - 2) + 1; ey = rand() % (righe - 2) + 1; } while ((ex < 5 && ey < 5) || mappa[ey][ex] != vuoto);
                enemy1.setPos(ex, ey); enemy1.vivo = true;
                do { cx = rand() % (colonne - 2) + 1; cy = rand() % (righe - 2) + 1; } while ((cx < 5 && cy < 5) || mappa[cy][cx] != vuoto);
                chaser1.setPos(cx, cy); chaser1.vivo = true;
                do { sx = rand() % (colonne - 2) + 1; sy = rand() % (righe - 2) + 1; } while ((sx < 5 && sy < 5) || mappa[sy][sx] != vuoto);
                shooter1.setPos(sx, sy); shooter1.vivo = true;

                invulnerabileRespawn = true; tempoUltimoRespawn = std::chrono::steady_clock::now();
                if (lv->timerLivello) lv->timerLivello->riprendiTimer();
                continue;
            } else if (assegnaPunti) {
                 giocoInCorso = false; continue;
            }
        }

        for (int i = 0; i < MAX_BOMBE; i++) {
            if (arrayBombe[i] != NULL) {
                if (!arrayBombe[i]->isEsplosa_funzione()) arrayBombe[i]->disegnaBomba(win); else arrayBombe[i]->disegnaEsplosione(win);
            }
        }

        if (invulnerabileRespawn) {
            auto oraAttuale = std::chrono::steady_clock::now();
            auto millisecondi = std::chrono::duration_cast<std::chrono::milliseconds>(oraAttuale.time_since_epoch()).count();
            if ((millisecondi / 200) % 2 == 0) player1.disegna(win);
        } else { player1.disegna(win); }

        enemy1.disegnanemico(win); chaser1.disegnanemico(win); shooter1.disegnanemico(win);
        statusPartita->disegnaStatus(win, righe, 18);
        if (lv->timerLivello != NULL) lv->timerLivello->disegna(win, righe, 2);

        mvwprintw(win, righe + 1, 0, "Spazio:Bomba|N/P:Scorri Mappa|Q:Esc");
        mvwprintw(win, righe + 2, 0, "Effetti: %s %s %s %s %s [LV: %d/5]", power_raggio ? "[+Raggio]" : "        ", power_immunita ? "[Scudo]" : "       ", power_lento ? "[Lento]" : "       ", power_piubombe ? "[+Bombe]" : "       ", invulnerabileRespawn ? "[INV]" : "     ", numeroLivello);
        wrefresh(win);

        int tasto_premuto = getch();

        int limite_attuale = power_piubombe ? MAX_BOMBE : 1;
        giocoInCorso = player1.gestisciinput(tasto_premuto, mappa, arrayBombe, limite_attuale, power_raggio, lv->timerLivello, statusPartita, navigaLivello);
    }

    if (statusPartita && lv->timerLivello) statusPartita->convertiTempoInPunti(lv->timerLivello->getTempo());
    for (int i = 0; i < num_premi; i++) if (premi_livello[i]) delete premi_livello[i];
    for (int i = 0; i < MAX_BOMBE; i++) if (arrayBombe[i]) delete arrayBombe[i];
    while (lv->precedente != NULL) lv = lv->precedente;
    while (lv != NULL) { gestore_livelli* daCancellare = lv; lv = lv->successivo; delete daCancellare; }
    delete statusPartita;

    endwin();
    return 0;
}


/*
#include <curses.h>
#include <chrono>
#include <cstdlib>

// =====================
// MY COLORS (OBBLIGATORI)
// =====================
#define MY_RED 1
#define MY_YELLOW 2
#define MY_GREEN 3
#define MY_PURPLE 4
#define MY_CYAN 5

int main() {
    initscr();

    if (!has_colors()) {
        printw("No colors\n");
        getch();
        endwin();
        return 0;
    }

    start_color();

    // ROSSO (foreground 1)
    init_pair(MY_RED, 1, 0);

    // GIALLO (foreground 3)
    init_pair(MY_YELLOW, 3, 0);

    // GRIGIO (su curses spesso è 7 o 8, qui usiamo 7)
    init_pair(MY_GREEN, 2, 0);

    init_pair(MY_CYAN, 6, 0);


    color_set(MY_RED, NULL);
    printw("TEST ROSSO\n");

    color_set(MY_YELLOW, NULL);
    printw("TEST GIALLO\n");

    color_set(MY_GREEN, NULL);
    printw("TEST GREEN\n");

    color_set(MY_PURPLE, NULL);
    printw("TEST PURPLE\n");

    color_set(0, NULL);
    printw("TEST NORMALE\n");

    refresh();
    getch();
    endwin();

    return 0;
}
*/