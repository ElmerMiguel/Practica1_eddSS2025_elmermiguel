#ifndef GESTORPOWERUPS_H
#define GESTORPOWERUPS_H

#include "PowerUp.h"
#include "../Estructuras/ListaT.h"

class EfectoActivo {
public:
    PowerUp* powerUp;
    int fila, columna;
    char lado;
    int turnosRestantes;
    char jugadorPropietario;
    
    EfectoActivo(PowerUp* p, int f, int c, char l, int turnos, char jugador)
        : powerUp(p), fila(f), columna(c), lado(l), turnosRestantes(turnos), jugadorPropietario(jugador) {}
};

class NodoEfecto {
public:
    EfectoActivo* efecto;
    NodoEfecto* siguiente;
    
    NodoEfecto(EfectoActivo* e) : efecto(e), siguiente(nullptr) {}
};

class GestorPowerUps {
private:
    NodoEfecto* efectosActivos;  // Lista de efectos activos en el tablero
    
public:
    GestorPowerUps();
    ~GestorPowerUps();
    
    
    GestorPowerUps(const GestorPowerUps& otro);           // Copy constructor
    GestorPowerUps& operator=(const GestorPowerUps& otro); // Assignment operator
    
    // Aplicar efectos de PowerUps
    bool aplicarEfecto(PowerUp* powerUp, int fila, int columna, char lado, char jugador);
    
    // Verificar si una línea está afectada por algún PowerUp
    bool lineaBloqueada(int fila, int columna, char lado);
    bool lineaConTrampa(int fila, int columna, char lado);
    bool lineaConUnionFuturo(int fila, int columna, char lado);
    
    // Gestión de turnos
    void procesarFinTurno();
    void limpiarEfectosExpirados();
    
    // Efectos específicos
    void activarBloqueo(int fila, int columna, char lado, char jugador);
    void activarTrampa(int fila, int columna, char lado, char jugador);
    void activarUnionFuturo(int fila, int columna, char lado, char jugador);
    
    // Mostrar efectos activos
    void mostrarEfectosActivos();
};

#endif
