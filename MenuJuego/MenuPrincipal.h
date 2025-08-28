#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include "Juego.h"
#include "../Estructuras/ArregloT.h"  // CAMBIADO: usar nuestro arreglo

class MenuPrincipal {
private:
    int filas;
    int columnas;
    int numJugadores;
    
public:
    MenuPrincipal();
    ~MenuPrincipal() = default;
    
    void mostrarBienvenida();
    void configurarJuego();
    void configurarJugadores(Juego& juego);
    void iniciarJuego();
    void mostrarMenu();
    void mostrarInstrucciones();
    void mostrarPowerUps();
    
private:
    bool validarTamaño(int filas, int columnas);
    char asignarInicialUnica(const std::string& nombre, ArregloT<char>& inicialesUsadas);  // CAMBIADO
};

#endif