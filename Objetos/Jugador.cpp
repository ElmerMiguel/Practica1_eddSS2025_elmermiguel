#include "Jugador.h"
#include <iostream>
using namespace std;

Jugador::Jugador(const string& nombre, char inicial) : nombre(nombre), inicial(inicial), puntos(0) {
    poderes = new PilaPower();
}

// DESTRUCTOR
Jugador::~Jugador() {
    delete poderes;
    poderes = nullptr;
}

// COPY CONSTRUCTOR
Jugador::Jugador(const Jugador& otro) : nombre(otro.nombre), inicial(otro.inicial), puntos(otro.puntos) {
    poderes = new PilaPower(*otro.poderes);
}

// ASSIGNMENT OPERATOR
Jugador& Jugador::operator=(const Jugador& otro) {
    if (this != &otro) {
        nombre = otro.nombre;
        inicial = otro.inicial;
        puntos = otro.puntos;
        
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
         << " | PowerUps: " << poderes->getTamaño() << endl;
}