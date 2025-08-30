#include "GestorPowerUps.h"
#include <iostream>
using namespace std;

GestorPowerUps::GestorPowerUps() : efectosActivos(nullptr), jugadoresEnRonda(0), turnosTranscurridos(0) {}

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
            
        case ESCURRIDIZO: {  // AGREGAR LLAVES
            cout << "¡Jugador " << jugador << " activó protección contra trampas!" << endl;
            cout << "Efecto: 50% probabilidad de evitar trampas por 3 turnos." << endl;
            
            PowerUp* escurridizo = PowerUp::crearPowerUp(ESCURRIDIZO);
            EfectoActivo* efecto = new EfectoActivo(escurridizo, -1, -1, ' ', 3, jugador);
            
            NodoEfecto* nuevoNodo = new NodoEfecto(efecto);
            nuevoNodo->siguiente = efectosActivos;
            efectosActivos = nuevoNodo;
            
            powerUp->usar();
            return true;
        }
            
        case DOBLE_LINEA:
            cout << "¡Jugador " << jugador << " puede colocar una línea adicional!" << endl;
            powerUp->usar();
            return true;
            
        case A_QUE_COSTO:
            activarAQueCosto(fila, columna, lado, jugador);
            powerUp->usar();
            return true;
            
        case NUEVAS_TIERRAS:
            cout << "🌍 ¡NUEVAS TIERRAS activado!" << endl;
            powerUp->usar();
            return true;
            
        case EXPLOSIVOS:
            cout << "💥 Explosivos aplicado - punto eliminado del mapa." << endl;
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
            efecto->lado == lado && efecto->turnosRestantes != 0) {
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

// La función mostrarEfectosActivos se movió a la línea ~407 con una implementación mejorada



bool GestorPowerUps::jugadorTieneEscurridizo(char jugador) {
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        if (efecto->powerUp->getTipo() == ESCURRIDIZO &&
            efecto->jugadorPropietario == jugador && 
            efecto->turnosRestantes > 0) {
            
            // 50% probabilidad
            int probabilidad = rand() % 100;
            if (probabilidad < 50) {
                cout << "🛡️ ¡Escurridizo activado! " << jugador << " evitó la trampa!" << endl;
                return true;
            } else {
                cout << "💥 Escurridizo falló. La trampa sigue activa." << endl;
                return false;
            }
        }
        actual = actual->siguiente;
    }
    return false;
}




void GestorPowerUps::activarAQueCosto(int fila, int columna, char lado, char jugador) {
    PowerUp* aQueCosto = PowerUp::crearPowerUp(A_QUE_COSTO);
    EfectoActivo* efecto = new EfectoActivo(aQueCosto, fila, columna, lado, -1, jugador);  // Permanente
    
    NodoEfecto* nuevoNodo = new NodoEfecto(efecto);
    nuevoNodo->siguiente = efectosActivos;
    efectosActivos = nuevoNodo;
    
    cout << "¡'A Qué Costo' colocado en (" << fila << "," << columna 
         << ") lado " << lado << " por jugador " << jugador << "!" << endl;
    cout << "Efecto: El punto irá para " << jugador << ", la casilla para quien complete." << endl;
}

char GestorPowerUps::obtenerPropietarioAQueCosto(int fila, int columna, char lado) {
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        if (efecto->powerUp->getTipo() == A_QUE_COSTO &&
            efecto->fila == fila && efecto->columna == columna && 
            efecto->lado == lado && efecto->turnosRestantes != 0) {
            
            // Consumir el efecto (solo se usa una vez)
            efecto->turnosRestantes = 0;
            return efecto->jugadorPropietario;
        }
        actual = actual->siguiente;
    }
    return ' '; // No hay efecto A Qué Costo
}




char GestorPowerUps::obtenerPropietarioTrampa(int fila, int columna, char lado) {
    NodoEfecto* actual = efectosActivos;
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        if (efecto->powerUp->getTipo() == TRAMPA_SECRETA &&
            efecto->fila == fila && efecto->columna == columna && 
            efecto->lado == lado && efecto->turnosRestantes != 0) {
            
            // Consumir la trampa (solo se activa una vez)
            efecto->turnosRestantes = 0;
            return efecto->jugadorPropietario;
        }
        actual = actual->siguiente;
    }
    return ' '; // No hay trampa
}




void GestorPowerUps::inicializarRonda(int numJugadores) {
    jugadoresEnRonda = numJugadores;
    turnosTranscurridos = 0;
}

void GestorPowerUps::avanzarTurno() {
    turnosTranscurridos++;
}

bool GestorPowerUps::rondaCompleta() {
    return (jugadoresEnRonda > 0 && turnosTranscurridos >= jugadoresEnRonda);
}

