#include <iostream>
#include <curses.h>
#include <cstdlib>
#include <cmath>
#include <chrono>

using namespace std;

const int righe = 20;
const int colonne = 40;
const char muro = 'X';
const char muro_distruttibile = '#';
const char vuoto = ' ';

//-----------------------
// CLASSE GAMESTATUS
// ----------------------
class GameStatus {
protected:
    int vite;
    int punti;

public:
    GameStatus() {
        vite = 5;
        punti = 0;
    }

    void perdiVita() { if (vite > 0) vite--; }
    int getVite() { return vite; }
    bool isGiocoFinito() { return vite <= 0; }
    void uccidiNemico(int puntiOttenuti) { punti += puntiOttenuti; }
    void convertiTempoInPunti(int secondiRimanenti) { if (secondiRimanenti > 0) punti += secondiRimanenti; }
    int getPunti() { return punti; }

    void aggiungiPuntiNemico() { uccidiNemico(25); }
    void aggiungiPuntiChaser() { uccidiNemico(50); }
    void aggiungiPuntiShooter() { uccidiNemico(100); }

    void aggiungiPuntiLivello(int livelloSuperato) {
        if (livelloSuperato == 1) punti += 25;
        else if (livelloSuperato == 2) punti += 50;
        else if (livelloSuperato == 3) punti += 75;
        else if (livelloSuperato == 4) punti += 100;
        else if (livelloSuperato == 5) punti += 150;
    }

    void disegnaStatus(WINDOW* win, int y, int x) {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y, x, " VITE: %d | PUNTI: %04d ", vite, punti);
        wattroff(win, COLOR_PAIR(1));
    }
};

//-----------------------
// CLASSE TEMPO
// ----------------------
class Tempo {
protected:
    int tempoRimanente;
    std::chrono::steady_clock::time_point ultimoAggiornamento;

public:
    Tempo() {
        tempoRimanente = 1000;
        ultimoAggiornamento = std::chrono::steady_clock::now();
    }
    void aggiorna() {
        if (tempoRimanente > 0) {
            std::chrono::steady_clock::time_point oraAttuale = std::chrono::steady_clock::now();
            auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(oraAttuale - ultimoAggiornamento).count();
            if (secondiTrascorsi >= 1) {
                tempoRimanente--;
                ultimoAggiornamento = oraAttuale;
            }
        }
    }

    void fineLivello() {
        tempoRimanente += 100;
        if (tempoRimanente > 1000) tempoRimanente = 1000;
    }
    int convertiInPunti() {
        int punti = tempoRimanente;
        tempoRimanente = 0;
        return punti;
    }

    int getTempo() { return tempoRimanente; }
    bool isScaduto() { return tempoRimanente <= 0; }

    void disegna(WINDOW* win, int y, int x) {
        if (tempoRimanente > 100) {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, y, x, " TIME: %03d ", tempoRimanente);
            wattroff(win, COLOR_PAIR(1));
        }
        else {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, y, x, " TIME: %03d ", tempoRimanente);
            wattroff(win, COLOR_PAIR(2));
        }
    }
};

//-----------------------
// CLASSE BOMBA
// ----------------------
class Bomba {
protected:
    int x;
    int y;
    int raggioScoppio;
    bool is_esplosa;
    bool finita;
    bool muri_distrutti;

    int raggioSu, raggioGiu, raggioDx, raggioSx;
    std::chrono::steady_clock::time_point tempoPiazzamento;
    std::chrono::steady_clock::time_point tempoEsplosione;

public:
    Bomba(int startX, int startY, int raggioOra, char mappa[righe][colonne]) {
        x = startX;
        y = startY;
        raggioScoppio = raggioOra;
        is_esplosa = false;
        finita = false;
        muri_distrutti = false;
        raggioSu = raggioGiu = raggioDx = raggioSx = 0;
        tempoPiazzamento = std::chrono::steady_clock::now();

        mappa[y][x]='!';
    }

    int getX() { return x; }
    int getY() { return y; }
    int getRaggioSu() { return raggioSu; }
    int getRaggioGiu() { return raggioGiu; }
    int getRaggioDx() { return raggioDx; }
    int getRaggioSx() { return raggioSx; }

