cmake_minimum_required(VERSION 3.30)
project(Practica1Edd)

set(CMAKE_CXX_STANDARD 20)

add_executable(Practica1Edd main.cpp
        Estructuras/ColaC.cpp
        Estructuras/ColaC.h
        Estructuras/ListaT.cpp
        Estructuras/ListaT.h
        Estructuras/PilaPower.cpp
        Estructuras/PilaPower.h
        MenuJuego/Juego.cpp
        MenuJuego/Juego.h
        Objetos/Jugador.cpp
        Objetos/Jugador.h
        Objetos/PowerUp.cpp
        Objetos/PowerUp.h
        PTablero/Celda.cpp
        PTablero/Celda.h
        PTablero/Tablero.cpp
        PTablero/Tablero.h)
