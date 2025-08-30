#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include "Juego.h"
#include "../Estructuras/ArregloT.h"  // CAMBIADO: usar nuestro arreglo

class MenuPrincipal {
private:
    int filas, columnas, numJugadores;
    Juego* juego;
    
    // AGREGAR esta declaración:
    void configurarTablero();
    void validarEntradaNumerica(int& valor, int min, int max, const std::string& mensaje);
    
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