    bool isEsplosa_funzione() {
        if (!is_esplosa && !finita) {
            std::chrono::steady_clock::time_point orarioAttuale = std::chrono::steady_clock::now();
            auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - tempoPiazzamento).count();
            if (secondiTrascorsi >= 3) {
                is_esplosa = true;
                tempoEsplosione = std::chrono::steady_clock::now();
            }
        }
        return is_esplosa;
    }

    bool isFinita() {
        if (is_esplosa && !finita) {
            std::chrono::steady_clock::time_point orarioAttuale = std::chrono::steady_clock::now();
            auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - tempoEsplosione).count();
            if (secondiTrascorsi >= 1) {
                finita = true;
            }
        }
        return finita;
    }

    void esplodiMuri(char mappa[righe][colonne]) {
        if (is_esplosa && !muri_distrutti) {
            mappa[y][x]=vuoto;

            for(int i = 1; i <= raggioScoppio; i++) {
                if(x + i < colonne) {
                    if(mappa[y][x + i] == muro) break;
                    raggioDx=i;
                    if(mappa[y][x + i] == muro_distruttibile) { mappa[y][x + i] = vuoto; break; }
                }
            }
            for(int i = 1; i <= raggioScoppio; i++) {
                if(x - i >= 0) {
                    if(mappa[y][x - i] == muro) break;
                    raggioSx=i;
                    if(mappa[y][x - i] == muro_distruttibile) { mappa[y][x - i] = vuoto; break; }
                }
            }
            for(int i = 1; i <= raggioScoppio; i++) {
                if(y + i < righe) {
                    if(mappa[y + i][x] == muro) break;
                    raggioGiu=i;
                    if(mappa[y + i][x] == muro_distruttibile) { mappa[y + i][x] = vuoto; break; }
                }
            }
            for(int i = 1; i <= raggioScoppio - 1; i++) {
                if(y - i >= 0) {
                    if(mappa[y - i][x] == muro) break;
                    raggioSu=i;
                    if(mappa[y - i][x] == muro_distruttibile) { mappa[y - i][x] = vuoto; break; }
                }
            }
            muri_distrutti = true;
        }
    }

    void disegnaBomba(WINDOW* win) {
        if (!is_esplosa && !finita) mvwaddch(win, y, x, '!');
    }

    void disegnaEsplosione(WINDOW* win) {
        if (is_esplosa && !finita) {
            mvwaddch(win, y, x, '*');
            for (int i = 1; i <= raggioSu; i++)  mvwaddch(win, y - i, x, '|');
            for (int i = 1; i <= raggioGiu; i++) mvwaddch(win, y + i, x, '|');
            for (int i = 1; i <= raggioSx; i++)  mvwaddch(win, y, x - i, '-');
            for (int i = 1; i <= raggioDx; i++)  mvwaddch(win, y, x + i, '-');
        }
    }
};

//-----------------------
// CLASSE LIVELLO
// ----------------------
class livello {
protected:
    char griglia [righe][colonne];
    int portaleX;
    int portaleY;

public:
    livello() {
        portaleX = -1;
        portaleY = -1;
        for (int i = 0; i < righe; i++) {
            for (int j = 0; j < colonne; j++) {
                griglia[i][j] = vuoto;
            }
        }
    }

    livello(int n_muri) : livello () {
        for (int i = 0; i < righe; i++) {
            griglia[i][0] = muro;
            griglia[i][colonne - 1] = muro;
        }
        for (int j = 0; j < colonne; j++) {
            griglia[0][j] = muro;
            griglia[righe - 1][j] = muro;
        }

        for (int i = 1; i < righe - 1; i += 2) {
            for (int j = 4; j < colonne - 1; j += 5) {
                griglia[i][j] = muro;
            }
        }
        creaMuri(n_muri);
        scegliPortale();
    }

    void creaMuri(int n) {
        int count = 0;
        while (count < n) {
            int y = rand() % righe;
            int x = rand() % colonne;

            if ((y == 1 && x == 1) || (y == 1 && x == 2) || (y == 2 && x == 1)) continue;

            if (griglia[y][x] == vuoto) {
                griglia[y][x] = muro_distruttibile;
                count++;
            }
        }
    }

    void scegliPortale() {
        int contaDistruttibili = 0;
        for (int i = 0; i < righe; i++) {
            for (int j = 0; j < colonne; j++) {
                if (griglia[i][j] == muro_distruttibile) contaDistruttibili++;
            }
        }
        if (contaDistruttibili == 0) return;

        int target = rand() % contaDistruttibili;
        int corrente = 0;
        for (int i = 0; i < righe; i++) {
            for (int j = 0; j < colonne; j++) {
                if (griglia[i][j] == muro_distruttibile) {
                    if (corrente == target) {
                        portaleX = j; portaleY = i;
                        return;
                    }
                    corrente++;
                }
            }
        }
    }

