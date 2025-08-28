#ifndef JUEGO_H
#define JUEGO_H

#include "../PTablero/Tablero.h"
#include "../Estructuras/ColaC.h"
#include "../Objetos/Jugador.h"
#include "../Estructuras/ArregloT.h"  

class Juego {
private:
    Tablero* tablero;
    ColaC* jugadores;
    bool juegoTerminado;

    // sistema desempate
    Jugador* determinarGanadorCompleto();
    void mostrarEstadisticasFinales();
    ArregloT<Jugador*> filtrarPorPuntos(const ArregloT<Jugador*>& candidatos);
    ArregloT<Jugador*> filtrarPorCuadrados(const ArregloT<Jugador*>& candidatos);
    ArregloT<Jugador*> filtrarPorFilas(const ArregloT<Jugador*>& candidatos);
    ArregloT<Jugador*> filtrarPorColumnas(const ArregloT<Jugador*>& candidatos);
    ArregloT<Jugador*> filtrarPorIsla(const ArregloT<Jugador*>& candidatos);
    ArregloT<Jugador*> filtrarPorPowerUps(const ArregloT<Jugador*>& candidatos);
    void mostrarCandidatos(const ArregloT<Jugador*>& candidatos, const std::string& criterio);

public:
    Juego(int filas, int columnas);  
    
    // Regla de los 3
    ~Juego();
    Juego(const Juego& otro);
    Juego& operator=(const Juego& otro);
    
    void agregarJugador(const Jugador& jugador);  
    void iniciar();
    void mostrarEstado();
    bool verificarFinJuego();
    
private:
    void procesarTurno();
    void mostrarTablero();
};

#endif