// MODIFICAR el método procesarFinTurno():

void GestorPowerUps::procesarFinTurno() {
    avanzarTurno();
    
    NodoEfecto* actual = efectosActivos;
    NodoEfecto* anterior = nullptr;
    
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        
        // Procesar según tipo de efecto
        if (efecto->powerUp->getTipo() == BLOQUEO) {
            // Los bloqueos duran toda la ronda
            if (rondaCompleta()) {
                cout << "🔓 Bloqueo en (" << efecto->fila << "," << efecto->columna 
                     << ") lado " << efecto->lado << " ha expirado." << endl;
                efecto->turnosRestantes = 0;
            }
        } else {
            // Otros efectos con duración normal
            if (efecto->turnosRestantes > 0) {
                efecto->turnosRestantes--;
                if (efecto->turnosRestantes == 0) {
                    cout << "⏰ Efecto " << efecto->powerUp->getSimbolo() 
                         << " ha expirado." << endl;
                }
            }
        }
        
        // Eliminar efectos expirados
        if (efecto->turnosRestantes == 0) {
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
    
    // Reiniciar contador si la ronda está completa
    if (rondaCompleta()) {
        turnosTranscurridos = 0;
        cout << "🔄 Nueva ronda iniciada." << endl;
    }
}

// MODIFICAR el método activarBloqueo():

void GestorPowerUps::activarBloqueo(int fila, int columna, char lado, char jugador) {
    PowerUp* bloqueo = PowerUp::crearPowerUp(BLOQUEO);
    // -1 indica que dura toda la ronda, no por turnos específicos
    EfectoActivo* efecto = new EfectoActivo(bloqueo, fila, columna, lado, -1, jugador);
    
    NodoEfecto* nuevoNodo = new NodoEfecto(efecto);
    nuevoNodo->siguiente = efectosActivos;
    efectosActivos = nuevoNodo;
    
    cout << "🔒 Bloqueo activado en (" << fila << "," << columna 
         << ") lado " << lado << " por " << jugador << endl;
    cout << "Duración: Toda la ronda actual." << endl;
}

void GestorPowerUps::mostrarEfectosActivos() {
    if (efectosActivos == nullptr) {
        cout << "   No hay efectos activos actualmente." << endl;
        return;
    }
    
    NodoEfecto* actual = efectosActivos;
    int contador = 0;
    
    while (actual != nullptr) {
        EfectoActivo* efecto = actual->efecto;
        contador++;
        
        cout << "   " << contador << ". ";
        
        // Mostrar información según el tipo de efecto
        switch (efecto->powerUp->getTipo()) {
            case BLOQUEO:
                cout << "🔒 BLOQUEO en (" << efecto->fila << "," << efecto->columna 
                     << ") lado " << efecto->lado 
                     << " - Dueño: " << efecto->jugadorPropietario
                     << " - Duración: Hasta fin de ronda" << endl;
                break;
                
            case TRAMPA_SECRETA:
                cout << "🪤 TRAMPA en (" << efecto->fila << "," << efecto->columna 
                     << ") lado " << efecto->lado 
                     << " - Dueño: " << efecto->jugadorPropietario 
                     << " - Duración: Permanente" << endl;
                break;
                
            case UNION_FUTURO:
                cout << "🔮 UNIÓN A FUTURO en (" << efecto->fila << "," << efecto->columna 
                     << ") lado " << efecto->lado 
                     << " - Dueño: " << efecto->jugadorPropietario
                     << " - Duración: Hasta completar cuadrado" << endl;
                break;
                
            case ESCURRIDIZO:
                cout << "🛡️ PROTECCIÓN ESCURRIDIZO para jugador " << efecto->jugadorPropietario
                     << " - Turnos restantes: " << efecto->turnosRestantes << endl;
                break;
                
            case A_QUE_COSTO:
                cout << "💰 A QUÉ COSTO en (" << efecto->fila << "," << efecto->columna
                     << ") lado " << efecto->lado 
                     << " - Beneficiario: " << efecto->jugadorPropietario
                     << " - Duración: Hasta usar" << endl;
                break;
                
            default:
                cout << "⚡ Efecto de tipo " << efecto->powerUp->getTipoString()
                     << " - Propietario: " << efecto->jugadorPropietario;
                if (efecto->turnosRestantes > 0) {
                    cout << " - Turnos restantes: " << efecto->turnosRestantes;
                } else if (efecto->turnosRestantes == -1) {
                    cout << " - Duración: Especial";
                } else {
                    cout << " - Duración: Permanente";
                }
                cout << endl;
        }
        
        actual = actual->siguiente;
    }
}