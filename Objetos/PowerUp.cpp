#include "PowerUp.h"
#include <iostream>
using namespace std;

PowerUp::PowerUp(const string& tipo, const string& descripcion) 
    : tipo(tipo), descripcion(descripcion), usado(false) {}

// COPY CONSTRUCTOR
PowerUp::PowerUp(const PowerUp& otro) 
    : tipo(otro.tipo), descripcion(otro.descripcion), usado(otro.usado) {}

// ASSIGNMENT OPERATOR
PowerUp& PowerUp::operator=(const PowerUp& otro) {
    if (this != &otro) {  // Evitar auto-asignación
        tipo = otro.tipo;
        descripcion = otro.descripcion;
        usado = otro.usado;
    }
    return *this;
}

void PowerUp::usar() {
    usado = true;
    cout << "PowerUp '" << tipo << "' usado!" << endl;
}

void PowerUp::mostrarInfo() const {
    cout << "PowerUp: " << tipo;
    if (!descripcion.empty()) {
        cout << " - " << descripcion;
    }
    cout << " [" << (usado ? "USADO" : "DISPONIBLE") << "]" << endl;
}

// Factory method para crear los 10 PowerUps del enunciado
PowerUp* PowerUp::crearPowerUp(const string& tipo) {
    if (tipo == "PAS") {
        return new PowerUp("PAS", "Pase: Salta tu turno");
    } else if (tipo == "RET") {
        return new PowerUp("RET", "Retroceso: Regresa al jugador anterior");
    } else if (tipo == "RAP") {
        return new PowerUp("RAP", "Rapidez: Otro turno");
    } else if (tipo == "TLT") {
        return new PowerUp("TLT", "Teletransporte: Mueve linea");
    } else if (tipo == "VIS") {
        return new PowerUp("VIS", "Vision: Ve proximos 3 PowerUps");
    } else if (tipo == "ES") {
        return new PowerUp("ES", "Espejo Secreto: Trampa detectada");
    } else if (tipo == "UF") {
        return new PowerUp("UF", "Union a Futuro: Doble punto futuro");
    } else if (tipo == "AC") {
        return new PowerUp("AC", "A Que Costo: Punto pero casilla ajena");
    } else if (tipo == "NT") {
        return new PowerUp("NT", "Nuevas Tierras: Expande tablero");
    } else if (tipo == "EX") {
        return new PowerUp("EX", "Explosivos: Elimina punto");
    } else {
        return new PowerUp("UNKNOWN", "PowerUp desconocido");
    }
}