    int getPortaleX() { return portaleX; }
    int getPortaleY() { return portaleY; }
    char (*getGriglia())[colonne] { return griglia; }

    void disegna(WINDOW* win) {
        for (int i = 0; i < righe; i++) {
            for (int j = 0; j < colonne; j++) {
                char carattereDaDisegnare = griglia[i][j];
                if (carattereDaDisegnare == muro || carattereDaDisegnare == muro_distruttibile) {
                    wattron(win, A_REVERSE);
                    mvwaddch(win, i, j, carattereDaDisegnare == muro ? '#' : ' ');
                    wattroff(win, A_REVERSE);
                } else {
                    mvwaddch(win, i, j, carattereDaDisegnare);
                }
            }
        }

        if (portaleX != -1 && portaleY != -1 && griglia[portaleY][portaleX] == vuoto) {
            mvwprintw(win, portaleY, portaleX, "P");
        }
    }
};

//-----------------------
// CLASSE PERSONAGGIO
// ----------------------
class personaggio {
protected:
    int x, y;
    char character;
public:
    personaggio(int startX, int startY, char s) { x = startX; y = startY; character = s; }

    void muovi(int dx, int dy, char mappa[righe][colonne]) {
        int nextX = x + dx;
        if (nextX >= 0 && nextX < colonne && mappa[y][nextX] == vuoto) x = nextX;
        int nextY = y + dy;
        if (nextY >= 0 && nextY < righe && mappa[nextY][x] == vuoto) y = nextY;
    }
    void disegna(WINDOW* win) { mvwaddch(win, y, x, character); }
    int getX() { return x; }
    int getY() { return y; }
    void setPos(int newX, int newY) { x = newX; y = newY; }
};

//-----------------------
// SOTTOCLASSE PLAYER
// ----------------------
class player : public personaggio {
public:
    player(int x, int y) : personaggio(x, y, '@') {}

    bool gestisciinput(char mappa[righe][colonne], Bomba* bombe[], int limiteBombe, bool raggioAumentato, Tempo* timerGioco, GameStatus* statusGioco, int &navigaLivello) {
        int tasto = getch();
        switch (tasto) {
            case 27: case 'q': case 'Q': return false;
            case KEY_UP: case 'w': muovi(0, -1, mappa); break;
            case KEY_DOWN: case 's': muovi(0, 1, mappa); break;
            case KEY_LEFT: case 'a': muovi(-1, 0, mappa); break;
            case KEY_RIGHT: case 'd': muovi(1, 0, mappa); break;

            case 'n': case 'N':
                navigaLivello = 1;
                break;
            case 'p': case 'P':
                navigaLivello = -1;
                break;

            case ' ':
                if (mappa[y][x]=='!') break;
                for (int i = 0; i < limiteBombe; i++) {
                    if (bombe[i] == NULL) {
                        bombe[i] = new Bomba(x, y, raggioAumentato ? 5 : 2, mappa);
                        break;
                    }
                }
                break;
        }
        return true;
    }
};

//-----------------------
// SOTTOCLASSE ENEMY
// ----------------------
class enemy : public personaggio {
public:
    bool vivo;
    enemy(int x, int y) : personaggio(x, y, 'O') { vivo = true; }

    void muovirandom (char mappa[righe][colonne], bool rallentato) {
        if (!vivo) return;
        static auto ultimoTempo = std::chrono::steady_clock::now();
        auto adesso = std::chrono::steady_clock::now();

        int delay = rallentato ? 800 : 400;

        if (std::chrono::duration_cast<std::chrono::milliseconds>(adesso - ultimoTempo).count() > delay) {
            int randomico = rand() % 4;
            switch (randomico) {
                case 0: muovi(1, 0, mappa); break;
                case 1: muovi(-1, 0, mappa); break;
                case 2: muovi(0, 1, mappa); break;
                case 3: muovi(0, -1, mappa); break;
            }
            ultimoTempo = adesso;
        }
    }
    void disegnanemico (WINDOW*win) {
        if (vivo) { disegna(win); }
    }
};

