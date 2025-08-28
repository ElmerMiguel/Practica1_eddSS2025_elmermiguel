#include "GestorPowerUps.h"
#include <iostream>
using namespace std;

GestorPowerUps::GestorPowerUps() : efectosActivos(nullptr) {}

// DESTRUCTOR
GestorPowerUps::~GestorPowerUps() {
    while (efectosActivos != nullptr) {
        NodoEfecto* temp = efectosActivos;
        efectosActivos = efectosActivos->siguiente;
        delete temp->efecto->powerUp;
        delete temp->efecto;
        delete temp;
    }
}



// COPY CONSTRUCTOR
GestorPowerUps::GestorPowerUps(const GestorPowerUps& otro) : efectosActivos(nullptr) {
    // Por simplicidad, no copiamos efectos activos (son temporales)
    
}

// ASSIGNMENT OPERATOR
GestorPowerUps& GestorPowerUps::operator=(const GestorPowerUps& otro) {
    if (this != &otro) {
        // Limpiar efectos actuales
        while (efectosActivos != nullptr) {
            NodoEfecto* temp = efectosActivos;
            efectosActivos = efectosActivos->siguiente;
            delete temp->efecto->powerUp;
            delete temp->efecto;
            delete temp;
        }
        efectosActivos = nullptr;
        
        // Por simplicidad, no copiamos efectos (son temporales)
    }
    return *this;
}






bool GestorPowerUps::aplicarEfecto(PowerUp* powerUp, int fila, int columna, char lado, char jugador) {
    if (powerUp == nullptr) return false;
    
    switch (powerUp->getTipo()) {
        case BLOQUEO:
            activarBloqueo(fila, columna, lado, jugador);
            powerUp->usar();
            return true;
            
        case TRAMPA_SECRETA:
            activarTrampa(fila, columna, lado, jugador);
            powerUp->usar();
            return true;
            
        case UNION_FUTURO:
            activarUnionFuturo(fila, columna, lado, jugador);
            powerUp->usar();
            return true;
            
        case PASE:
            cout << "¡Jugador " << jugador << " pasa su turno y va al final!" << endl;
            powerUp->usar();
            return true;
            
        case LLAVE_SECRETA:
            cout << "¡Jugador " << jugador << " puede ignorar bloqueos en este turno!" << endl;
            powerUp->usar();
            return true;
            
        case ESCURRIDIZO:
            cout << "¡Jugador " << jugador << " activó protección contra trampas!" << endl;
            powerUp->usar();
            return true;
            
        case DOBLE_LINEA:
            cout << "¡Jugador " << jugador << " puede colocar una línea adicional!" << endl;
            powerUp->usar();
            return true;
            
        case A_QUE_COSTO:
            cout << "¡Efecto 'A Qué Costo' activado! Punto para el dueño, casilla para quien marca." << endl;
            powerUp->usar();
            return true;
            
        case NUEVAS_TIERRAS:
            cout << "¡El tablero se expandirá en la próxima actualización!" << endl;
            powerUp->usar();
            return true;
            
        case EXPLOSIVOS:
            cout << "¡PowerUp Explosivos activado! Se eliminará un punto al azar." << endl;
            powerUp->usar();
            return true;
            
        default:
            return false;
    }
}




