#include "Premio.hpp"

Premio::Premio(int startX, int startY, int tipoPremio) {
    x = startX; y = startY; tipo = tipoPremio;
    nascosto = true; raccolto = false;
    momentoRaccolta = std::chrono::steady_clock::time_point();
}

void Premio::scopri() { nascosto = false; }

void Premio::raccogli() {
    if (!nascosto && !raccolto) { raccolto = true; momentoRaccolta = std::chrono::steady_clock::now(); }
}

bool Premio::isNascosto() { return nascosto; }
bool Premio::isRaccolto() { return raccolto; }

bool Premio::isEffettoFinito() {
    if (raccolto) {
        auto oraAttuale = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(oraAttuale - momentoRaccolta).count() >= 10) return true;
    }
    return false;
}

int Premio::getX() { return x; }
int Premio::getY() { return y; }
int Premio::getTipo() { return tipo; }

void Premio::disegnaPremio(WINDOW* win) {
    if (!nascosto && !raccolto) {
        if (tipo == AUMENTA_RAGGIO) mvwprintw(win, y, x, "r");
        else if (tipo == IMMUNITA) mvwprintw(win, y, x, "$");
        else if (tipo == DIMINUISCI_VELOCITA) mvwprintw(win, y, x, "-");
        else if (tipo == POSIZIONA_PIU_BOMBE) mvwprintw(win, y, x, "+");
    }
}