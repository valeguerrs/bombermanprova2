//
// Created by alice flamigni on 08/03/26.
//

#ifndef PROVA_PROGETTO_PREMIO_H
#define PROVA_PROGETTO_PREMIO_H
#include <ctime> // serve per contare i 10 sec dell'effetto del premio
/* premio: raccolto dal personaggio una volta che le sue coordinate e quelle del premio
 * coincidono. compare una volta che viene rotto un muro distruttibilr (non si trova sotto
 * ogni muro distruttibile). il premio, se raccolto (se il personaggio ci va sopra), fornisce
 * un maggiore raggio della bomca per una durata di 10 secondi. se non viene raccolto, rimane li
*/
class Premio {
protected:
    int x; // coordinata orizzontale
    int y; // coordinata verticale
    bool nascosto; // false se il muro sotto cui era è stato distrutto, true altrimenti
    bool raccolto;
    time_t momentoRaccolta; // salva orario esatto in cui il personaggio tocca il premio
public:
    Premio(int startX, int startY); // costruttore con coordinate di partenza

    void scopri(); // quando bomba distrugge muro sopra premio diventa
    void raccogli(); // quando il personaggio cammina sulle cordinate del premio.
                    //imposta raccolto=true; salva l'orario attuale in momentoRaccolta

    bool isNascosto();
    bool isRaccolto(); // true se il premio è stato preso
    bool isEffettoFinito(); // calcola se sono passati 10 secondi dall'inizio
                            // dell'effetto, in questo caso ritorna true

    int getX(); // da la coordinata x del premio
    int getY(); // da la coordinata y del premio

    void disegnaPremio(); //creo premio con "!"
    void cancella(); // dopo che l'ho raccolto, devo cancellarlo
};

// mancano: togliere l'item una volta raccolto; dire di aumentare il raggio di 1 in tutte e 4 le direzioni
// (manca funzione dell'effetto del premio vero e proprio)
#endif //PROVA_PROGETTO_PREMIO_H
// tutti gli items presi aumentano il punteggio di 50 punti, tutti i nemici distrutti
// uguale.
//item che: 1. da maggiore raggio di scoppio alla bomba per 10 sec; 2. da immunità dai nemici per 10 secondi;
//3. rallenta la velocità dei nemici per 10 secondi.