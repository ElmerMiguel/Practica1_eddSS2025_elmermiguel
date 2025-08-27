#include "PilaPower.h"
#include <iostream>
using namespace std;

PilaPower::PilaPower() : tope(nullptr), tamaño(0) {}

// DESTRUCTOR - Libera TODA la memoria
PilaPower::~PilaPower() {
    while (!estaVacia()) {
        NodoPila* temp = tope;
        tope = tope->siguiente;
        delete temp->powerUp;  // Libera el PowerUp
        delete temp;           // Libera el nodo
        tamaño--;
    }
    tope = nullptr;
}

// COP CONSTRUCTOR
PilaPower::PilaPower(const PilaPower& otra) : tope(nullptr), tamaño(0) {
    if (otra.tope == nullptr) return;
    
    // Ivertir para el orden
    NodoPila* temp = nullptr;
    NodoPila* actual = otra.tope;
    
    // pila temporal invertida
    while (actual != nullptr) {
        PowerUp* nuevoPower = new PowerUp(*actual->powerUp);  // Copy constructor de PowerUp
        NodoPila* nuevoNodo = new NodoPila(nuevoPower);
        nuevoNodo->siguiente = temp;
        temp = nuevoNodo;
        actual = actual->siguiente;
    }
    
    // volver a invertir para orden original
    while (temp != nullptr) {
        NodoPila* siguiente = temp->siguiente;
        temp->siguiente = tope;
        tope = temp;
        temp = siguiente;
        tamaño++;
    }
}

// ASIG OPERATOR
PilaPower& PilaPower::operator=(const PilaPower& otra) {
    if (this != &otra) {  // Evitar auto-asignación
        // Limpiar memoria actual
        while (!estaVacia()) {
            NodoPila* temp = tope;
            tope = tope->siguiente;
            delete temp->powerUp;
            delete temp;
            tamaño--;
        }
        
        // Reinicializar
        tope = nullptr;
        tamaño = 0;
        
        //copiar
        if (otra.tope != nullptr) {
            // invertir para el orden
            NodoPila* temp = nullptr;
            NodoPila* actual = otra.tope;
            
            // pila temporal
            while (actual != nullptr) {
                PowerUp* nuevoPower = new PowerUp(*actual->powerUp);
                NodoPila* nuevoNodo = new NodoPila(nuevoPower);
                nuevoNodo->siguiente = temp;
                temp = nuevoNodo;
                actual = actual->siguiente;
            }
            
            // invertir de vuelta 
            while (temp != nullptr) {
                NodoPila* siguiente = temp->siguiente;
                temp->siguiente = tope;
                tope = temp;
                temp = siguiente;
                tamaño++;
            }
        }
    }
    return *this;
}

void PilaPower::push(PowerUp* powerUp) {
    NodoPila* nuevo = new NodoPila(powerUp);
    nuevo->siguiente = tope;
    tope = nuevo;
    tamaño++;
}

PowerUp* PilaPower::pop() {
    if (estaVacia()) {
        return nullptr;
    }
    
    PowerUp* powerUpExtraido = tope->powerUp;
    NodoPila* temp = tope;
    tope = tope->siguiente;
    delete temp;  // Solo elimina el nodo, no el PowerUp
    tamaño--;
    
    return powerUpExtraido;
}

PowerUp* PilaPower::top() {
    return estaVacia() ? nullptr : tope->powerUp;
}

bool PilaPower::estaVacia() {
    return tope == nullptr;
}

void PilaPower::mostrar() {
    if (estaVacia()) {
        cout << "Pila vacía" << endl;
        return;
    }
    
    cout << "PowerUps (tope -> base): ";
    NodoPila* actual = tope;
    while (actual != nullptr) {
        cout << actual->powerUp->getTipo() << " -> ";
        actual = actual->siguiente;
    }
    cout << "null" << endl;
}