//-----------------------
// CLASSE CHASER
// ----------------------
struct punto { int x, y; };

class chaser: public enemy {
private:
    int raggioAttivazione;
    std::chrono::steady_clock::time_point ultimomovimento;
public:
    chaser(int x, int y):enemy(x,y) {
        character='C';
        raggioAttivazione=8;
        ultimomovimento=std::chrono::steady_clock::now();
    }
    void aggiorna (char mappa[righe][colonne], int pX, int pY, punto scia[], bool lento) {
        if (!vivo) return;
        auto ora = std::chrono::steady_clock::now();
        int delay = lento ? 800 : 400;

        if (std::chrono::duration_cast<std::chrono::milliseconds>(ora - ultimomovimento).count() >= delay) {
            float dist = sqrt(pow(x - pX, 2) + pow(y - pY, 2));
            if (dist < raggioAttivazione) {
                punto target = scia[0];
                int dx = 0, dy = 0;
                if (target.x > x) dx = 1; else if (target.x < x) dx = -1;
                if (target.y > y) dy = 1; else if (target.y < y) dy = -1;
                muovi(dx, dy, mappa);
            } else {
                int r = rand() % 4;
                if (r == 0) muovi(1, 0, mappa);
                else if (r == 1) muovi(-1, 0, mappa);
                else if (r == 2) muovi(0, 1, mappa);
                else if (r == 3) muovi(0, -1, mappa);
            }
            ultimomovimento = ora;
        }
    }
};

//-----------------------
// CLASSE SHOOTER
// ----------------------
struct proiettile {
    int x, y;
    int dx, dy;
    bool attivo;
};

class shooter: public enemy {
private:
    std::chrono::steady_clock::time_point ultimoSparo;
    std::chrono::steady_clock::time_point ultimoMovimento;
    bool inPausa;
public:
    shooter(int x, int y) : enemy(x, y) {
        character = 'S';
        ultimoSparo = std::chrono::steady_clock::now();
        ultimoMovimento = std::chrono::steady_clock::now();
        inPausa=false;
    }

    void aggiorna(int pX, int pY, char mappa[righe][colonne], proiettile proiettili[], int maxP) {
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

private:
    bool visioneLibera(int pX, int pY, char mappa[righe][colonne]) {
        if (y == pY) {
            int start = (x < pX) ? x + 1 : pX + 1;
            int end = (x < pX) ? pX : x;
            for (int i = start; i < end; i++) {
                if (mappa[y][i] == muro || mappa[y][i] == muro_distruttibile) return false;
            }
        } else {
            int start = (y < pY) ? y + 1 : pY + 1;
            int end = (y < pY) ? pY : y;
            for (int i = start; i < end; i++) {
                if (mappa[i][x] == muro || mappa[i][x] == muro_distruttibile) return false;
            }
        }
        return true;
    }

    void muoviCasuale(char mappa[righe][colonne]) {
        int dir = rand() % 4;
        int nx = x, ny = y;
        if (dir == 0) ny--; else if (dir == 1) ny++; else if (dir == 2) nx++; else if (dir == 3) nx--;
        if (nx >= 0 && nx < colonne && ny >= 0 && ny < righe && mappa[ny][nx] == vuoto) {
            x = nx; y = ny;
        }
    }
};

//-----------------------
// CLASSE PREMIO
// ----------------------
enum TipoPremio_enum {
    AUMENTA_RAGGIO = 0,
    IMMUNITA = 1,
    DIMINUISCI_VELOCITA = 2,
    POSIZIONA_PIU_BOMBE = 3
};

class Premio {
protected:
    int x, y, tipo;
    bool nascosto, raccolto;
    std::chrono::steady_clock::time_point momentoRaccolta;

public:
    Premio(int startX, int startY, int tipoPremio);
    void scopri() { nascosto = false; }
    void raccogli() {
        if (!nascosto && !raccolto) { raccolto = true; momentoRaccolta = std::chrono::steady_clock::now(); }
    }
    bool isNascosto() { return nascosto; }
    bool isRaccolto() { return raccolto; }
    bool isEffettoFinito() {
        if (raccolto) {
            auto oraAttuale = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(oraAttuale - momentoRaccolta).count() >= 10) return true;
        }
        return false;
    }
    int getX() { return x; }
    int getY() { return y; }
    int getTipo() { return tipo; }

