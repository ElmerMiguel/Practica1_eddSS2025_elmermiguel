#include "Tablero.h"
#include <iostream>
using namespace std;

Tablero::Tablero(int filas, int columnas) : filas(filas), columnas(columnas) {
    celdas = new ListaT();
    inicializar();
}

// DESTRUCTOR
Tablero::~Tablero() {
    delete celdas;
    celdas = nullptr;
}

// COPY CONSTRUCTOR
Tablero::Tablero(const Tablero& otro) : filas(otro.filas), columnas(otro.columnas) {
    celdas = new ListaT(*otro.celdas);  // Copia profunda de la lista
}

// ASSIGNMENT OPERATOR
Tablero& Tablero::operator=(const Tablero& otro) {
    if (this != &otro) {  // Evitar auto-asignación
        filas = otro.filas;
        columnas = otro.columnas;
        
        delete celdas;  // Liberar lista actual
        celdas = new ListaT(*otro.celdas);  // Crear nueva copia
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

void Tablero::mostrarTablero() {
    cout << "\n=== TABLERO ===" << endl;
    celdas->mostrar();
}

Celda* Tablero::obtenerCelda(int fila, int columna) {
    return celdas->obtener(fila, columna);
}

bool Tablero::marcarLinea(int fila, int columna, char lado) {
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) return false;
    
    switch (lado) {
        case 'S': case 's':
            if (!celda->getLadoSuperior()) {
                celda->setLadoSuperior(true);
                return true;
            }
            break;
        case 'I': case 'i':
            if (!celda->getLadoInferior()) {
                celda->setLadoInferior(true);
                return true;
            }
            break;
        case 'L': case 'l':
            if (!celda->getLadoIzquierdo()) {
                celda->setLadoIzquierdo(true);
                return true;
            }
            break;
        case 'D': case 'd':
            if (!celda->getLadoDerecho()) {
                celda->setLadoDerecho(true);
                return true;
            }
            break;
    }
    return false;
}

bool Tablero::verificarCuadradoCompleto(int fila, int columna) {
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) return false;
    
    return celda->getLadoSuperior() && celda->getLadoInferior() && 
           celda->getLadoIzquierdo() && celda->getLadoDerecho();
}