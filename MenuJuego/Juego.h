#ifndef JUEGO_H
#define JUEGO_H

#include "../PTablero/Tablero.h"
#include "../Estructuras/ColaC.h"
#include "../Objetos/Jugador.h"

class Juego {
private:
    Tablero* tablero;
    ColaC* jugadores;
    bool juegoTerminado;

public:
    Juego(int filas, int columnas);  // MODIFICADO: ahora acepta filas y columnas
    
    // Regla de los 3
    ~Juego();
    Juego(const Juego& otro);
    Juego& operator=(const Juego& otro);
    
    void agregarJugador(const Jugador& jugador);  // MODIFICADO: ahora acepta referencia
    void iniciar();
    void mostrarEstado();
    bool verificarFinJuego();
    
private:
    void procesarTurno();
    void mostrarTablero();
};

#endif