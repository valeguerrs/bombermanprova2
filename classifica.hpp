#ifndef CLASSIFICA_HPP
#define CLASSIFICA_HPP

// La struttura che rappresenta ogni riga della classifica
struct NodoClassifica {
    int punti;
    char nome[4]; // Contiene 3 lettere + il terminatore '\0'
    NodoClassifica* next;
};

// Prototipi delle funzioni pubbliche e di supporto
NodoClassifica* leggiClassifica();
void liberaLista(NodoClassifica*& testa);
void stampaLista(NodoClassifica* testa);

NodoClassifica* inserisci_risultato(const char nome[], int punti, NodoClassifica* classifica);
void aggiorna_foglio(NodoClassifica* classifica);

// Questa è la funzione principale che chiamerai dal resto del gioco
void aggiorna_classifica(const char nome[], int punteggio);

#endif // CLASSIFICA_HPP