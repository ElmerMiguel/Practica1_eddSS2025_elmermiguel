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
    cout << "\n🎮 TABLERO " << filas << "x" << columnas << " 🎮" << endl;
    
    // === NÚMEROS DE COLUMNA ===
    cout << "     ";
    for (int j = 0; j < columnas; j++) {
        cout << j << "     ";
    }
    cout << endl;

    // === LÍNEA SUPERIOR DE NODOS ===
    cout << "   ";
    for (int j = 0; j < columnas; j++) {
        cout << "+";
        
        // Línea horizontal hacia la derecha (solo si hay línea marcada)
        if (j < columnas - 1) {
            // Verificar si hay línea horizontal entre columnas j y j+1 en fila 0
            Celda* celda = obtenerCelda(0, j);
            if (celda != nullptr && celda->getLadoDerecho()) {
                cout << "-----";
            } else {
                cout << "     ";
            }
        }
    }
    cout << endl;

    // === FILAS CON NODOS Y CONTENIDO ===
    for (int i = 0; i < filas; i++) {
        // Número de fila + nodos con líneas verticales
        cout << i << " ";
        
        for (int j = 0; j < columnas; j++) {
            cout << "+";
            
            // Línea horizontal hacia la derecha
            if (j < columnas - 1) {
                Celda* celda = obtenerCelda(i, j);
                if (celda != nullptr && celda->getLadoDerecho()) {
                    cout << "-----";
                } else {
                    cout << "     ";
                }
            }
        }
        cout << endl;

        // === LÍNEA CON CONTENIDO DE CELDAS ===
        if (i < filas) {
            // Líneas verticales y contenido
            cout << "  ";
            
            for (int j = 0; j < columnas; j++) {
                // Línea vertical hacia abajo
                Celda* celda = obtenerCelda(i, j);
                if (celda != nullptr && celda->getLadoInferior()) {
                    cout << "|";
                } else {
                    cout << " ";
                }

                // Contenido de la celda (5 espacios)
                if (j < columnas) {
                    if (celda != nullptr) {
                        char propietario = celda->getPropietario();
                        string powerUp = celda->getPowerUp();
                        
                        if (propietario != ' ') {
                            cout << "  " << propietario << "  ";
                        } else if (!powerUp.empty() && powerUp != " ") {
                            if (powerUp.length() >= 2) {
                                cout << " " << powerUp.substr(0, 2) << " ";
                            } else {
                                cout << "  " << powerUp << "  ";
                            }
                        } else {
                            cout << "     ";  // Celda vacía
                        }
                    } else {
                        cout << "     ";
                    }
                }
            }
            cout << endl;
            
            // Línea en blanco para separación
            if (i < filas - 1) {
                cout << endl;
            }
        }
    }

    cout << "\n💡 Leyenda: Letras = Jugadores | Símbolos = PowerUps" << endl;
    cout << "🎯 Controles: W=↑ A=← S=↓ D=→" << endl;
}
Celda* Tablero::obtenerCelda(int fila, int columna) {
    return celdas->obtener(fila, columna);
}



bool Tablero::marcarLinea(int fila, int columna, char lado, char jugador) {
    // Validar coordenadas
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas) {
        cout << "❌ Coordenadas fuera de rango." << endl;
        return false;
    }
    
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) {
        cout << "❌ Celda no válida." << endl;
        return false;
    }
    
    // Verificar que la línea no esté ya marcada
    bool yaExiste = false;
    switch(lado) {
        case 'S': // W -> Superior (línea horizontal arriba)
            yaExiste = celda->getLadoSuperior();
            break;
        case 'I': // S -> Inferior (línea horizontal abajo)
            yaExiste = celda->getLadoInferior();
            break;
        case 'L': // A -> Izquierdo (línea vertical izquierda)
            yaExiste = celda->getLadoIzquierdo();
            break;
        case 'D': // D -> Derecho (línea vertical derecha)
            yaExiste = celda->getLadoDerecho();
            break;
        default:
            cout << "❌ Lado inválido: " << lado << endl;
            return false;
    }
    
    if (yaExiste) {
        cout << "❌ Esta línea ya está marcada." << endl;
        return false;
    }
    
    // Marcar la línea
    switch(lado) {
        case 'S': // W -> Superior
            celda->setLadoSuperior(true);
            break;
        case 'I': // S -> Inferior
            celda->setLadoInferior(true);
            break;
        case 'L': // A -> Izquierdo
            celda->setLadoIzquierdo(true);
            break;
        case 'D': // D -> Derecho
            celda->setLadoDerecho(true);
            break;
    }
    
    cout << "✅ Línea marcada en (" << fila << "," << columna << ") lado " << lado << endl;
    return true;
}