bool GestorPowerUps::lineaBloqueada(int fila, int columna, char lado) {
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        if (efecto->powerUp->getTipo() == BLOQUEO &&
            efecto->fila == fila && efecto->columna == columna && 
            efecto->lado == lado && efecto->turnosRestantes > 0) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool GestorPowerUps::lineaConTrampa(int fila, int columna, char lado) {
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        if (efecto->powerUp->getTipo() == TRAMPA_SECRETA &&
            efecto->fila == fila && efecto->columna == columna && 
            efecto->lado == lado && efecto->turnosRestantes > 0) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool GestorPowerUps::lineaConUnionFuturo(int fila, int columna, char lado) {
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        if (efecto->powerUp->getTipo() == UNION_FUTURO &&
            efecto->fila == fila && efecto->columna == columna && 
            efecto->lado == lado && efecto->turnosRestantes > 0) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

void GestorPowerUps::procesarFinTurno() {
    // Reducir turnos restantes de todos los efectos
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        actual->efecto->turnosRestantes--;
        actual = actual->siguiente;
    }
    
    // Limpiar efectos expirados
    limpiarEfectosExpirados();
}

void GestorPowerUps::limpiarEfectosExpirados() {
    NodoEfecto* actual = efectosActivos;
    NodoEfecto* anterior = nullptr;
    
    while (actual != nullptr) {
        if (actual->efecto->turnosRestantes <= 0) {
            cout << "Efecto " << actual->efecto->powerUp->getSimbolo() 
                 << " en (" << actual->efecto->fila << "," << actual->efecto->columna 
                 << ") lado " << actual->efecto->lado << " ha expirado." << endl;
            
            if (anterior == nullptr) {
                efectosActivos = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            
            NodoEfecto* temp = actual;
            actual = actual->siguiente;
            delete temp->efecto->powerUp;
            delete temp->efecto;
            delete temp;
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}

void GestorPowerUps::activarBloqueo(int fila, int columna, char lado, char jugador) {
    PowerUp* bloqueo = PowerUp::crearPowerUp(BLOQUEO);
    EfectoActivo* efecto = new EfectoActivo(bloqueo, fila, columna, lado, 3, jugador);  // 3 turnos
    
    NodoEfecto* nuevoNodo = new NodoEfecto(efecto);
    nuevoNodo->siguiente = efectosActivos;
    efectosActivos = nuevoNodo;
    
    cout << "¡Línea (" << fila << "," << columna << ") lado " << lado 
         << " bloqueada por 3 turnos!" << endl;
}

void GestorPowerUps::activarTrampa(int fila, int columna, char lado, char jugador) {
    PowerUp* trampa = PowerUp::crearPowerUp(TRAMPA_SECRETA);
    EfectoActivo* efecto = new EfectoActivo(trampa, fila, columna, lado, -1, jugador);  // Permanente hasta uso
    
    NodoEfecto* nuevoNodo = new NodoEfecto(efecto);
    nuevoNodo->siguiente = efectosActivos;
    efectosActivos = nuevoNodo;
    
    cout << "¡Trampa colocada en línea (" << fila << "," << columna 
         << ") lado " << lado << "!" << endl;
}

void GestorPowerUps::activarUnionFuturo(int fila, int columna, char lado, char jugador) {
    PowerUp* union_fut = PowerUp::crearPowerUp(UNION_FUTURO);
    EfectoActivo* efecto = new EfectoActivo(union_fut, fila, columna, lado, 10, jugador);  // 10 turnos
    
    NodoEfecto* nuevoNodo = new NodoEfecto(efecto);
    nuevoNodo->siguiente = efectosActivos;
    efectosActivos = nuevoNodo;
    
    cout << "¡Unión a Futuro colocada en (" << fila << "," << columna 
         << ") lado " << lado << " por jugador " << jugador << "!" << endl;
}

void GestorPowerUps::mostrarEfectosActivos() {
    if (efectosActivos == nullptr) {
        cout << "No hay efectos activos en el tablero." << endl;
        return;
    }
    
    cout << "\n=== EFECTOS ACTIVOS ===" << endl;
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        cout << efecto->powerUp->getSimbolo() << " en (" 
             << efecto->fila << "," << efecto->columna << ") lado " 
             << efecto->lado << " - Jugador: " << efecto->jugadorPropietario;
        if (efecto->turnosRestantes > 0) {
            cout << " - Turnos restantes: " << efecto->turnosRestantes;
        } else if (efecto->turnosRestantes == -1) {
            cout << " - Permanente";
        }
        cout << endl;
        actual = actual->siguiente;
    }
    cout << "===================" << endl;
}