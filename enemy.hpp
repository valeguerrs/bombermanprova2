#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "personaggio.hpp"
#include "costanti.hpp"
#include <chrono>

class enemy : public personaggio {
protected:
    // VARIABILI PERSONALI DEL SINGOLO NEMICO (Senza 'static'!)
    int direzione;
    std::chrono::steady_clock::time_point ultimoMovimento;

public:
    bool vivo;
    enemy(int x, int y);
    void muovirandom(char mappa[righe][colonne], bool rallentato);
    void disegnanemico(WINDOW* win);
};

class chaser : public enemy {
protected:
    int raggioAttivazione;
public:
    chaser(int x, int y);
    void aggiorna(char mappa[righe][colonne], int pX, int pY, punto scia[], bool lento);
};

class shooter : public enemy {
protected:
    std::chrono::steady_clock::time_point ultimoSparo;
    bool inPausa;
public:
    shooter(int x, int y);
    void aggiorna(int pX, int pY, char mappa[righe][colonne], proiettile proiettili[], int maxP);
    bool visioneLibera(int pX, int pY, char mappa[righe][colonne]);
    void muoviCasuale(char mappa[righe][colonne]);
};

#endif