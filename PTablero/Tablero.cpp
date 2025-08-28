#include "Tablero.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Tablero::Tablero(int filas, int columnas) : filas(filas), columnas(columnas) {
    celdas = new ListaT();
    gestorPowers = new GestorPowerUps();  
    inicializar();
    generarPowerUpsAleatorios();  
}

// DESTRUCTOR
Tablero::~Tablero() {
    delete celdas;
    delete gestorPowers; 
    celdas = nullptr;
    gestorPowers = nullptr;
}

// COPY CONSTRUCTOR
Tablero::Tablero(const Tablero& otro) : filas(otro.filas), columnas(otro.columnas) {
    celdas = new ListaT(*otro.celdas);
    gestorPowers = new GestorPowerUps(*otro.gestorPowers); 
}

// ASSIGNMENT OPERATOR
Tablero& Tablero::operator=(const Tablero& otro) {
    if (this != &otro) {
        filas = otro.filas;
        columnas = otro.columnas;
        
        delete celdas;
        delete gestorPowers; 
        
        celdas = new ListaT(*otro.celdas);
        gestorPowers = new GestorPowerUps(*otro.gestorPowers);  
    }
    return *this;
}

void Tablero::inicializar() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            Celda* nuevaCelda = new Celda(i, j);
            celdas->insertar(nuevaCelda);
        }
    }
}

void Tablero::generarPowerUpsAleatorios() {
    srand(time(nullptr));
    
    // Generar PowerUps en algunas celdas (30% de probabilidad)
    NodoLista* actual = celdas->getPrimero();
    while (actual != nullptr) {
        if (rand() % 100 < 30) {  // 30% probabilidad
            PowerUp* powerUp = PowerUp::crearPowerUpAleatorio();
            actual->celda->setPowerUp(powerUp->getSimbolo());
        }
        actual = actual->siguiente;
    }
    
    cout << "PowerUps generados aleatoriamente en el tablero!" << endl;
}

void Tablero::mostrarTablero() {
    cout << "\n=== TABLERO " << filas << "x" << columnas << " ===" << endl;
    
    // Mostrar información de las celdas
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            Celda* celda = obtenerCelda(i, j);
            if (celda != nullptr) {
                cout << "(" << i << "," << j << ")";
                if (!celda->getPowerUp().empty()) {
                    cout << "[" << celda->getPowerUp() << "]";
                }
                if (celda->getPropietario() != ' ') {
                    cout << "{" << celda->getPropietario() << "}";
                }
                cout << " ";
            }
        }
        cout << endl;
    }
    
    // Mostrar efectos activos
    gestorPowers->mostrarEfectosActivos();
}

Celda* Tablero::obtenerCelda(int fila, int columna) {
    return celdas->obtener(fila, columna);
}



bool Tablero::marcarLinea(int fila, int columna, char lado, char jugador) {
    // Verificar si la línea está bloqueada
    if (gestorPowers->lineaBloqueada(fila, columna, lado)) {
        cout << "¡Esta línea está bloqueada!" << endl;
        return false;
    }
    
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) return false;
    
    // Verificar si ya está marcada
    bool yaEstaCapturada = false;
    switch (lado) {
        case 'S': case 's':
            yaEstaCapturada = celda->getLadoSuperior();
            break;
        case 'I': case 'i':
            yaEstaCapturada = celda->getLadoInferior();
            break;
        case 'L': case 'l':
            yaEstaCapturada = celda->getLadoIzquierdo();
            break;
        case 'D': case 'd':
            yaEstaCapturada = celda->getLadoDerecho();
            break;
    }
    
    if (yaEstaCapturada) {
        cout << "Esta línea ya está marcada!" << endl;
        return false;
    }
    
    // Marcar la lnea en ambas celdas adyacentes
    bool marcadoExitoso = false;
    
    switch (lado) {
        case 'S': case 's': {
            // Marcar lado superior de la celda actual
            celda->setLadoSuperior(true);
            marcadoExitoso = true;
            
            // Marcar lado inferior de la celda de arriba (fila-1)
            if (fila > 0) {
                Celda* celdaArriba = obtenerCelda(fila - 1, columna);
                if (celdaArriba != nullptr) {
                    celdaArriba->setLadoInferior(true);
                }
            }
            break;
        }
        case 'I': case 'i': {
            // Marcar lado inferior de la celda actual
            celda->setLadoInferior(true);
            marcadoExitoso = true;
            
            // Marcar lado superior de la celda de abajo (fila+1)
            if (fila < filas - 1) {
                Celda* celdaAbajo = obtenerCelda(fila + 1, columna);
                if (celdaAbajo != nullptr) {
                    celdaAbajo->setLadoSuperior(true);
                }
            }
            break;
        }
        case 'L': case 'l': {
            // Marcar lado izquierdo de la celda actual
            celda->setLadoIzquierdo(true);
            marcadoExitoso = true;
            
            // Marcar lado derecho de la celda de la izquierda (columna-1)
            if (columna > 0) {
                Celda* celdaIzquierda = obtenerCelda(fila, columna - 1);
                if (celdaIzquierda != nullptr) {
                    celdaIzquierda->setLadoDerecho(true);
                }
            }
            break;
        }
        case 'D': case 'd': {
            // Marcar lado derecho de la celda actual
            celda->setLadoDerecho(true);
            marcadoExitoso = true;
            
            // Marcar lado izquierdo de la celda de la derecha (columna+1)
            if (columna < columnas - 1) {
                Celda* celdaDerecha = obtenerCelda(fila, columna + 1);
                if (celdaDerecha != nullptr) {
                    celdaDerecha->setLadoIzquierdo(true);
                }
            }
            break;
        }
        default:
            return false;
    }
    
    // Verificar si hay trampa
    if (gestorPowers->lineaConTrampa(fila, columna, lado)) {
        cout << "¡TRAMPA ACTIVADA! El jugador que la puso gana puntos!" << endl;
    }
    
    return marcadoExitoso;
}




