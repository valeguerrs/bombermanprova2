//
// Created by alice flamigni on 08/03/26.
//

//OGNI VOLTA CHE C'E 20,40 ANDRÀ CAMBIATO CON RIGHE,COLONNE

#include "Bomba.h"
#include "griglia.cpp"
#include <cstdlib>
#include <ncurses.h>
#include <chrono>

Bomba::Bomba (int startX, int startY, int raggioOra, char mappa[20][40]) { // costruttore
    x = startX;
    y = startY;
    raggioScoppio = raggioOra;
    is_esplosa = false; // all'inizio non è esplosa
    finita = false; // all'inizio non ha finito il tempo di esplosione
    muri_distrutti = false; // inizializzata a false. Diventa true quando la bomba rompe i muri per la prima volta:
                            // questo evita che la mappa venga ricontrollata inutilmente più volte al secondo, risparmiando CPU.

    raggioDx = raggioSx = raggioGiu = raggioSu = 0; // inizializzo i raggi a 0 per sicurezza

    // registra il momento esatto in cui viene creata (piazzata)
    tempoPiazzamento = std::chrono::steady_clock::now(); // nel parametro protected del file .h del tempo di piazzamento ci vado a memorizzare l'istante in cui viene sganciata.
                                    // chiamata una sola volta nel costruttore, ossia quando la bomba viene sganciata
    mappa[y][x] = '!'; // inizializzo la mappa dichiarata nel costruttore cosi in modo che alle coordinate x, y (gestite nel main: significa che il mio personagggio di trova in quelle
    // coordinate quando dichiaro la bomba da sganciare) sostituisco il blocco '!' anziche mettere uno spazio vuoto calpestabile

} //IL COSTRUTTORE FINISCE QUA

int Bomba::getX() { return x; }
int Bomba::getY() { return y; }
int Bomba::getRaggio() { return raggioScoppio; }

bool Bomba::isEsplosa_funzione() {
    if (!is_esplosa && !finita) { // entra nell'if solo se la bomba non è ancora esplosa e se l'esplosione non è terminata
        std::chrono::steady_clock::time_point orarioAttuale = std::chrono::steady_clock::now(); // una volta stabilito che non è esplosa/finito il tempo di scoppio, registra l'istante corrente per confrontarlo con 'tempoPiazzamento'.
        // viene chiamato continuamente finché la bomba è a terra per verificare se sono passati i secondi necessari per l'esplosione.

        auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - tempoPiazzamento).count(); // vedi commento tempo.cpp/ qua sotto
        if ((secondiTrascorsi) >= 3) { // se sono passati almeno 3 secondi da quando ho piazzato la bomba, la faccio esplodere
            is_esplosa = true; // faccio esplodere la bomba
            tempoEsplosione = std::chrono::steady_clock::now(); // registra l'istante esatto in cui la bomba scoppia.
            // a differenza di 'tempoPiazzamento' (registrato alla creazione), questo ottiene un valore reale solo ora.
            // servd neiesuccessivi controlli per calcolare quando sarà trascorso 1 secondo dallo scoppio
        }
    }
    return is_esplosa;
}

bool Bomba::isFinita() { // questa funzione si comporta esattamente come isEsploso_funzione, solo che
    //anziché verificare se la bomba deve esplodere, verifica se è passato il tempo necessario per eliminare il raggio di scoppio
    if (is_esplosa && !finita) {
        std::chrono::steady_clock::time_point orarioAttuale = std::chrono::steady_clock::now(); // variabile identica a quella della funzione precedente, la dichiaro di nuovo perché l'altra
        //esisteva solo dentro la funzione:  questa varibile può avere anche un nome diverso
        // l'esplosione scompare 1 secondo dopo lo scoppio

        // !!!!!!!!!!!!!!!!!!!!!!!! CORREZIONE QUI: Usiamo tempoEsplosione invece di tempoPiazzamento!  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - tempoEsplosione).count();
        //prende orarioAttuale e gli sottrae l'istante di partenzza in modo da avere un delta-tempo; duration_cast<std::chrono::seconds> casta il delta-tempo da nanosecondi a secondi
        // count() : funzione che "tira fuori" il numero dal calcolo del delta-tempo precedente: senza questa funzione
        // secondiTrascorsi non si potrebbe confrontare con un numero, siccome si tratta in se solo di un oggetto e non di un numero
        //auto: stabilisce automaticamente il tipo di variabile senza che me lo debba ricordare

        if ((secondiTrascorsi) >= 1) {
            finita = true;
        }
    }
    return finita;
}

