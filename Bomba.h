//
// Created by alice flamigni on 08/03/26.
//


// OGNI VOLTA CHE C'E 20,40 ANDRÀ CAMBIATO CON RIGHE, COLONNE



#ifndef PROVA_PROGETTO_BOMBA_H
#define PROVA_PROGETTO_BOMBA_H
#include <chrono>
#include "griglia.cpp"
#include <ncurses.h>
/* viene lasciata dal personaggio in un punto (x,y); quando colpisce un muro
 * distruttibile, questo si rompe; quando colpisce il personaggio, esso perde
 * una vita; quando colpisce un nemico, questo muore.
 */
class Bomba {
protected:
    int x; // coordinata orizzontale
    int y; // coordinata verticale
    int raggioScoppio; // raggio di espansione dell'esplosione
    bool is_esplosa; // true se ha finito il conto alla rovescia di 3 secondi (è esplosa); così posso cancellarla dopo che è esplosa
    bool finita; // true se l'esplosione è terminata (1 secondo dopo lo scoppio)
    bool muri_distrutti; //la bomba con l'esplosione ha già spaccato i muri intorno a sè? (ha già modificato la mappa?)
    int raggioSu, raggioGiu, raggioDx, raggioSx; // registrano la distanza esatta percorsa dall'esplosione nelle quattro direzioni prima di sbattere contro un muro
    std::chrono::steady_clock::time_point tempoPiazzamento; // memorizza l'istante di rilascio
    std::chrono::steady_clock::time_point tempoEsplosione; // memorizza l'istante di scoppio

public:
    Bomba(int startX, int startY, int raggioOra, char mappa[righe][colonne]); // costruttore che prende coordinate iniziali bomba, raggio e matrice
    // nel costruttore piazzo anche la matrice perché la bomba deve trasformarsi in un "muro solido" (non calpestabile) nell'istante in cui viene
    // creata. dare la mappa al costruttore garantisce che non ci sia alcun ritardo. avrei potuto creare la bomba in modo astratto (solo il disegno ma
    // senza trattarla come muro) e fare un metodo apposta per passarle la mappa, però era piu scomodo siccome nel main avrei dovuto dichiarare due cose
    // anziche una ogni volta, con rischio di errore

    int getX(); // restituisce coordinata x
    int getY(); // restituisce coordinata y
    int getRaggio(); // restituisce il raggio

    bool isEsplosa_funzione(); // restituisce true se sono passati 3 secondi dal piazzamento della bomba
    bool isFinita(); // essenziale per dire al main quando usare la delete

    void esplodiMuri(char mappa[righe][colonne]);
    void disegnaBomba(WINDOW* win);
    void disegnaEsplosione(WINDOW* win);
};
// mancano: conseguenze su muri e personaggi (da fare nella classe personaggio e classe mappa)

#endif //PROVA_PROGETTO_BOMBA_H