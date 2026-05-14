//
// Created by alice flamigni on 08/03/26.
//

#ifndef PROVA_PROGETTO_PREMIO_H
#define PROVA_PROGETTO_PREMIO_H
#include <ncurses.h>
#include <chrono>// serve per contare i 10 sec dell'effetto del premio
/* premio: raccolto dal personaggio una volta che le sue coordinate e quelle del premio
 * coincidono. compare una volta che viene rotto un muro distruttibile (non si trova sotto
 * ogni muro distruttibile). il premio, se raccolto (se il personaggio ci va sopra), fornisce
 * diversi tipi di vantaggi per una durata di 10 secondi. se non viene raccolto, rimane lì
*/

enum TipoPremio_enum { // tipo di dato, formato da un insieme di costanti intere a cui è assegnato un nome (es: cibo alle macchinette)
    // (la macchina capisce i numeri, non le parole -> creo un'etichetta)
    AUMENTA_RAGGIO = 0,      // corrisponde al 'r' (aumenta il raggio di scoppio)
    IMMUNITA = 1,            // corrisponde al '$' (immunità dai nemici)
    DIMINUISCI_VELOCITA = 2,    // corrisponde al '-' (diminuisce la velocità dei nemici)
    POSIZIONA_PIU_BOMBE = 3   // corrisponde al '+' (ti permette di posizionare più di una bomba alla volta)
};


class Premio {
    protected:
    int x; // coordinata x del premio
    int y; // coordinata y del premio
    int tipo; // tipologia di premio diversa : 'r' (aumenta il raggio di scoppio), '$' (immunità dai nemici), '-' (diminuisce velocita dei nemici), '+' (posiziona più di una bomba alla volta)
    bool nascosto; // true se il premio si trova ancora sotto a un muro distruttibile
    bool raccolto; // true se l'oggetto (dopo che è stato distrutto il muro distruttibile sotto cui era) è stato raccolto dal personaggio
    //(se il personaggio ha avuto le stesse coordinate del premio)
    std::chrono::steady_clock::time_point momentoRaccolta; // memorizza l'istante in cui il premio viene raccolto cosi da poter far durare l'effetto 10 secondi

    public:

    Premio(int startX, int startY, int tipoPremio); // costruttore: coordinate del premio, tipologia di premio (4 tipi diversi)

    void scopri();
    void raccogli();

    bool isNascosto(); // true se il premio sta ancora sotto a un muro
    bool isRaccolto(); // true se a un certo punto il premio (scoperto dal muro) e il personaggio hanno avuto le stesse coordinate
    bool isEffettoFinito(); // true se sono passati 10 secondi dal raccoglimento del premio

    int getX(); // da coordinata x del premio
    int getY(); // da coordinata y del premio
    int getTipo(); // da tipo di premio

    void disegnaPremio(WINDOW* win); // disegna premio (a seconda del tipo di premio che viene scoperto dal muro distruttibile)
};


#endif //PROVA_PROGETTO_PREMIO_H
// tutti gli items presi aumentano il punteggio di 50 punti, tutti i nemici distrutti
// uguale.
//item che: 1. da maggiore raggio di scoppio alla bomba per 10 sec; 2. da immunità dai nemici per 10 secondi;
//3. rallenta la velocità dei nemici per 10 secondi.