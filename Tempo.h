//
// Created by alice flamigni on 04/03/26.
//

#ifndef PROVA_PROGETTO_TEMPO_H
#define PROVA_PROGETTO_TEMPO_H
#include <ctime>

class Tempo {
protected:
    int secondiRimanenti;
    time_t ultimoSecondo;

public:
    Tempo();

    void aggiorna();
    int getSecondi ();
    bool isTempoScaduto();
    void livelloCompletato();
};

// il giocatore guadagna 10 secondi una volta che ha completato un livello. il massimo
//di secondi totali resta sempre uguale al tempo con cui si è iniziato
#endif //PROVA_PROGETTO_TEMPO_H