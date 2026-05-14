//
// Created by alice flamigni on 04/03/26.
#ifndef PROVA_PROGETTO_GAMESTATUS_H
#define PROVA_PROGETTO_GAMESTATUS_H
#include <ncurses.h>

/* la classe GameStatus gestisce vite e punti del giocatore. si parte da
 * 5 vite e 0 punti. una vita in meno se il personaggio è colpito da una bomba
 * o da un nemico. i punti incrementano quando il personaggio uccide un nemico
 * e se alla fine del gioco rimangono secondi extra, essi si convertono in punti.
 */

class GameStatus {
protected:
    int vite; // parametro intero per numero di vite
    int punti; // parametro intero per numero di punti

public:
    GameStatus(); // costruttore: nel .cpp ci imposto vite = 5 e punti = 0

    void perdiVita(); // vite --;
    int getVite(); // torna il numero delle vite
    bool isGiocoFinito(); // true se ho 0 o meno vite
    void uccidiNemico(int puntiOttenuti); // somma i punti ai punti ottenuti
    void convertiTempoInPunti(int secondiRimanenti); // converte il tempo rimanente a fine partita in punti (conversione di 1:1)
    int getPunti(); // torna i punti


    void aggiungiPuntiNemico_BASE(); // aggiunge 50 punti se uccidi un nemico
    void aggiungiPuntiLivello(); // aggiunge 40 punti se superi un livello


    void disegnaStatus(WINDOW* win, int y, int x); // disegna la barra in alto che tiene il conteggio dei punti (simile a quella del tempo)
};
#endif // PROVA_PROGETTO_GAMESTATUS_H