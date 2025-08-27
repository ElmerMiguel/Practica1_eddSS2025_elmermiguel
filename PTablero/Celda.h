#ifndef CELDA_H
#define CELDA_H

#include <string>

class Celda {
private:
    int fila;
    int columna;
    bool ladoSuperior;
    bool ladoInferior;
    bool ladoIzquierdo;
    bool ladoDerecho;
    char propietario;
    std::string powerUp;

public:
    Celda(int fila, int columna);
    
    // Regla de los 3 (aunque es simple, por buena práctica)
    ~Celda() = default;                          // Destructor por defecto
    Celda(const Celda& otra);                   // Copy constructor
    Celda& operator=(const Celda& otra);        // Assignment operator
    
    // Getters
    int getFila() const { return fila; }
    int getColumna() const { return columna; }
    bool getLadoSuperior() const { return ladoSuperior; }
    bool getLadoInferior() const { return ladoInferior; }
    bool getLadoIzquierdo() const { return ladoIzquierdo; }
    bool getLadoDerecho() const { return ladoDerecho; }
    char getPropietario() const { return propietario; }
    std::string getPowerUp() const { return powerUp; }
    
    // Setters
    void setLadoSuperior(bool estado) { ladoSuperior = estado; }
    void setLadoInferior(bool estado) { ladoInferior = estado; }
    void setLadoIzquierdo(bool estado) { ladoIzquierdo = estado; }
    void setLadoDerecho(bool estado) { ladoDerecho = estado; }
    void setPropietario(char prop) { propietario = prop; }
    void setPowerUp(const std::string& power) { powerUp = power; }
    
    bool estaCompleta() const;
    void mostrarInfo() const;
};

#endif