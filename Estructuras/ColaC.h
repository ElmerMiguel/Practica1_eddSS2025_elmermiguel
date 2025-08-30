#ifndef COLAC_H
#define COLAC_H

#include "../Objetos/Jugador.h"

class NodoCola {
public:
    Jugador* jugador;
    NodoCola* siguiente;
    
    NodoCola(Jugador* j) : jugador(j), siguiente(nullptr) {}
};

class ColaC {
private:
    NodoCola* frente;
    NodoCola* final;
    int tamaño;

public:
    ColaC();
    
    // Regla de los 3 - NUEVO
    ~ColaC();                              // Destructor
    ColaC(const ColaC& otra);             // Copy constructor
    ColaC& operator=(const ColaC& otra);  // Assignment operator
    
    void encolar(Jugador* jugador);
    Jugador* desencolar();
    Jugador* frente_cola();
    bool estaVacia();
    void mostrar();
    int getTamaño() const { return tamaño; }
    
  
    void moverAlFrente(Jugador* jugador);  // Para trampa secreta
    void moverAlFinal(Jugador* jugador);   // Para pase
};

#endif