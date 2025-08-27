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
    
    // Preguntar si  usar un PowerUp
    if (actual->tienePowerUps()) {
        char usarPower;
        cout << "¿Usar un PowerUp? (s/n): ";
        cin >> usarPower;
        
        if (usarPower == 's' || usarPower == 'S') {
            actual->mostrarPowerUps();
            PowerUp* powerUsado = actual->usarPowerUp();
            if (powerUsado != nullptr) {
                cout << "Ingrese fila, columna y lado para aplicar el PowerUp: ";
                int fila, columna;
                char lado;
                cin >> fila >> columna >> lado;
                
                tablero->usarPowerUp(powerUsado, fila, columna, lado, actual->getInicial());
                
                // Manejar efectos especiales
                if (powerUsado->getTipo() == PASE) {
                    // Mover al final de la cola
                    Jugador* jugadorActual = jugadores->desencolar();
                    jugadores->encolar(jugadorActual);
                    tablero->procesarFinTurno();
                    return; // Terminar turno sin marcar línea
                }
                
                delete powerUsado; // Liberar memoria del PowerUp usado
            }
        }
    }
    
    // Obtener entrada del usuario para marcar linea
    int fila, columna;
    char lado;
    cout << "Ingrese fila, columna y lado (S/I/L/D): ";
    cin >> fila >> columna >> lado;
    
    // Intentar marcar la linea
    if (tablero->marcarLinea(fila, columna, lado, actual->getInicial())) {
        cout << "Línea marcada exitosamente!" << endl;
        
        // Verificar si se completo un cuadrado
        if (tablero->verificarCuadradoCompleto(fila, columna)) {
            actual->incrementarPuntos();
            cout << "¡" << actual->getNombre() << " completó un cuadrado!" << endl;
            
            // Verificar si la celda tiene PowerUp
            Celda* celda = tablero->obtenerCelda(fila, columna);
            if (celda != nullptr && !celda->getPowerUp().empty()) {
                cout << "¡Recogiste un PowerUp: " << celda->getPowerUp() << "!" << endl;
                PowerUp* nuevoPower = PowerUp::crearPowerUpAleatorio();
                actual->agregarPowerUp(nuevoPower);
                celda->setPowerUp(""); // Limpiar PowerUp de la celda
            }
            
            // No avanzar turno, el mismo jugador continúa
        } else {
            // Avanzar al siguiente jugador
            Jugador* jugadorActual = jugadores->desencolar();
            jugadores->encolar(jugadorActual);
        }
    } else {
        cout << "Movimiento inválido. Intente de nuevo." << endl;
    }
    
    // Procesar fin de turno (efectos de PowerUps)
    tablero->procesarFinTurno();
}

void Juego::mostrarTablero() {
    tablero->mostrarTablero();
}

void Juego::mostrarEstado() {
    cout << "\n=== ESTADO FINAL ===" << endl;
    
    // Mostrar puntuacion de todos los jugadores
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