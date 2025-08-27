#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego(int filas, int columnas) : juegoTerminado(false) {
    tablero = new Tablero(filas, columnas);
    jugadores = new ColaC();
}

// DESTRUCTOR
Juego::~Juego() {
    delete tablero;
    delete jugadores;
    tablero = nullptr;
    jugadores = nullptr;
}

// COPY CONSTRUCTOR
Juego::Juego(const Juego& otro) : juegoTerminado(otro.juegoTerminado) {
    tablero = new Tablero(*otro.tablero);
    jugadores = new ColaC(*otro.jugadores);
}

// ASSIGNMENT OPERATOR
Juego& Juego::operator=(const Juego& otro) {
    if (this != &otro) {
        juegoTerminado = otro.juegoTerminado;
        
        delete tablero;
        delete jugadores;
        
        tablero = new Tablero(*otro.tablero);
        jugadores = new ColaC(*otro.jugadores);
    }
    return *this;
}

void Juego::agregarJugador(const Jugador& jugador) {
    // Crear una copia del jugador para almacenar en la cola
    Jugador* nuevoJugador = new Jugador(jugador);
    jugadores->encolar(nuevoJugador);
}

void Juego::iniciar() {
    cout << "=== INICIANDO JUEGO ===" << endl;
    
    while (!juegoTerminado) {
        procesarTurno();
        
        if (verificarFinJuego()) {
            juegoTerminado = true;
            cout << "¡Juego terminado!" << endl;
            mostrarEstado();
        }
    }
}

void Juego::procesarTurno() {
    if (jugadores->estaVacia()) return;
    
    Jugador* actual = jugadores->frente_cola();
    cout << "\n=== Turno de: " << actual->getNombre() 
         << " (" << actual->getInicial() << ") ===" << endl;
    
    mostrarTablero();
    actual->mostrarInfo();
    
    // Obtener entrada del usuario
    int fila, columna;
    char lado;
    cout << "Ingrese fila, columna y lado (S/I/L/D): ";
    cin >> fila >> columna >> lado;
    
    // Intentar marcar la línea
    if (tablero->marcarLinea(fila, columna, lado)) {
        cout << "Línea marcada exitosamente!" << endl;
        
        // Verificar si se completó un cuadrado
        if (tablero->verificarCuadradoCompleto(fila, columna)) {
            actual->incrementarPuntos();
            cout << "¡" << actual->getNombre() << " completó un cuadrado!" << endl;
            // No avanzar turno, el mismo jugador continúa
        } else {
            // Avanzar al siguiente jugador
            Jugador* jugadorActual = jugadores->desencolar();
            jugadores->encolar(jugadorActual);
        }
    } else {
        cout << "Movimiento inválido. Intente de nuevo." << endl;
    }
}

void Juego::mostrarTablero() {
    tablero->mostrarTablero();
}

void Juego::mostrarEstado() {
    cout << "\n=== ESTADO FINAL ===" << endl;
    
    // Mostrar puntuación de todos los jugadores
    if (!jugadores->estaVacia()) {
        Jugador* inicial = jugadores->frente_cola();
        Jugador* actual = inicial;
        
        do {
            actual->mostrarInfo();
            jugadores->encolar(jugadores->desencolar());
            actual = jugadores->frente_cola();
        } while (actual != inicial);
    }
}

bool Juego::verificarFinJuego() {
    // Por ahora, permitir salida manual
    char continuar;
    cout << "¿Continuar jugando? (s/n): ";
    cin >> continuar;
    return (continuar == 'n' || continuar == 'N');
}