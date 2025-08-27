#include "PowerUp.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

PowerUp::PowerUp(TipoPowerUp tipo) : tipo(tipo), usado(false), activo(false), 
                                     filaEfecto(-1), columnaEfecto(-1), ladoEfecto(' ') {
    // Inicializar simb y descrip segun el tipo
    switch(tipo) {
        case DOBLE_LINEA:
            simbolo = "DL";
            descripcion = "Permite colocar dos líneas consecutivas";
            break;
        case TRAMPA_SECRETA:
            simbolo = "TS";
            descripcion = "Línea trampa que roba puntos al enemigo";
            break;
        case BLOQUEO:
            simbolo = "BL";
            descripcion = "Bloquea una línea durante una ronda";
            break;
        case PASE:
            simbolo = "PS";
            descripcion = "Pasa turno y va al final de la cola";
            break;
        case LLAVE_SECRETA:
            simbolo = "LS";
            descripcion = "Ignora un bloqueo existente";
            break;
        case ESCURRIDIZO:
            simbolo = "ES";
            descripcion = "Protege contra trampas por una ronda";
            break;
        case UNION_FUTURO:
            simbolo = "UF";
            descripcion = "Doble punto si el mismo jugador completa después";
            break;
        case A_QUE_COSTO:
            simbolo = "AC";
            descripcion = "Punto para quien corresponde, casilla para quien puso línea";
            break;
        case NUEVAS_TIERRAS:
            simbolo = "NT";
            descripcion = "Expande el tablero hacia afuera";
            break;
        case EXPLOSIVOS:
            simbolo = "EX";
            descripcion = "Elimina un punto del mapa";
            break;
    }
}

// COPY CONSTRUCTOR
PowerUp::PowerUp(const PowerUp& otro) 
    : tipo(otro.tipo), simbolo(otro.simbolo), descripcion(otro.descripcion),
      usado(otro.usado), activo(otro.activo), filaEfecto(otro.filaEfecto),
      columnaEfecto(otro.columnaEfecto), ladoEfecto(otro.ladoEfecto) {}

// ASSIG OPERATOR
PowerUp& PowerUp::operator=(const PowerUp& otro) {
    if (this != &otro) {
        tipo = otro.tipo;
        simbolo = otro.simbolo;
        descripcion = otro.descripcion;
        usado = otro.usado;
        activo = otro.activo;
        filaEfecto = otro.filaEfecto;
        columnaEfecto = otro.columnaEfecto;
        ladoEfecto = otro.ladoEfecto;
    }
    return *this;
}

string PowerUp::getTipoString() const {
    return simbolo;
}

void PowerUp::setEfecto(int fila, int columna, char lado) {
    filaEfecto = fila;
    columnaEfecto = columna;
    ladoEfecto = lado;
    activo = true;
}

void PowerUp::usar() {
    usado = true;
    cout << "PowerUp '" << simbolo << "' activado: " << descripcion << endl;
}

void PowerUp::mostrarInfo() const {
    cout << "[" << simbolo << "] " << descripcion;
    if (usado) cout << " (USADO)";
    if (activo) cout << " (ACTIVO)";
    cout << endl;
}

bool PowerUp::afectaLinea(int fila, int columna, char lado) const {
    if (!activo) return false;
    return (filaEfecto == fila && columnaEfecto == columna && ladoEfecto == lado);
}

// FACTORY METHODS
PowerUp* PowerUp::crearPowerUpAleatorio() {
    static bool semillaInicializada = false;
    if (!semillaInicializada) {
        srand(time(nullptr));
        semillaInicializada = true;
    }
    
    TipoPowerUp tipoAleatorio = static_cast<TipoPowerUp>(rand() % 10);
    return new PowerUp(tipoAleatorio);
}

PowerUp* PowerUp::crearPowerUp(TipoPowerUp tipo) {
    return new PowerUp(tipo);
}