bool Tablero::verificarCuadradoCompleto(int fila, int columna) {
    Celda* celda = obtenerCelda(fila, columna);
    if (celda == nullptr) return false;
    
    // Un cuadrado está completo si tiene los 4 lados
    return celda->getLadoSuperior() && 
           celda->getLadoInferior() && 
           celda->getLadoIzquierdo() && 
           celda->getLadoDerecho();
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



void Tablero::mostrarPuntosDisponibles() {
    cout << "\n═══════ PUNTOS DISPONIBLES PARA ELIMINAR ═══════" << endl;
    cout << "Coordenadas válidas (fila, columna):" << endl;
    
    for (int i = 0; i <= filas; i++) {
        for (int j = 0; j <= columnas; j++) {
            cout << "(" << i << "," << j << ") ";
            if ((j + 1) % 6 == 0) cout << endl; 
        }
    }
    cout << endl;
}

bool Tablero::puntoEsValido(int fila, int columna) {
    return (fila >= 0 && fila <= filas && columna >= 0 && columna <= columnas);
}

ArregloT<char> Tablero::obtenerJugadoresAfectados(int fila, int columna) {
    ArregloT<char> jugadoresAfectados;
    
    // Verificar cuadrados adyacentes al punto
    // Cuadrado superior-izquierdo: (fila-1, columna-1)
    if (fila > 0 && columna > 0) {
        Celda* celda = obtenerCelda(fila - 1, columna - 1);
        if (celda != nullptr && celda->estaCompleta() && celda->getPropietario() != ' ') {
            if (!jugadoresAfectados.contiene(celda->getPropietario())) {
                jugadoresAfectados.agregar(celda->getPropietario());
            }
        }
    }
    
    // Cuadrado superior-derecho: (fila-1, columna)
    if (fila > 0 && columna < columnas) {
        Celda* celda = obtenerCelda(fila - 1, columna);
        if (celda != nullptr && celda->estaCompleta() && celda->getPropietario() != ' ') {
            if (!jugadoresAfectados.contiene(celda->getPropietario())) {
                jugadoresAfectados.agregar(celda->getPropietario());
            }
        }
    }
    
    // Cuadrado inferior-izquierdo: (fila, columna-1)
    if (fila < filas && columna > 0) {
        Celda* celda = obtenerCelda(fila, columna - 1);
        if (celda != nullptr && celda->estaCompleta() && celda->getPropietario() != ' ') {
            if (!jugadoresAfectados.contiene(celda->getPropietario())) {
                jugadoresAfectados.agregar(celda->getPropietario());
            }
        }
    }
    
    // Cuadrado inferior-derecho: (fila, columna)
    if (fila < filas && columna < columnas) {
        Celda* celda = obtenerCelda(fila, columna);
        if (celda != nullptr && celda->estaCompleta() && celda->getPropietario() != ' ') {
            if (!jugadoresAfectados.contiene(celda->getPropietario())) {
                jugadoresAfectados.agregar(celda->getPropietario());
            }
        }
    }
    
    return jugadoresAfectados;
}

void Tablero::eliminarPunto(int fila, int columna) {
    if (!puntoEsValido(fila, columna)) {
        cout << "❌ Punto inválido: (" << fila << "," << columna << ")" << endl;
        return;
    }
    
    cout << "💥 ELIMINANDO PUNTO (" << fila << "," << columna << ")" << endl;
    
    // Obtener jugadores afectados ANTES de eliminar
    ArregloT<char> jugadoresAfectados = obtenerJugadoresAfectados(fila, columna);
    
    // Mostrar jugadores afectados
    if (jugadoresAfectados.getTamaño() > 0) {
        cout << "⚠️  JUGADORES AFECTADOS: ";
        for (int i = 0; i < jugadoresAfectados.getTamaño(); i++) {
            cout << jugadoresAfectados[i];
            if (i < jugadoresAfectados.getTamaño() - 1) cout << ", ";
        }
        cout << endl;
        cout << "Sus cuadrados perderán dueño pero mantendrán los puntos." << endl;
    } else {
        cout << "✅ No hay jugadores afectados por esta eliminación." << endl;
    }
    
    // Eliminar propietarios de cuadrados afectados
    // Cuadrado superior-izquierdo
    if (fila > 0 && columna > 0) {
        Celda* celda = obtenerCelda(fila - 1, columna - 1);
        if (celda != nullptr && celda->estaCompleta()) {
            cout << "   - Cuadrado (" << (fila-1) << "," << (columna-1) << ") pierde dueño" << endl;
            celda->setPropietario(' '); // Quitar dueño pero mantener cuadrado
        }
    }
    
    // Cuadrado superior-derecho
    if (fila > 0 && columna < columnas) {
        Celda* celda = obtenerCelda(fila - 1, columna);
        if (celda != nullptr && celda->estaCompleta()) {
            cout << "   - Cuadrado (" << (fila-1) << "," << columna << ") pierde dueño" << endl;
            celda->setPropietario(' ');
        }
    }
    
    // Cuadrado inferior-izquierdo
    if (fila < filas && columna > 0) {
        Celda* celda = obtenerCelda(fila, columna - 1);
        if (celda != nullptr && celda->estaCompleta()) {
            cout << "   - Cuadrado (" << fila << "," << (columna-1) << ") pierde dueño" << endl;
            celda->setPropietario(' ');
        }
    }
    
    // Cuadrado inferior-derecho
    if (fila < filas && columna < columnas) {
        Celda* celda = obtenerCelda(fila, columna);
        if (celda != nullptr && celda->estaCompleta()) {
            cout << "   - Cuadrado (" << fila << "," << columna << ") pierde dueño" << endl;
            celda->setPropietario(' ');
        }
    }
    
    cout << "💥 Punto eliminado exitosamente." << endl;
}