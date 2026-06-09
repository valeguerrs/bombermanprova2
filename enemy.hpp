#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "personaggio.hpp"
#include <chrono>

class enemy : public personaggio {
public:
    bool vivo;
    enemy(int x, int y);
    void muovirandom(char mappa[righe][colonne], bool rallentato);
    void disegnanemico(WINDOW* win);
};

class chaser : public enemy {
private:
    int raggioAttivazione;
    std::chrono::steady_clock::time_point ultimomovimento;
public:
    chaser(int x, int y);
    void aggiorna(char mappa[righe][colonne], int pX, int pY, punto scia[], bool lento);
};

class shooter : public enemy {
private:
    std::chrono::steady_clock::time_point ultimoSparo;
    std::chrono::steady_clock::time_point ultimoMovimento;
    bool inPausa;

    bool visioneLibera(int pX, int pY, char mappa[righe][colonne]);
    void muoviCasuale(char mappa[righe][colonne]);

public:
    shooter(int x, int y);
    void aggiorna(int pX, int pY, char mappa[righe][colonne], proiettile proiettili[], int maxP);
};

#endif