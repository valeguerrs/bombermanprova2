#include "player.hpp"
#include "bomba.hpp"
#include "tempo.hpp"
#include "gamestatus.hpp"

player::player(int x, int y) : personaggio(x, y, '@') {}

bool player::gestisciinput(int tasto, char mappa[righe][colonne], Bomba* bombe[], int limiteBombe,
                           bool raggioAumentato, Tempo* timerGioco, GameStatus* statusGioco, int &navigaLivello) {
    // NON CHIAMARE PIÙ GETCH() QUI DENTRO, USA IL PARAMETRO "TASTO"

    switch (tasto) {
        case 27:
        case 'q': case 'Q': return false;

        case KEY_UP:
        case 'w': case 'W': muovi(0, -1, mappa); break;

        case KEY_DOWN:
        case 's': case 'S': muovi(0, 1, mappa); break;

        case KEY_LEFT:
        case 'a': case 'A': muovi(-1, 0, mappa); break;

        case KEY_RIGHT:
        case 'd': case 'D': muovi(1, 0, mappa); break;

        case 'n': case 'N': navigaLivello = 1; break;
        case 'p': case 'P': navigaLivello = -1; break;

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