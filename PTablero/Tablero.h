#ifndef TABLERO_H
#define TABLERO_H

#include "../Estructuras/ListaT.h"
#include "Celda.h"

class Tablero {
private:
    ListaT* celdas;
    int filas;
    int columnas;

public:
    Tablero(int filas, int columnas);
    
    // Regla de los 3
    ~Tablero();                                 // Destructor
    Tablero(const Tablero& otro);             // Copy constructor
    Tablero& operator=(const Tablero& otro);  // Assignment operator
    
    void inicializar();
    void mostrarTablero();
    Celda* obtenerCelda(int fila, int columna);
    bool marcarLinea(int fila, int columna, char lado);
    bool verificarCuadradoCompleto(int fila, int columna);
    
    // Getters
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
    ListaT* getCeldas() const { return celdas; }
};

#endif