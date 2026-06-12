#include "classifica.hpp"
#include <iostream>
#include <fstream>
#include <curses.h>

using namespace std;

// Ritorna una lista a partire dal file classifica.txt
NodoClassifica* leggiClassifica() {
    ifstream foglio("classifica.txt");
    
    // Se il file non esiste ancora, ritorniamo semplicemente una lista vuota
    if (!foglio.is_open()) {
        return nullptr;
    }

    NodoClassifica* testa = nullptr;
    NodoClassifica* coda = nullptr;
    char bufferNome[4];
    int puntiLetti;

    while (foglio >> bufferNome >> puntiLetti) {
        NodoClassifica* nuovoNodo = new NodoClassifica;
        nuovoNodo->punti = puntiLetti;

        // Copia sicura dei caratteri inserendo il terminatore alla fine
        for (int i = 0; i < 3; i++) {
            nuovoNodo->nome[i] = bufferNome[i];
        }
        nuovoNodo->nome[3] = '\0'; 
        nuovoNodo->next = nullptr;

        if (testa == nullptr) {
            testa = nuovoNodo;
            coda = nuovoNodo;
        } else {
            coda->next = nuovoNodo;
            coda = nuovoNodo;
        }
    }
    foglio.close();
    return testa;
}

// Libera la memoria dinamica della lista per evitare leak
void liberaLista(NodoClassifica*& testa) {
    NodoClassifica* corrente = testa;
    while (corrente != nullptr) {
        NodoClassifica* prossimo = corrente->next;
        delete corrente;
        corrente = prossimo;
    }
    testa = nullptr;
}

// Stampa temporanea in Standard Output (da convertire in ncurses in futuro)
void stampaLista(NodoClassifica* testa) {
    timeout(-1);
    int y_iniziale = 3; // Riga da cui iniziare a stampare
    int x_iniziale = 5; // Colonna da cui iniziare a stampare (margine sinistro)

    // 1. Puliamo lo schermo per evitare residui del menu
    erase();

    // 2. Stampiamo un titolo carino per la classifica
    attron(A_BOLD); // Opzionale: rende il titolo in grassetto
    mvprintw(y_iniziale, x_iniziale, "=== CLASSIFICA HIGHSCORES ===");
    attroff(A_BOLD);

    y_iniziale += 2; // Lasciamo una riga vuota sotto il titolo

    NodoClassifica* corrente = testa;

    // Se la lista è vuota (es. file non trovato o vuoto)
    if (corrente == nullptr) {
        mvprintw(y_iniziale, x_iniziale, "Ancora nessun punteggio salvato!");
        y_iniziale++;
    }
    // Altrimenti stampiamo i giocatori
    else {
        while (corrente != nullptr) {
            // Usiamo la sintassi stile printf: %s per il nome (array di char) e %d per i punti
            mvprintw(y_iniziale, x_iniziale, "Giocatore: %s   |   Punti: %d", corrente->nome, corrente->punti);

            corrente = corrente->next;
            y_iniziale++; // Incrementiamo la riga per il prossimo giocatore
        }
    }

    // 3. Chiediamo all'utente di premere un tasto per uscire, altrimenti il menu
    // ridisegnerebbe tutto all'istante senza darti il tempo di leggere!
    y_iniziale += 2;
    mvprintw(y_iniziale, x_iniziale, "Premi un tasto qualsiasi per tornare al menu...");

    refresh(); // Forza ncurses a mostrare tutto quello che abbiamo stampato
    getch();   // Attende la pressione di un tasto prima di uscire dalla funzione
}

// Inserisce un nuovo punteggio in modo ordinato decrescente
NodoClassifica* inserisci_risultato(const char nome[], int punti, NodoClassifica* classifica) {
    NodoClassifica* newPunteggio = new NodoClassifica;
    newPunteggio->punti = punti;
    
    for (int i = 0; i < 3; i++) {
        newPunteggio->nome[i] = nome[i];
    }
    newPunteggio->nome[3] = '\0';
    newPunteggio->next = nullptr;

    // Inserimento in testa (se la lista è vuota o il punteggio è il più alto)
    if (classifica == nullptr || newPunteggio->punti > classifica->punti) {
        newPunteggio->next = classifica;
        classifica = newPunteggio;
    } 
    // Inserimento in mezzo o in coda
    else {
        NodoClassifica* tmp = classifica;
        while (tmp->next != nullptr && tmp->next->punti >= newPunteggio->punti) {
            tmp = tmp->next;
        }
        newPunteggio->next = tmp->next;
        tmp->next = newPunteggio;
    }
    return classifica;
}

// Scrive sul foglio "classifica.txt" il contenuto corretto della lista
void aggiorna_foglio(NodoClassifica* classifica) {
    ofstream foglio("classifica.txt");
    if (!foglio.is_open()) {
        cerr << "Errore: Impossibile aprire o creare classifica.txt" << endl;
        return;
    }

    NodoClassifica* tmp = classifica;
    while (tmp != nullptr) {
        // Ora il nome è salvato in modo sicuro e non genera stringhe corrotte
        foglio << tmp->nome << "  " << tmp->punti << endl;
        tmp = tmp->next;
    }
    foglio.close();
}

// Funzione principale di gestione
void aggiorna_classifica(const char nome[], int punteggio) {
    // 1) Carica i vecchi punteggi
    NodoClassifica* classifica = leggiClassifica();
    
    // 2) Aggiunge il nuovo record mantenendo l'ordine
    classifica = inserisci_risultato(nome, punteggio, classifica);
    
    // 3) Sovrascrive il file txt
    aggiorna_foglio(classifica);
    
    // 4) Pulisce la memoria una volta sola alla fine di tutto il processo
    liberaLista(classifica);
}