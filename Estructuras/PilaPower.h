#ifndef PILAPOWER_H
#define PILAPOWER_H

#include "../Objetos/PowerUp.h"

class NodoPila {
public:
    PowerUp* powerUp;
    NodoPila* siguiente;
    
    NodoPila(PowerUp* p) : powerUp(p), siguiente(nullptr) {}
};

class PilaPower {
private:
    NodoPila* tope;
    int tamaño;

public:
    PilaPower();
    
    // Regla de los 3
    ~PilaPower();                                // Destructor
    PilaPower(const PilaPower& otra);           // Copy constructor
    PilaPower& operator=(const PilaPower& otra); // Assignment operator
    
    void push(PowerUp* powerUp);
    PowerUp* pop();
    PowerUp* top();
    bool estaVacia();
    void mostrar();
    int getTamaño() const { return tamaño; }
};

#endif