    void disegnaPremio(WINDOW* win) {
        if (!nascosto && !raccolto) {
            if (tipo == AUMENTA_RAGGIO) mvwprintw(win, y, x, "r");
            else if (tipo == IMMUNITA) mvwprintw(win, y, x, "$");
            else if (tipo == DIMINUISCI_VELOCITA) mvwprintw(win, y, x, "-");
            else if (tipo == POSIZIONA_PIU_BOMBE) mvwprintw(win, y, x, "+");
        }
    }
};

Premio::Premio(int startX, int startY, int tipoPremio) {
    x = startX; y = startY; tipo = tipoPremio;
    nascosto = true; raccolto = false;
    momentoRaccolta = std::chrono::steady_clock::time_point();
}

// MODIFICA SALVATAGGIO STATO LIVELLO: Spostato gestore_livelli in fondo per incapsulare i nemici e premi generati
struct gestore_livelli {
    livello lv_attuale;
    gestore_livelli *precedente;
    gestore_livelli *successivo;

    Premio* premi_livello[800];
    int num_premi;

    enemy* e1;
    chaser* c1;
    shooter* s1;

    gestore_livelli() {
        precedente = NULL;
        successivo = NULL;
        num_premi = 0;
        for (int i = 0; i < 800; i++) premi_livello[i] = NULL;
        e1 = NULL;
        c1 = NULL;
        s1 = NULL;
    }

    ~gestore_livelli() {
        for (int i = 0; i < num_premi; i++) {
            if (premi_livello[i] != NULL) {
                delete premi_livello[i];
                premi_livello[i] = NULL;
            }
        }
        if (e1 != NULL) delete e1;
        if (c1 != NULL) delete c1;
        if (s1 != NULL) delete s1;
    }

    // Viene chiamato solo la prima volta che si visita un livello nuovo
    void inizializzaEntita(char mappa[righe][colonne]) {
        if (e1 != NULL) return; // Evita di sovrascrivere se le entità esistono già

        num_premi = 0;
        for (int i = 0; i < righe; i++) {
            for (int j = 0; j < colonne; j++) {
                // Generazione dei premi sotto i muri distruttibili
                if (mappa[i][j] == muro_distruttibile) {
                    if (rand() % 100 < 30) {
                        premi_livello[num_premi] = new Premio(j, i, rand() % 4);
                        num_premi++;
                    }
                }
            }
        }

        int ex, ey, cx, cy, sx, sy;
        do { ex = rand() % (colonne - 2) + 1; ey = rand() % (righe - 2) + 1; } while ((ex < 5 && ey < 5) || mappa[ey][ex] != vuoto);
        e1 = new enemy(ex, ey);
        do { cx = rand() % (colonne - 2) + 1; cy = rand() % (righe - 2) + 1; } while ((cx < 5 && cy < 5) || mappa[cy][cx] != vuoto);
        c1 = new chaser(cx, cy);
        do { sx = rand() % (colonne - 2) + 1; sy = rand() % (righe - 2) + 1; } while ((sx < 5 && sy < 5) || mappa[sy][sx] != vuoto);
        s1 = new shooter(sx, sy);
    }
};

