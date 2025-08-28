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

    // Estados especiales
    bool protegidoContraTrampas;
    int turnosEscurridizoRestantes;

    // NUEVOS: Para los 7 niveles de desempate
    int cuadradosPorFila[10];     // Máximo 10 filas
    int cuadradosPorColumna[10];  // Máximo 10 columnas
    int totalCuadrados;
    int powerUpsUsados;
    int islaMasGrande;

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

    // NUEVOS: Sistema de puntuación avanzado
    void restarPuntos(int cantidad = 1);
    void duplicarUltimoPunto();
    bool tieneEfectoEscurridizo() const;
    void activarEscurridizo(int turnos = 3);

    // NUEVOS: Para los 7 niveles de desempate
    void registrarCuadrado(int fila, int columna);
    void registrarPowerUpUsado();
    int getCuadradosFila(int fila) const;
    int getCuadradosColumna(int columna) const;
    int getTotalCuadrados() const;
    int getPowerUpsUsados() const;
    void calcularIslaMasGrande(class Tablero* tablero);
    int getIslaMasGrande() const;
};

#endif
