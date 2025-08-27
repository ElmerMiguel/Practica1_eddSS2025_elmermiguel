#include "ListaT.h"
#include <iostream>
using namespace std;

ListaT::ListaT() : cabeza(nullptr), tamaño(0) {}

// DESTRUCTOR - Libera TODA la memoria
ListaT::~ListaT() {
    NodoLista* actual = cabeza;
    while (actual != nullptr) {
        NodoLista* temp = actual;
        actual = actual->siguiente;
        delete temp->celda;  // Libera la celda
        delete temp;         // Libera el nodo
    }
    cabeza = nullptr;
}

// COPY CONSTRUCTOR - Crea copia profunda
ListaT::ListaT(const ListaT& otra) : cabeza(nullptr), tamaño(0) {
    NodoLista* actual = otra.cabeza;
    while (actual != nullptr) {
        // Crear nueva celda copiando los datos
        Celda* nuevaCelda = new Celda(actual->celda->getFila(), actual->celda->getColumna());
        // Copiar el estado de la celda original
        nuevaCelda->setLadoSuperior(actual->celda->getLadoSuperior());
        nuevaCelda->setLadoInferior(actual->celda->getLadoInferior());
        nuevaCelda->setLadoIzquierdo(actual->celda->getLadoIzquierdo());
        nuevaCelda->setLadoDerecho(actual->celda->getLadoDerecho());
        nuevaCelda->setPropietario(actual->celda->getPropietario());
        nuevaCelda->setPowerUp(actual->celda->getPowerUp());
        
        insertar(nuevaCelda);
        actual = actual->siguiente;
    }
}

// ASSIGNMENT OPERATOR
ListaT& ListaT::operator=(const ListaT& otra) {
    if (this != &otra) {  // Evitar auto-asignación
        // Limpiar memoria actual
        this->~ListaT();
        
        // Reinicializar
        cabeza = nullptr;
        tamaño = 0;
        
        // Copiar de la otra lista
        NodoLista* actual = otra.cabeza;
        while (actual != nullptr) {
            Celda* nuevaCelda = new Celda(actual->celda->getFila(), actual->celda->getColumna());
            // Copiar estado...
            nuevaCelda->setLadoSuperior(actual->celda->getLadoSuperior());
            nuevaCelda->setLadoInferior(actual->celda->getLadoInferior());
            nuevaCelda->setLadoIzquierdo(actual->celda->getLadoIzquierdo());
            nuevaCelda->setLadoDerecho(actual->celda->getLadoDerecho());
            nuevaCelda->setPropietario(actual->celda->getPropietario());
            nuevaCelda->setPowerUp(actual->celda->getPowerUp());
            
            insertar(nuevaCelda);
            actual = actual->siguiente;
        }
    }
    return *this;
}

void ListaT::insertar(Celda* celda) {
    NodoLista* nuevo = new NodoLista(celda);
    if (cabeza == nullptr) {
        cabeza = nuevo;
    } else {
        NodoLista* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    tamaño++;
}

Celda* ListaT::obtener(int fila, int columna) {
    NodoLista* actual = cabeza;
    while (actual != nullptr) {
        if (actual->celda->getFila() == fila && actual->celda->getColumna() == columna) {
            return actual->celda;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void ListaT::mostrar() {
    NodoLista* actual = cabeza;
    while (actual != nullptr) {
        cout << "Celda (" << actual->celda->getFila() << ", " << actual->celda->getColumna() << ")" << endl;
        actual = actual->siguiente;
    }
}