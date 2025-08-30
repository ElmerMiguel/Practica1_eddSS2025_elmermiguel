#include "Jugador.h"
#include <iostream>
#include <limits>
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
    if (!tienePowerUps()) {
        cout << "❌ No tienes PowerUps para usar." << endl;
        return nullptr;
    }
    
    // Si solo hay un PowerUp disponible, úsalo automáticamente
    if (poderes->getTamaño() == 1) {
        return poderes->pop();
    }
    
    // Si hay varios PowerUps, permite seleccionar uno con validación
    cout << "\nSelecciona un PowerUp para usar:" << endl;
    
    // Mostrar PowerUps disponibles con índice
    NodoPila* actual = poderes->getTope();
    int index = 1;
    while (actual != nullptr) {
        cout << index << ". " << actual->powerUp->getTipoString() << " - " << actual->powerUp->getDescripcion() << endl;
        actual = actual->siguiente;
        index++;
    }
    
    // Solicitar selección con validación
    int seleccion = 0;
    bool entradaValida = false;
    
    do {
        cout << "Número de PowerUp (1-" << poderes->getTamaño() << "): ";
        if (!(cin >> seleccion)) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            cout << "❌ Por favor, ingresa un número válido." << endl;
            continue;
        }
        
        // Validar rango
        if (seleccion < 1 || seleccion > poderes->getTamaño()) {
            cout << "❌ Número fuera de rango. Debe estar entre 1 y " << poderes->getTamaño() << "." << endl;
            continue;
        }
        
        entradaValida = true;
    } while (!entradaValida);
    
    // Extraer el PowerUp seleccionado
    return poderes->extraer(seleccion);
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
    // Solo incrementamos los contadores específicos de fila/columna
    // pero no el contador total, que se calculará de manera más precisa
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
    // En lugar de usar el contador totalCuadrados, vamos a contar 
    // los cuadrados marcados por el jugador en el tablero
    int total = 0;
    for (int i = 0; i < 10; i++) {
        total += cuadradosPorFila[i];
    }
    
    // Nota: Podríamos usar también la suma de cuadrados por columna
    // pero debemos elegir solo uno para no duplicar
    return total;
}

int Jugador::getPowerUpsUsados() const {
    return powerUpsUsados;
}

void Jugador::calcularIslaMasGrande(Tablero* tablero) {
    // Como no tenemos acceso directo a los métodos de Tablero debido a inclusiones circulares,
    // usaremos una implementación simplificada.
    
    // Por ahora, simplemente usamos el total de cuadrados calculado por los contadores de fila
    int totalCuadros = 0;
    for (int i = 0; i < 10; i++) {
        totalCuadros += cuadradosPorFila[i];
    }
    
    // Implementación básica - puede mejorarse
    islaMasGrande = totalCuadros; // Por ahora, asumir que todos están conectados
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