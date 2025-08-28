#include "Jugador.h"
#include <iostream>
using namespace std;

Jugador::Jugador(const string& nombre, char inicial) : nombre(nombre), inicial(inicial), puntos(0), 
                                                       protegidoContraTrampas(false), turnosEscurridizoRestantes(0),
                                                       totalCuadrados(0), powerUpsUsados(0), islaMasGrande(0) {
    poderes = new PilaPower();
    
    // Inicializar arreglos
    for (int i = 0; i < 10; i++) {
        cuadradosPorFila[i] = 0;
        cuadradosPorColumna[i] = 0;
    }
}

// DESTRUCTOR
Jugador::~Jugador() {
    delete poderes;
    poderes = nullptr;
}

// COPY CONSTRUCTOR
Jugador::Jugador(const Jugador& otro) : nombre(otro.nombre), inicial(otro.inicial), puntos(otro.puntos),
                                        protegidoContraTrampas(otro.protegidoContraTrampas),
                                        turnosEscurridizoRestantes(otro.turnosEscurridizoRestantes),
                                        totalCuadrados(otro.totalCuadrados), powerUpsUsados(otro.powerUpsUsados),
                                        islaMasGrande(otro.islaMasGrande) {
    poderes = new PilaPower(*otro.poderes);
    
    for (int i = 0; i < 10; i++) {
        cuadradosPorFila[i] = otro.cuadradosPorFila[i];
        cuadradosPorColumna[i] = otro.cuadradosPorColumna[i];
    }
}

// ASSIGNMENT OPERATOR
Jugador& Jugador::operator=(const Jugador& otro) {
    if (this != &otro) {
        nombre = otro.nombre;
        inicial = otro.inicial;
        puntos = otro.puntos;
        protegidoContraTrampas = otro.protegidoContraTrampas;
        turnosEscurridizoRestantes = otro.turnosEscurridizoRestantes;
        
        delete poderes;
        poderes = new PilaPower(*otro.poderes);
    }
    return *this;
}

void Jugador::agregarPowerUp(PowerUp* power) {
    if (power != nullptr) {
        poderes->push(power);
    }
}

PowerUp* Jugador::usarPowerUp() {
    return poderes->pop();
}

bool Jugador::tienePowerUps() const {
    return !poderes->estaVacia();
}

void Jugador::mostrarPowerUps() const {
    cout << "PowerUps de " << nombre << ": ";
    poderes->mostrar();
}

void Jugador::mostrarInfo() const {
    cout << "Jugador: " << nombre << " (" << inicial << ") | Puntos: " << puntos 
         << " | PowerUps: " << poderes->getTamaño();
    if (tieneEfectoEscurridizo()) {
        cout << " | 🛡️Protegido(" << turnosEscurridizoRestantes << ")";
    }
    cout << endl;
}


void Jugador::restarPuntos(int cantidad) {
    puntos -= cantidad;
    if (puntos < 0) puntos = 0;  // No permitir puntos negativos
    cout << "¡" << nombre << " perdió " << cantidad << " punto(s)! Puntos actuales: " << puntos << endl;
}

void Jugador::duplicarUltimoPunto() {
    if (puntos > 0) {
        puntos++;
        cout << "¡" << nombre << " recibió punto doble por Unión a Futuro! Puntos: " << puntos << endl;
    }
}

bool Jugador::tieneEfectoEscurridizo() const {
    return protegidoContraTrampas && turnosEscurridizoRestantes > 0;
}

void Jugador::activarEscurridizo(int turnos) {
    protegidoContraTrampas = true;
    turnosEscurridizoRestantes = turnos;
    cout << "¡" << nombre << " está protegido contra trampas por " << turnos << " turnos!" << endl;
}



void Jugador::registrarCuadrado(int fila, int columna) {
    totalCuadrados++;
    cuadradosPorFila[fila]++;
    cuadradosPorColumna[columna]++;
}

void Jugador::registrarPowerUpUsado() {
    powerUpsUsados++;
}

int Jugador::getCuadradosFila(int fila) const {
    return (fila < 10) ? cuadradosPorFila[fila] : 0;
}

int Jugador::getCuadradosColumna(int columna) const {
    return (columna < 10) ? cuadradosPorColumna[columna] : 0;
}

int Jugador::getTotalCuadrados() const {
    return totalCuadrados;
}

int Jugador::getPowerUpsUsados() const {
    return powerUpsUsados;
}

void Jugador::calcularIslaMasGrande(Tablero* tablero) {
    // Implementación básica - puede mejorarse
    islaMasGrande = totalCuadrados; // Por ahora, asumir que todos están conectados
}

int Jugador::getIslaMasGrande() const {
    return islaMasGrande;
}




void Jugador::procesarFinTurnoJugador() {
    // Marcar todos los PowerUps como "viejos" para que puedan usarse
    // Esto se implementaría si tuviéramos acceso a la pila interna
    // Por ahora, asumiremos que se maneja en el uso
    
    if (turnosEscurridizoRestantes > 0) {
        turnosEscurridizoRestantes--;
        if (turnosEscurridizoRestantes == 0) {
            protegidoContraTrampas = false;
            cout << "🛡️ Protección Escurridizo de " << nombre << " ha expirado." << endl;
        }
    }
}