/*
 commento gemini:
Stai calcolando il tempo trascorso partendo da tempoPiazzamento (cioè quando hai sganciato la bomba).
Ma siccome la bomba ci mette 3 secondi a esplodere, quando il programma entra in isFinita(), i secondi
trascorsi da quando l'hai piazzata sono già 3. Di conseguenza, l'if ((secondiTrascorsi) >= 1) si avvera
immediatamente nello stesso millisecondo in cui la bomba esplode, impostando subito finita = true e facendo sparire il fuoco istantaneamente.
Devi calcolare i secondi trascorsi usando tempoEsplosione (che hai giustamente salvato nella funzione precedente), non tempoPiazzamento.
*/
/*
bool Bomba::isFinita() { // questa funzione si comporta esattamente come isEsploso_funzione, solo che
    //anziché verificare se la bomba deve esplodere, verifica se è passato il tempo necessario per eliminare il raggio di scoppio
    if (is_esplosa && !finita) {
        std::chrono::steady_clock::time_point orarioAttuale = std::chrono::steady_clock::now(); // variabile identica a quella della funzione precedente, la dichiaro di nuovo perché l'altra
        //esisteva solo dentro la funzione:  questa varibile può avere anche un nome diverso
        // l'esplosione scompare 1 secondo dopo lo scoppio
        auto secondiTrascorsi = std::chrono::duration_cast<std::chrono::seconds>(orarioAttuale - tempoPiazzamento).count();
        if ((secondiTrascorsi) >= 1) {
            finita = true;
        }
    }
    return finita;
}
*/

void Bomba::esplodiMuri(char mappa[20][40]) { // funzione che modifica graficamente la matrice: se trova un muro distruttibile
                                            // lo modifica con uno spazio vuoto "calpestabile"
    if (is_esplosa && !muri_distrutti) { // entra nell'if se la bomba è esplosa e se muri_distrutti = false cosi da poter
                                        // distruggere il primo muro distruttibile che trova

        mappa[y][x] = vuoto; // l'oggetto fisico "bomba" smette di esistere: sostituisco il "blocco" '!' con 'vuoto' che corrisponde a ' '

        // DESTRA
        for(int i = 1; i <= raggioScoppio; i++) { // ciclo for per contare le caselle: da quella adiacente alla bomba fino a un max pari
                                                    // al raggioScoppio
            if(x + i < 40) { // se il raggio a destra non andrebbe fuori dalla matrice
                if(mappa[y][x + i] == muro) break; // se incontra un muro non distruttibile non succede nulla
                raggioDx = i; // SALVIAMO LA DISTANZA per poter dire poi alla funzione disegnaEsplosione fino a dove arrivare
                if(mappa[y][x + i] == muro_distruttibile) { mappa[y][x + i] = vuoto; break; } // se incontra un muro distruttibile,
                                                                                            // lo sostituisce con un blocco vuoto
            }
        }
        // SINISTRA
        for(int i = 1; i <= raggioScoppio; i++) {
            if(x - i >= 0) {
                if(mappa[y][x - i] == muro) break;
                raggioSx = i; // SALVIAMO LA DISTANZA
                if(mappa[y][x - i] == muro_distruttibile) { mappa[y][x - i] = vuoto; break; }
            }
        }
        // GIÙ
        for(int i = 1; i <= raggioScoppio - 1; i++) { // ho fatto in modo che il raggio verticale sia piu corto di quello orizzontale
            if(y + i < 20) {
                if(mappa[y + i][x] == muro) break;
                raggioGiu = i; // SALVIAMO LA DISTANZA
                if(mappa[y + i][x] == muro_distruttibile) { mappa[y + i][x] = vuoto; break; }
            }
        }
        // SU
        for(int i = 1; i <= raggioScoppio - 1; i++) { // ho fatto in modo che il raggio verticale sia piu corto di quello orizzontale
            if(y - i >= 0) {
                if(mappa[y - i][x] == muro) break;
                raggioSu = i; // SALVIAMO LA DISTANZA
                if(mappa[y - i][x] == muro_distruttibile) { mappa[y - i][x] = vuoto; break; }
            }
        }
        muri_distrutti = true; // comunica che il blocco è stato distrutto, evitando di far ripetere al computer gli stessi
                                // calcoli inutili per tutto il secondo in cui l'esplosione rimane visibile
    }
}