bool Tablero::verificarCuadradoCompleto(int fila, int columna) {
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) return false;
    
    return celda->estaCompleta();
}

bool Tablero::usarPowerUp(PowerUp* powerUp, int fila, int columna, char lado, char jugador) {
    return gestorPowers->aplicarEfecto(powerUp, fila, columna, lado, jugador);
}

void Tablero::procesarFinTurno() {
    gestorPowers->procesarFinTurno();
}

void Tablero::mostrarEfectosActivos() {
    gestorPowers->mostrarEfectosActivos();
}



bool Tablero::tableroCompleto() {
    // Un tablero está completo cuando todas las líneas posibles están marcadas
    int totalLineas = contarLineasTotales();
    int lineasMarcadas = contarLineasMarcadas();
    
    cout << "Progreso: " << lineasMarcadas << "/" << totalLineas << " líneas marcadas" << endl;
    
    return lineasMarcadas >= totalLineas;
}

int Tablero::contarLineasTotales() {
    // En un tablero de FxC hay:
    // - Líneas horizontales: F * (C-1) + (F-1) * C
    // - Líneas verticales: (F-1) * C + F * (C-1)
    // Simplificado: 2*F*C - F - C
    return 2 * filas * columnas - filas - columnas;
}

int Tablero::contarLineasMarcadas() {
    int contador = 0;
    
    NodoLista* actual = celdas->getPrimero();
    while (actual != nullptr) {
        Celda* celda = actual->celda;
        int fila = celda->getFila();
        int columna = celda->getColumna();
        
        // Contar líneas que no se duplican con celdas adyacentes
        // Solo contamos líneas superior e izquierda para evitar duplicados
        
        // Línea superior (solo si no es primera fila)
        if (fila > 0 && celda->getLadoSuperior()) {
            contador++;
        }
        
        // Línea izquierda (solo si no es primera columna)
        if (columna > 0 && celda->getLadoIzquierdo()) {
            contador++;
        }
        
        // Líneas del borde derecho (solo última columna)
        if (columna == columnas - 1 && celda->getLadoDerecho()) {
            contador++;
        }
        
        // Líneas del borde inferior (solo última fila)
        if (fila == filas - 1 && celda->getLadoInferior()) {
            contador++;
        }
        
        actual = actual->siguiente;
    }
    
    return contador;
}






bool Tablero::puedeExpandir() const {
    // Solo expandir si el tablero es menor a 8x8
    return (filas < 8 && columnas < 8);
}

void Tablero::expandirTablero() {
    if (!puedeExpandir()) {
        cout << "❌ No se puede expandir más el tablero (máximo 8x8)." << endl;
        return;
    }
    
    cout << "🚀 ¡EXPANDIENDO TABLERO!" << endl;
    cout << "Tablero anterior: " << filas << "x" << columnas << endl;
    
    // Guardar datos existentes
    ListaT* celdasAnteriores = celdas;
    int filasAnteriores = filas;
    int columnasAnteriores = columnas;
    
    // Incrementar dimensiones
    filas++;
    columnas++;
    
    // Crear nueva lista de celdas
    celdas = new ListaT();
    
    // Inicializar todas las celdas nuevas
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            Celda* nuevaCelda = new Celda(i, j);
            celdas->insertar(nuevaCelda);
        }
    }
    
    // Transferir datos de celdas anteriores
    NodoLista* actualAnterior = celdasAnteriores->getPrimero();
    while (actualAnterior != nullptr) {
        Celda* celdaAnterior = actualAnterior->celda;
        int filaAnterior = celdaAnterior->getFila();
        int columnaAnterior = celdaAnterior->getColumna();
        
        // Encontrar celda correspondiente en nuevo tablero
        Celda* celdaNueva = obtenerCelda(filaAnterior, columnaAnterior);
        if (celdaNueva != nullptr) {
            // Transferir estado
            celdaNueva->setLadoSuperior(celdaAnterior->getLadoSuperior());
            celdaNueva->setLadoInferior(celdaAnterior->getLadoInferior());
            celdaNueva->setLadoIzquierdo(celdaAnterior->getLadoIzquierdo());
            celdaNueva->setLadoDerecho(celdaAnterior->getLadoDerecho());
            celdaNueva->setPropietario(celdaAnterior->getPropietario());
            celdaNueva->setPowerUp(celdaAnterior->getPowerUp());
        }
        
        actualAnterior = actualAnterior->siguiente;
    }
    
    // Limpiar datos anteriores
    delete celdasAnteriores;
    
    // Generar nuevos PowerUps en las áreas expandidas
    redistribuirPowerUps();
    
    cout << "✅ Tablero expandido a: " << filas << "x" << columnas << endl;
}

void Tablero::redistribuirPowerUps() {
    // Generar PowerUps en las nuevas filas y columnas
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            // Solo en las áreas nuevas (última fila o última columna)
            if (i == filas - 1 || j == columnas - 1) {
                if (rand() % 100 < 20) {  // 20% probabilidad en áreas nuevas
                    Celda* celda = obtenerCelda(i, j);
                    if (celda != nullptr && celda->getPowerUp().empty()) {
                        PowerUp* powerUp = PowerUp::crearPowerUpAleatorio();
                        celda->setPowerUp(powerUp->getSimbolo());
                        delete powerUp; // Solo necesitamos el símbolo
                    }
                }
            }
        }
    }
    
    cout << "🎁 Nuevos PowerUps generados en áreas expandidas!" << endl;
}