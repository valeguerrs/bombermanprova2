#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "personaggio.hpp"

class Bomba;
class Tempo;
class GameStatus;

class player : public personaggio {
public:
    player(int x, int y);
    bool gestisciinput(int tasto, char mappa[righe][colonne], Bomba* bombe[], int limiteBombe,
                   bool raggioAumentato, Tempo* timerGioco, GameStatus* statusGioco, int &navigaLivello);
};

#endif