void Bomba::disegnaBomba(WINDOW* win) { // una volta che sgancio la bomba, la disegno con un "!"

    if (!is_esplosa && !finita) { // se l'ho posizionata ma non è né esplosa né ha finito il tempo di scoppio, la disegno
        mvwprintw(win, y, x, "!"); // carattere della bomba prima dello scoppio
    }
}

void Bomba::disegnaEsplosione(WINDOW* win) { //una volta scoppiara la bomba (3 sec dopo averla sganciata) disegno l'esplosione al posto della bomba

    if (is_esplosa && !finita) { // se è esplosa ma non ha finito il tempo di esplosione, disegno l'esplosione
        mvwprintw(win, y, x, "*"); // centro dell'esplosione

        // uso i raggi calcolati invece del raggio globale, così il fuoco si ferma sul muro rotto
        for (int i = 1; i <= raggioSu; i++)  if (y - i > 0)  mvwprintw(win, y - i, x, "|");
        for (int i = 1; i <= raggioGiu; i++) if (y + i < righe - 1) mvwprintw(win, y + i, x, "|");
        for (int i = 1; i <= raggioSx; i++)  if (x - i > 0)  mvwprintw(win, y, x - i, "-");
        for (int i = 1; i <= raggioDx; i++)  if (x + i < colonne - 1) mvwprintw(win, y, x + i, "-");
    }
}
/* ESPLOSIONE VECCHIA : CHE NON SI FERMA SSE INCONTRA UN MURO DISTRUTTIBILE
void Bomba::disegnaEsplosione(WINDOW* win) { // una volta scoppiata la bomba (3 sec dopo averla sganciata) disegno l'esplosione al posto della bomba

    if (is_esplosa && !finita) { // se è esplosa ma non ha finito il tempo di esplosione, disegno l'esplosione
        mvwprintw(win,y, x, "*"); // centro dell'esplosione


        for (int i = 1; i <= raggioScoppio; i++) { // ciclo for che va da 1 al raggio dell'uesplosione in quel momento per poter disegnare lo scoppio.
            // quando prendo un premio che mi aumenta il raggio, il ciclo for disegna un'esplosione piu grande.
            // prima di disegnare l'esplosione controlla che essa non esca dalla griglia (matrice [righe][colonne]): in caso positivo
            // viene disegnata l'esplosione, altrimenti non viene disegnato niente (analisi fatta lato per lato)

            if (y - i > 0) mvwprintw(win,y - i, x, "|"); // fuoco in alto //va aggiunto win
            if (y + i < righe-1) mvwprintw(win,y + i, x, "|"); // fuoco in basso //idem
            if (x - i > 0) mvwprintw(win,y, x - i, "-"); // fuoco a sinistra // idem
            if (x + i < colonne - 1) mvwprintw(win,y, x + i, "-"); // fuoco a destra // idem
        }
    }
} */
