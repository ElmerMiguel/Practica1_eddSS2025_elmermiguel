#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "../Estructuras/PilaPower.h"

class Jugador {
private:
    std::string nombre;
    char inicial;  
    int puntos;
    PilaPower* poderes;

public:
    Jugador(const std::string& nombre, char inicial);  
    
    // Regla de los 3
    ~Jugador();
    Jugador(const Jugador& otro);
    Jugador& operator=(const Jugador& otro);
    
    // Getters y setters
    std::string getNombre() const { return nombre; }
    char getInicial() const { return inicial; }  
    int getPuntos() const { return puntos; }
    void setPuntos(int p) { puntos = p; }
    void incrementarPuntos() { puntos++; }
    
    // Manejo de PowerUps
    void agregarPowerUp(PowerUp* power);
    PowerUp* usarPowerUp();
    bool tienePowerUps() const;
    void mostrarPowerUps() const;
    
    void mostrarInfo() const;
};

#endif