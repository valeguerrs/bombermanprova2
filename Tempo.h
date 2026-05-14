//
// Created by alice flamigni on 04/03/26.
//

#ifndef PROVA_PROGETTO_TEMPO_H
#define PROVA_PROGETTO_TEMPO_H
#include <chrono>
#include <ncurses.h>

class Tempo {
protected:
    int tempoRimanente; // numero di secondi rimanenti
    std::chrono::steady_clock::time_point ultimoAggiornamento; // memorizza un istante preciso nel tempo (tipo cronometro che va solo in avanti);
                                                                //si inizializza all'interno del costruttore (file .cpp). permette di calcolare quanto tempo passa tra un controllo e l'altro
    
public:
    Tempo(); // costruttore: quando viene chiamato nel main imposta tempoRimanente a 1000 e inizializza l'ultimoAggiornamento

    void aggiorna(); // da chiamare nel ciclo del main (viene chiamato continuamente) per scalare i secondi
    void fineLivello(); // aggiunge 50 secondi ogni volta che si finisce un livello (arriva a un massimo di 1000)
    int convertiInPunti(); // restituisce i secondi rimasti convertiti in punti (scala 1:1)

    int getTempo(); // restituisce i secondi rimanenti
    bool isScaduto(); // restituisce true se il tempo arriva a 0

    void disegna(WINDOW* win, int y, int x); // disegna la barra del tempo alle coordinate indicate
};
// parte da 500. incrementa punti se ho tempo extra
// il giocatore guadagna 50 secondi una volta che ha completato un livello. il massimo
//di secondi totali resta sempre uguale al tempo con cui si è iniziato
#endif //PROVA_PROGETTO_TEMPO_H