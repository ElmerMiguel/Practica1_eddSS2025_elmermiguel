#include "Celda.h"
#include <iostream>
using namespace std;

Celda::Celda(int fila, int columna) 
    : fila(fila), columna(columna), 
      ladoSuperior(false), ladoInferior(false), 
      ladoIzquierdo(false), ladoDerecho(false),
      propietario(' '), powerUp("") {}

// COPY CONSTRUCTOR
Celda::Celda(const Celda& otra) 
    : fila(otra.fila), columna(otra.columna),
      ladoSuperior(otra.ladoSuperior), ladoInferior(otra.ladoInferior),
      ladoIzquierdo(otra.ladoIzquierdo), ladoDerecho(otra.ladoDerecho),
      propietario(otra.propietario), powerUp(otra.powerUp) {}

// ASSIGNMENT OPERATOR
Celda& Celda::operator=(const Celda& otra) {
    if (this != &otra) {  // Evitar auto-asignación
        fila = otra.fila;
        columna = otra.columna;
        ladoSuperior = otra.ladoSuperior;
        ladoInferior = otra.ladoInferior;
        ladoIzquierdo = otra.ladoIzquierdo;
        ladoDerecho = otra.ladoDerecho;
        propietario = otra.propietario;
        powerUp = otra.powerUp;
    }
    return *this;
}

bool Celda::estaCompleta() const {
    return ladoSuperior && ladoInferior && ladoIzquierdo && ladoDerecho;
}

void Celda::mostrarInfo() const {
    cout << "Celda (" << fila << "," << columna << ") - ";
    cout << "S:" << (ladoSuperior ? "X" : "O") << " ";
    cout << "I:" << (ladoInferior ? "X" : "O") << " ";
    cout << "L:" << (ladoIzquierdo ? "X" : "O") << " ";
    cout << "D:" << (ladoDerecho ? "X" : "O") << " ";
    cout << "Prop:" << propietario << " ";
    if (!powerUp.empty()) {
        cout << "Power:" << powerUp;
    }
    cout << endl;
}