//-----------------------
// MAIN
// ----------------------
int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(50);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    unsigned int seme = std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now().time_since_epoch()
                        ).count();
    srand(seme);

    WINDOW* win = newwin(righe + 4, colonne, 0, 0);

    int numeroLivello = 1;
    gestore_livelli* lv = new gestore_livelli;
    lv->lv_attuale = livello(100);
    char(*mappa)[colonne] = lv->lv_attuale.getGriglia();

    // MODIFICA SALVATAGGIO STATO LIVELLO: Chiamata alla nuova funzione che genera i nemici DENTRO al livello 1
    lv->inizializzaEntita(mappa);

    player player1(1, 1);

    const int MAX_PROIETTILI = 10;
    proiettile proiettili[MAX_PROIETTILI];
    for (int i = 0; i < MAX_PROIETTILI; i++) proiettili[i].attivo = false;

    punto scia[1];
    Tempo* timerPartita = new Tempo();
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

        if (timerPartita != NULL) timerPartita->aggiorna();

        if (timerPartita->isScaduto() || statusPartita->isGiocoFinito()) {
            giocoInCorso = false;
            continue;
        }

        lv->lv_attuale.disegna(win);

        // 1. GESTIONE PREMI DEL LIVELLO ATTUALE
        bool power_raggio = false, power_immunita = false, power_lento = false, power_piubombe = false;

        // MODIFICA SALVATAGGIO STATO LIVELLO: Sostituito num_premi e premi_livello globali con i parametri dello specifico livello (lv->...)
        for (int i = 0; i < lv->num_premi; i++) {
            if (lv->premi_livello[i] != NULL) {
                if (lv->premi_livello[i]->isNascosto() && mappa[lv->premi_livello[i]->getY()][lv->premi_livello[i]->getX()] == vuoto) {
                    lv->premi_livello[i]->scopri();
                }
                lv->premi_livello[i]->disegnaPremio(win);
                if (!lv->premi_livello[i]->isNascosto() && !lv->premi_livello[i]->isRaccolto()) {
                    if (player1.getX() == lv->premi_livello[i]->getX() && player1.getY() == lv->premi_livello[i]->getY()) {
                        lv->premi_livello[i]->raccogli();
                    }
                }
                if (lv->premi_livello[i]->isRaccolto() && !lv->premi_livello[i]->isEffettoFinito()) {
                    int t = lv->premi_livello[i]->getTipo();
                    if (t == AUMENTA_RAGGIO) power_raggio = true;
                    else if (t == IMMUNITA) power_immunita = true;
                    else if (t == DIMINUISCI_VELOCITA) power_lento = true;
                    else if (t == POSIZIONA_PIU_BOMBE) power_piubombe = true;
                }
            }
        }

        // 2. LOGICA NEMICI VICINI A BOMBA
        bool nemico1_vicino = false;
        bool chaser1_vicino = false;
        for (int i = 0; i < MAX_BOMBE; i++) {
            if (arrayBombe[i] != NULL && !arrayBombe[i]->isEsplosa_funzione()) {
                // MODIFICA SALVATAGGIO STATO LIVELLO: Sostituito enemy1 con lv->e1 ecc. per ogni nemico
                if (sqrt(pow(lv->e1->getX() - arrayBombe[i]->getX(), 2) + pow(lv->e1->getY() - arrayBombe[i]->getY(), 2)) < 4.0) nemico1_vicino = true;
                if (sqrt(pow(lv->c1->getX() - arrayBombe[i]->getX(), 2) + pow(lv->c1->getY() - arrayBombe[i]->getY(), 2)) < 4.0) chaser1_vicino = true;
            }
        }

        // 3. GESTIONE BOMBE ED ESPLOSIONI SUI NEMICI
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
                                statusPartita->perdiVita();
                                player1.setPos(1, 1);
                                invulnerabileRespawn = true;
                                tempoUltimoRespawn = std::chrono::steady_clock::now();
                                napms(200);
                            }
                        }

                        // MODIFICA SALVATAGGIO STATO LIVELLO: Aggiornati i controlli d'impatto bomba usando i puntatori dei nemici specifici di 'lv'
                        int nx = lv->e1->getX(), ny = lv->e1->getY();
                        if (lv->e1->vivo && ((nx == bx && ny >= by - rU && ny <= by + rD) || (ny == by && nx >= bx - rL && nx <= bx + rR))) {
                            lv->e1->vivo = false; statusPartita->aggiungiPuntiNemico();
                        }

                        nx = lv->c1->getX(); ny = lv->c1->getY();
                        if (lv->c1->vivo && ((nx == bx && ny >= by - rU && ny <= by + rD) || (ny == by && nx >= bx - rL && nx <= bx + rR))) {
                            lv->c1->vivo = false; statusPartita->aggiungiPuntiChaser();
                        }

                        nx = lv->s1->getX(); ny = lv->s1->getY();
                        if (lv->s1->vivo && ((nx == bx && ny >= by - rU && ny <= by + rD) || (ny == by && nx >= bx - rL && nx <= bx + rR))) {
                            lv->s1->vivo = false; statusPartita->aggiungiPuntiShooter();
                        }
                    }
                }
                if (arrayBombe[i]->isFinita()) { delete arrayBombe[i]; arrayBombe[i] = NULL; }
            }
        }

        // 4. MOVIMENTO NEMICI DELLA STANZA ATTUALE
        scia[0] = {player1.getX(), player1.getY()};
        lv->e1->muovirandom(mappa, power_lento || nemico1_vicino);
        lv->c1->aggiorna(mappa, player1.getX(), player1.getY(), scia, power_lento || chaser1_vicino);
        lv->s1->aggiorna(player1.getX(), player1.getY(), mappa, proiettili, MAX_PROIETTILI);

        // 5. PROIETTILI
        static auto lastBulletUpdate = std::chrono::steady_clock::now();
        auto nowBullet = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(nowBullet - lastBulletUpdate).count() >= 100) {
            for (int i = 0; i < MAX_PROIETTILI; i++) {
                if (proiettili[i].attivo) {
                    proiettili[i].x += proiettili[i].dx;
                    proiettili[i].y += proiettili[i].dy;
                    if (mappa[proiettili[i].y][proiettili[i].x] != vuoto) proiettili[i].attivo = false;

                    if (proiettili[i].x == player1.getX() && proiettili[i].y == player1.getY()) {
                        if (!power_immunita && !invulnerabileRespawn) {
                            statusPartita->perdiVita();
                            player1.setPos(1, 1);
                            invulnerabileRespawn = true;
                            tempoUltimoRespawn = std::chrono::steady_clock::now();
                            napms(200);
                        }
                        proiettili[i].attivo = false;
                    }
                }
            }
            lastBulletUpdate = nowBullet;
        }

        for (int i = 0; i < MAX_PROIETTILI; i++) {
            if (proiettili[i].attivo) mvwaddch(win, proiettili[i].y, proiettili[i].x, '.' | COLOR_PAIR(3));
        }

        // 6. COLLISIONI FISICHE
        if (!power_immunita && !invulnerabileRespawn) {
            bool presoFisicamente = false;
            // MODIFICA SALVATAGGIO STATO LIVELLO: Collisioni basate sui puntatori del livello
            if (lv->e1->vivo && player1.getX() == lv->e1->getX() && player1.getY() == lv->e1->getY()) presoFisicamente = true;
            if (lv->c1->vivo && player1.getX() == lv->c1->getX() && player1.getY() == lv->c1->getY()) presoFisicamente = true;
            if (lv->s1->vivo && player1.getX() == lv->s1->getX() && player1.getY() == lv->s1->getY()) presoFisicamente = true;

            if (presoFisicamente) {
                statusPartita->perdiVita();
                player1.setPos(1, 1);
                invulnerabileRespawn = true;
                tempoUltimoRespawn = std::chrono::steady_clock::now();
                napms(200);
            }
        }

        // 7. VERIFICA CAMBIO LIVELLO E NAVIGAZIONE MANUALE
        bool eseguiCambioInAvanti = false;
        bool eseguiCambioIndietro = false;
        bool assegnaPunti = false;

        // Condizione apertura portale
        bool tuttiNemiciMorti = !lv->e1->vivo && !lv->c1->vivo && !lv->s1->vivo;
        int portX = lv->lv_attuale.getPortaleX();
        int portY = lv->lv_attuale.getPortaleY();

        if (tuttiNemiciMorti && portX != -1 && portY != -1 && mappa[portY][portX] == vuoto) {
            if (player1.getX() == portX && player1.getY() == portY) {
                eseguiCambioInAvanti = true;
                assegnaPunti = true;
            }
        }

        // Comandi manuali per spostarsi tra stanze generate e salvate
        if (navigaLivello == 1) {
            eseguiCambioInAvanti = true;
            assegnaPunti = false;
            navigaLivello = 0;
        } else if (navigaLivello == -1) {
            eseguiCambioIndietro = true;
            assegnaPunti = false;
            navigaLivello = 0;
        }

        // MOVIMENTO INDIETRO
        if (eseguiCambioIndietro) {
            if (lv->precedente != NULL) {
                numeroLivello--;
                lv = lv->precedente; // Ci si sposta al livello precedente, che HA GIA' I SUOI NEMICI E PREMI SALVATI!
                mappa = lv->lv_attuale.getGriglia();

                player1.setPos(1, 1);
                for (int i = 0; i < MAX_BOMBE; i++) { if (arrayBombe[i]) { delete arrayBombe[i]; arrayBombe[i] = NULL; } }
                for (int i = 0; i < MAX_PROIETTILI; i++) proiettili[i].attivo = false;

                // MODIFICA SALVATAGGIO STATO LIVELLO: Completamente RIMOSSA la ri-generazione di nemici e premi.
                // Il livello precedente viene lasciato "congelato" esattamente come lo avevamo abbandonato!

                invulnerabileRespawn = true;
                tempoUltimoRespawn = std::chrono::steady_clock::now();
                continue;
            }
        }

        // MOVIMENTO IN AVANTI
        if (eseguiCambioInAvanti) {
            if (assegnaPunti) {
                statusPartita->aggiungiPuntiLivello(numeroLivello);
                timerPartita->fineLivello();
            }

            if (numeroLivello < 5) {
                numeroLivello++;
                if (lv->successivo == NULL) {
                    gestore_livelli* nuovoNodo = new gestore_livelli;
                    nuovoNodo->lv_attuale = livello(100 - (numeroLivello * 10));
                    nuovoNodo->precedente = lv;
                    lv->successivo = nuovoNodo;

                    // MODIFICA SALVATAGGIO STATO LIVELLO: Se il livello non esiste, delega l'inizializzazione alla sua funzione nativa
                    lv->successivo->inizializzaEntita(nuovoNodo->lv_attuale.getGriglia());
                }
                lv = lv->successivo;
                mappa = lv->lv_attuale.getGriglia();

                player1.setPos(1, 1);
                for (int i = 0; i < MAX_BOMBE; i++) { if (arrayBombe[i]) { delete arrayBombe[i]; arrayBombe[i] = NULL; } }
                for (int i = 0; i < MAX_PROIETTILI; i++) proiettili[i].attivo = false;

                // MODIFICA SALVATAGGIO STATO LIVELLO: Anche qua rimossa la generazione di nemici nel main, tutto viene delegato alla struct!

                invulnerabileRespawn = true;
                tempoUltimoRespawn = std::chrono::steady_clock::now();
                continue;
            } else if (assegnaPunti) {
                 giocoInCorso = false;
                 continue;
            }
        }

        // 8. DISEGNO FINALE FRAME
        for (int i = 0; i < MAX_BOMBE; i++) {
            if (arrayBombe[i] != NULL) {
                if (!arrayBombe[i]->isEsplosa_funzione()) arrayBombe[i]->disegnaBomba(win);
                else arrayBombe[i]->disegnaEsplosione(win);
            }
        }

        if (invulnerabileRespawn) {
            auto oraAttuale = std::chrono::steady_clock::now();
            auto millisecondi = std::chrono::duration_cast<std::chrono::milliseconds>(oraAttuale.time_since_epoch()).count();
            if ((millisecondi / 200) % 2 == 0) player1.disegna(win);
        } else {
            player1.disegna(win);
        }

        // MODIFICA SALVATAGGIO STATO LIVELLO: Disegno i nemici legati al livello
        lv->e1->disegnanemico(win);
        lv->c1->disegnanemico(win);
        lv->s1->disegnanemico(win);

        statusPartita->disegnaStatus(win, righe, 18);
        timerPartita->disegna(win, righe, 2);

        mvwprintw(win, righe + 1, 0, "Spazio:Bomba|N/P:Scorri Mappa|Q:Esc");
        mvwprintw(win, righe + 2, 0, "Effetti: %s %s %s %s %s [LV: %d/5]",
                  power_raggio ? "[+Raggio]" : "        ",
                  power_immunita ? "[Scudo]" : "       ",
                  power_lento ? "[Lento]" : "       ",
                  power_piubombe ? "[+Bombe]" : "       ",
                  invulnerabileRespawn ? "[INV]" : "     ",
                  numeroLivello);

        wrefresh(win);

        // 9. INPUT
        int limite_attuale = power_piubombe ? MAX_BOMBE : 1;
        giocoInCorso = player1.gestisciinput(mappa, arrayBombe, limite_attuale, power_raggio, timerPartita, statusPartita, navigaLivello);

    } // FINE WHILE

    if (statusPartita && timerPartita) statusPartita->convertiTempoInPunti(timerPartita->getTempo());
    for (int i = 0; i < MAX_BOMBE; i++) if (arrayBombe[i]) delete arrayBombe[i];

    // Riporta il puntatore alla testa della lista prima di cancellare
    while (lv->precedente != NULL) lv = lv->precedente;

    // Cancella l'intera lista procedendo in avanti (i distruttori cancelleranno i loro premi e nemici privati)
    while (lv != NULL) {
        gestore_livelli* daCancellare = lv;
        lv = lv->successivo;
        delete daCancellare;
    }

    delete timerPartita; delete statusPartita;

    endwin();
    return 0;
}