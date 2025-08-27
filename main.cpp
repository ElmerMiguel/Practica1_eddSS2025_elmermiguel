
#include "MenuJuego/Juego.h"

int main() {
    Juego juego(3, 3); // tablero 3x3

    Jugador alice("Alice", 'A');
    Jugador bob("Bob", 'B');
    
    juego.agregarJugador(alice);
    juego.agregarJugador(bob);

    juego.iniciar();
    return 0;
}