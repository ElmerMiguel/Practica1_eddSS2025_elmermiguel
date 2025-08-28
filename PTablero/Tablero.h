#ifndef TABLERO_H
#define TABLERO_H

#include "../Estructuras/ListaT.h"
#include "../Objetos/GestorPowerUps.h"
#include "Celda.h"

class Tablero {
private:
    ListaT* celdas;
    GestorPowerUps* gestorPowers; 
    int filas;
    int columnas;

public:
    Tablero(int filas, int columnas);
    
    // Regla de los 3
    ~Tablero();
    Tablero(const Tablero& otro);
    Tablero& operator=(const Tablero& otro);
    
    void inicializar();
    void mostrarTablero();
    Celda* obtenerCelda(int fila, int columna);
    bool marcarLinea(int fila, int columna, char lado, char jugador = ' ');  
    bool verificarCuadradoCompleto(int fila, int columna);
    
    // Metodos para PowerUps
    void generarPowerUpsAleatorios();
    bool usarPowerUp(PowerUp* powerUp, int fila, int columna, char lado, char jugador);
    void procesarFinTurno();
    void mostrarEfectosActivos();
    
    // verif si juego termino
    bool tableroCompleto();
    int contarLineasTotales();
    int contarLineasMarcadas();
    
    // Getters
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
    ListaT* getCeldas() const { return celdas; }
    GestorPowerUps* getGestorPowers() const { return gestorPowers; }
};

#endif
