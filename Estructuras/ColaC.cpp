#include "ColaC.h"
#include <iostream>
using namespace std;

ColaC::ColaC() : frente(nullptr), final(nullptr), tamaño(0) {}

// DESTRUCTOR - Libera TODA la memoria
ColaC::~ColaC() {
    while (!estaVacia()) {
        NodoCola* temp = frente;
        frente = frente->siguiente;
        // NO eliminamos el jugador porque puede ser usado en otros lados
        // Solo eliminamos el nodo
        delete temp;
        tamaño--;
    }
    frente = nullptr;
    final = nullptr;
}

// COPY CONSTRUCTOR - Crea copia profunda
ColaC::ColaC(const ColaC& otra) : frente(nullptr), final(nullptr), tamaño(0) {
    NodoCola* actual = otra.frente;
    while (actual != nullptr) {
        encolar(actual->jugador);  // Reutilizamos el mismo jugador (referencia)
        actual = actual->siguiente;
    }
}

// ASSIGNMENT OPERATOR
ColaC& ColaC::operator=(const ColaC& otra) {
    if (this != &otra) {  // Evitar auto-asignación
        // Limpiar memoria actual
        this->~ColaC();
        
        // Reinicializar
        frente = nullptr;
        final = nullptr;
        tamaño = 0;
        
        // Copiar de la otra cola
        NodoCola* actual = otra.frente;
        while (actual != nullptr) {
            encolar(actual->jugador);
            actual = actual->siguiente;
        }
    }
    return *this;
}

void ColaC::encolar(Jugador* jugador) {
    NodoCola* nuevo = new NodoCola(jugador);
    
    if (estaVacia()) {
        frente = final = nuevo;
        nuevo->siguiente = nuevo;  // Circular
    } else {
        final->siguiente = nuevo;
        nuevo->siguiente = frente;
        final = nuevo;
    }
    tamaño++;
}

Jugador* ColaC::desencolar() {
    if (estaVacia()) {
        return nullptr;
    }
    
    Jugador* jugadorDesencolado = frente->jugador;
    
    if (tamaño == 1) {
        delete frente;
        frente = final = nullptr;
    } else {
        NodoCola* temp = frente;
        frente = frente->siguiente;
        final->siguiente = frente;
        delete temp;
    }
    
    tamaño--;
    return jugadorDesencolado;
}

Jugador* ColaC::frente_cola() {
    return estaVacia() ? nullptr : frente->jugador;
}

bool ColaC::estaVacia() {
    return frente == nullptr;
}

void ColaC::mostrar() {
    if (estaVacia()) {
        cout << "Cola vacía" << endl;
        return;
    }
    
    NodoCola* actual = frente;
    do {
        cout << actual->jugador->getNombre() << " -> ";
        actual = actual->siguiente;
    } while (actual != frente);
    cout << "(circular)" << endl;
}

// NUEVOS MÉTODOS para PowerUps
void ColaC::moverAlFrente(Jugador* jugador) {
    if (estaVacia() || frente->jugador == jugador) return;
    
    // Buscar y remover el jugador
    NodoCola* actual = frente;
    NodoCola* anterior = final;
    
    do {
        if (actual->jugador == jugador) {
            // Remover del lugar actual
            if (actual == final) {
                final = anterior;
            }
            anterior->siguiente = actual->siguiente;
            
            // Insertar al frente
            actual->siguiente = frente;
            final->siguiente = actual;
            frente = actual;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    } while (actual != frente);
}

void ColaC::moverAlFinal(Jugador* jugador) {
    if (estaVacia() || final->jugador == jugador) return;
    
    // Simplemente desencolamos y encolamos
    if (frente->jugador == jugador) {
        Jugador* j = desencolar();
        encolar(j);
    }
}