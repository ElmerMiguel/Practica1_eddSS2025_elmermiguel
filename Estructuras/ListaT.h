#ifndef LISTAT_H
#define LISTAT_H

#include "../PTablero/Celda.h"

class NodoLista {
public:
    Celda* celda;
    NodoLista* siguiente;
    
    NodoLista(Celda* c) : celda(c), siguiente(nullptr) {}
};

class ListaT {
private:
    NodoLista* cabeza;
    int tamaño;

public:
    ListaT();
    
    // Regla de los 3 - NUEVO
    ~ListaT();                              // Destructor
    ListaT(const ListaT& otra);            // Copy constructor  
    ListaT& operator=(const ListaT& otra); // Assignment operator
    
    void insertar(Celda* celda);
    Celda* obtener(int fila, int columna);
    void mostrar();
    int getTamaño() const { return tamaño; }
    
    // Nuevo método para iterar
    NodoLista* getPrimero() const { return cabeza; }
};

#endif