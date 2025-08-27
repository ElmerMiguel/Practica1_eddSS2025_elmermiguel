#include "Tablero.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Tablero::Tablero(int filas, int columnas) : filas(filas), columnas(columnas) {
    celdas = new ListaT();
    gestorPowers = new GestorPowerUps();  // NUEVO
    inicializar();
    generarPowerUpsAleatorios();  // NUEVO
}

// DESTRUCTOR
Tablero::~Tablero() {
    delete celdas;
    delete gestorPowers;  // NUEVO
    celdas = nullptr;
    gestorPowers = nullptr;
}

// COPY CONSTRUCTOR
Tablero::Tablero(const Tablero& otro) : filas(otro.filas), columnas(otro.columnas) {
    celdas = new ListaT(*otro.celdas);
    gestorPowers = new GestorPowerUps(*otro.gestorPowers);  // NUEVO (necesitarás implementar copy constructor en GestorPowerUps)
}

// ASSIGNMENT OPERATOR
Tablero& Tablero::operator=(const Tablero& otro) {
    if (this != &otro) {
        filas = otro.filas;
        columnas = otro.columnas;
        
        delete celdas;
        delete gestorPowers;  // NUEVO
        
        celdas = new ListaT(*otro.celdas);
        gestorPowers = new GestorPowerUps(*otro.gestorPowers);  // NUEVO
    }
    return *this;
}

void Tablero::inicializar() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            Celda* nuevaCelda = new Celda(i, j);
            celdas->insertar(nuevaCelda);
        }
    }
}

void Tablero::generarPowerUpsAleatorios() {
    srand(time(nullptr));
    
    // Generar PowerUps en algunas celdas (30% de probabilidad)
    NodoLista* actual = celdas->getPrimero();
    while (actual != nullptr) {
        if (rand() % 100 < 30) {  // 30% probabilidad
            PowerUp* powerUp = PowerUp::crearPowerUpAleatorio();
            actual->celda->setPowerUp(powerUp->getSimbolo());
        }
        actual = actual->siguiente;
    }
    
    cout << "PowerUps generados aleatoriamente en el tablero!" << endl;
}

void Tablero::mostrarTablero() {
    cout << "\n=== TABLERO " << filas << "x" << columnas << " ===" << endl;
    
    // Mostrar información de las celdas
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            Celda* celda = obtenerCelda(i, j);
            if (celda != nullptr) {
                cout << "(" << i << "," << j << ")";
                if (!celda->getPowerUp().empty()) {
                    cout << "[" << celda->getPowerUp() << "]";
                }
                if (celda->getPropietario() != ' ') {
                    cout << "{" << celda->getPropietario() << "}";
                }
                cout << " ";
            }
        }
        cout << endl;
    }
    
    // Mostrar efectos activos
    gestorPowers->mostrarEfectosActivos();
}

Celda* Tablero::obtenerCelda(int fila, int columna) {
    return celdas->obtener(fila, columna);
}



bool Tablero::marcarLinea(int fila, int columna, char lado, char jugador) {
    // Verificar si la línea está bloqueada
    if (gestorPowers->lineaBloqueada(fila, columna, lado)) {
        cout << "¡Esta línea está bloqueada!" << endl;
        return false;
    }
    
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) return false;
    
    // Verificar si ya está marcada
    bool yaEstaCapturada = false;
    switch (lado) {
        case 'S': case 's':
            yaEstaCapturada = celda->getLadoSuperior();
            break;
        case 'I': case 'i':
            yaEstaCapturada = celda->getLadoInferior();
            break;
        case 'L': case 'l':
            yaEstaCapturada = celda->getLadoIzquierdo();
            break;
        case 'D': case 'd':
            yaEstaCapturada = celda->getLadoDerecho();
            break;
    }
    
    if (yaEstaCapturada) {
        cout << "Esta línea ya está marcada!" << endl;
        return false;
    }
    
    // NUEVO: Marcar la línea en AMBAS celdas adyacentes
    bool marcadoExitoso = false;
    
    switch (lado) {
        case 'S': case 's': {
            // Marcar lado superior de la celda actual
            celda->setLadoSuperior(true);
            marcadoExitoso = true;
            
            // Marcar lado inferior de la celda de arriba (fila-1)
            if (fila > 0) {
                Celda* celdaArriba = obtenerCelda(fila - 1, columna);
                if (celdaArriba != nullptr) {
                    celdaArriba->setLadoInferior(true);
                }
            }
            break;
        }
        case 'I': case 'i': {
            // Marcar lado inferior de la celda actual
            celda->setLadoInferior(true);
            marcadoExitoso = true;
            
            // Marcar lado superior de la celda de abajo (fila+1)
            if (fila < filas - 1) {
                Celda* celdaAbajo = obtenerCelda(fila + 1, columna);
                if (celdaAbajo != nullptr) {
                    celdaAbajo->setLadoSuperior(true);
                }
            }
            break;
        }
        case 'L': case 'l': {
            // Marcar lado izquierdo de la celda actual
            celda->setLadoIzquierdo(true);
            marcadoExitoso = true;
            
            // Marcar lado derecho de la celda de la izquierda (columna-1)
            if (columna > 0) {
                Celda* celdaIzquierda = obtenerCelda(fila, columna - 1);
                if (celdaIzquierda != nullptr) {
                    celdaIzquierda->setLadoDerecho(true);
                }
            }
            break;
        }
        case 'D': case 'd': {
            // Marcar lado derecho de la celda actual
            celda->setLadoDerecho(true);
            marcadoExitoso = true;
            
            // Marcar lado izquierdo de la celda de la derecha (columna+1)
            if (columna < columnas - 1) {
                Celda* celdaDerecha = obtenerCelda(fila, columna + 1);
                if (celdaDerecha != nullptr) {
                    celdaDerecha->setLadoIzquierdo(true);
                }
            }
            break;
        }
        default:
            return false;
    }
    
    // Verificar si hay trampa
    if (gestorPowers->lineaConTrampa(fila, columna, lado)) {
        cout << "¡TRAMPA ACTIVADA! El jugador que la puso gana puntos!" << endl;
    }
    
    return marcadoExitoso;
}




bool Tablero::verificarCuadradoCompleto(int fila, int columna) {
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) return false;
    
    return celda->estaCompleta();
}

bool Tablero::usarPowerUp(PowerUp* powerUp, int fila, int columna, char lado, char jugador) {
    return gestorPowers->aplicarEfecto(powerUp, fila, columna, lado, jugador);
}

void Tablero::procesarFinTurno() {
    gestorPowers->procesarFinTurno();
}

void Tablero::mostrarEfectosActivos() {
    gestorPowers->mostrarEfectosActivos();
}