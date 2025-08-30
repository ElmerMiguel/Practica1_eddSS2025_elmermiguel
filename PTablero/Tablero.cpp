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


#include <iostream>
#include <string>
using namespace std;

// --- Función auxiliar para centrar texto en celdas de ancho fijo ---
string centrarTexto(const string& texto, int ancho = 5) {
    if ((int)texto.length() >= ancho) return texto.substr(0, ancho); // recortar si se pasa
    int espacios = ancho - texto.length();
    int izquierda = espacios / 2;
    int derecha = espacios - izquierda;
    return string(izquierda, ' ') + texto + string(derecha, ' ');
}

void Tablero::mostrarTablero() {
    cout << "\n🎮 TABLERO " << filas << "x" << columnas << " 🎮" << endl;

    // === NÚMEROS DE COLUMNA ===
    cout << "     ";
    for (int j = 0; j < columnas; j++) {
        cout << "  " << j << "   ";
    }
    cout << endl;

    // === RECORRER FILAS ===
    for (int i = 0; i < filas; i++) {
        // --- Dibujar línea superior de las celdas ---
        cout << "   ";
        for (int j = 0; j < columnas; j++) {
            Celda* celda = obtenerCelda(i, j);

            cout << "+";
            if (celda != nullptr && celda->getLadoSuperior()) {
                cout << "-----";  // línea dibujada
            } else {
                cout << "     ";  // espacio vacío
            }
        }
        cout << "+" << endl;

        // --- Contenido de las celdas + líneas verticales ---
        cout << " " << i << " ";
        for (int j = 0; j < columnas; j++) {
            Celda* celda = obtenerCelda(i, j);

            if (celda != nullptr && celda->getLadoIzquierdo()) {
                cout << "|";  // línea dibujada
            } else {
                cout << " ";  // espacio vacío
            }

            // Contenido centrado (jugador o powerup)
            string contenido = " ";
            if (celda != nullptr) {
                char propietario = celda->getPropietario();
                string powerUp = celda->getPowerUp();

                if (propietario != ' ') {
                    contenido = string(1, propietario);
                } else if (!powerUp.empty() && powerUp != " ") {
                    contenido = powerUp.substr(0, min((size_t)2, powerUp.size()));
                }
            }
            cout << centrarTexto(contenido, 5);
        }

        // Línea derecha del último cuadro
        Celda* ultimaCelda = obtenerCelda(i, columnas - 1);
        if (ultimaCelda != nullptr && ultimaCelda->getLadoDerecho()) {
            cout << "|";
        } else {
            cout << " ";
        }
        cout << endl;
    }

    // --- Línea inferior final ---
    cout << "   ";
    for (int j = 0; j < columnas; j++) {
        Celda* celda = obtenerCelda(filas - 1, j);
        cout << "+";
        if (celda != nullptr && celda->getLadoInferior()) {
            cout << "-----";
        } else {
            cout << "     ";
        }
    }
    cout << "+" << endl;

    cout << "\n Leyenda: Letras = Jugadores | Símbolos = PowerUps" << endl;
    cout << " Controles: S=↑ I=↓ L=← D=→" << endl;
}


Celda* Tablero::obtenerCelda(int fila, int columna) {
    return celdas->obtener(fila, columna);
}




bool Tablero::marcarLinea(int fila, int columna, char lado, Jugador* actual, ColaC* jugadores) {
    // Validar coordenadas
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas) {
        cout << "Coordenadas fuera de rango." << endl;
        return false;
    }

    Celda* celda = obtenerCelda(fila, columna);
    if (!celda) {
        cout << "Celda no válida." << endl;
        return false;
    }

    // Verificar si ya estaba marcada
    bool yaExiste = false;
    switch(lado) {
        case 'S': yaExiste = celda->getLadoSuperior(); break;
        case 'I': yaExiste = celda->getLadoInferior(); break;
        case 'L': yaExiste = celda->getLadoIzquierdo(); break;
        case 'D': yaExiste = celda->getLadoDerecho(); break;
        default:
            cout << "Lado inválido: " << lado << " (usa W=↑ A=← S=↓ D=→)" << endl;
            return false;
    }
    if (yaExiste) {
        cout << "Esta línea ya está marcada." << endl;
        return false;
    }

    // --- Revisar PowerUps antes de marcar ---
    // Bloqueo
    if (gestorPowers->lineaBloqueada(fila, columna, lado)) {
        cout << "❌ Línea bloqueada por un PowerUp." << endl;
        return false;
    }

    // Trampa
    if (gestorPowers->lineaConTrampa(fila, columna, lado)) {
        procesarTrampa(fila, columna, lado, actual, jugadores);
        if (gestorPowers->jugadorTieneEscurridizo(actual->getInicial())) {
            cout << "🛡 Escurridizo activó protección. Línea segura." << endl;
        } else {
            cout << "💥 Trampa activada. Línea afectada según PowerUp." << endl;
        }
    }

    // Marcar la línea normalmente
    switch(lado) {
        case 'S':
            celda->setLadoSuperior(true);
            if (fila > 0) {
                Celda* vecina = obtenerCelda(fila-1, columna);
                if (vecina) vecina->setLadoInferior(true);
            }
            break;
        case 'I':
            celda->setLadoInferior(true);
            if (fila < filas-1) {
                Celda* vecina = obtenerCelda(fila+1, columna);
                if (vecina) vecina->setLadoSuperior(true);
            }
            break;
        case 'L':
            celda->setLadoIzquierdo(true);
            if (columna > 0) {
                Celda* vecina = obtenerCelda(fila, columna-1);
                if (vecina) vecina->setLadoDerecho(true);
            }
            break;
        case 'D':
            celda->setLadoDerecho(true);
            if (columna < columnas-1) {
                Celda* vecina = obtenerCelda(fila, columna+1);
                if (vecina) vecina->setLadoIzquierdo(true);
            }
            break;
    }

    cout << "Linea marcada en (" << fila << "," << columna << ") lado " << lado << endl;
    mostrarTablero();

    // --- Procesar cuadrados afectados ---
    procesarCuadrado(fila, columna, lado, actual);

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
    // Un tablero está completo cuando todos los cuadrados están completos
    int totalCuadrados = filas * columnas;
    int cuadradosCompletos = 0;
    
    // Verificar cada celda para ver si está completa
    NodoLista* actual = celdas->getPrimero();
    while (actual != nullptr) {
        Celda* celda = actual->celda;
        if (celda->estaCompleta()) {
            cuadradosCompletos++;
        }
        actual = actual->siguiente;
    }
    
    cout << "Progreso: " << cuadradosCompletos << "/" << totalCuadrados << " cuadrados completos" << endl;
    
    return cuadradosCompletos >= totalCuadrados;
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

void Tablero::procesarTrampa(int fila, int columna, char lado, Jugador* actual, ColaC* jugadores) {
    if (!gestorPowers->lineaConTrampa(fila, columna, lado))
        return;

    char propietarioTrampa = gestorPowers->obtenerPropietarioTrampa(fila, columna, lado);
    if (propietarioTrampa == ' ' || gestorPowers->jugadorTieneEscurridizo(actual->getInicial()))
        return;

    cout << "💥 ¡TRAMPA ACTIVADA por jugador " << propietarioTrampa << "!" << endl;

    // Buscar al trapero
    Jugador* trapero = nullptr;
    int posiciones = 0;
    Jugador* busqueda = jugadores->frente_cola();
    Jugador* inicial = busqueda;

    do {
        if (busqueda->getInicial() == propietarioTrampa) {
            trapero = busqueda;
            break;
        }
        jugadores->encolar(jugadores->desencolar());
        busqueda = jugadores->frente_cola();
        posiciones++;
    } while (busqueda != inicial && posiciones < 10);

    if (trapero != nullptr && trapero != actual) {
        Jugador* extraido = jugadores->desencolar();
        while (extraido != trapero && posiciones > 0) {
            jugadores->encolar(extraido);
            extraido = jugadores->desencolar();
            posiciones--;
        }
        jugadores->encolar(extraido); // Trapero al frente después del turno actual
        cout << "🎯 " << trapero->getNombre() << " se mueve al frente de la cola!" << endl;
    }
}

void Tablero::procesarCuadrado(int fila, int columna, char lado, Jugador* actual) {
    Celda* celda = obtenerCelda(fila, columna);
    if (!celda) return;
    
    // Si se completa un cuadrado con esta línea y no tiene propietario aún
    if (verificarCuadradoCompleto(fila, columna) && celda->getPropietario() == ' ') {
        cout << "🎯 ¡Cuadrado completado en (" << fila << "," << columna << ")!" << endl;
        
        // Revisar A_QUE_COSTO
        char propietarioPunto = gestorPowers->obtenerPropietarioAQueCosto(fila, columna, lado);
        if (propietarioPunto != ' ') {
            cout << "💰 ¡A Qué Costo activado!" << endl;
            
            // Registrar el cuadrado en las estadísticas del jugador actual
            actual->registrarCuadrado(fila, columna);
            celda->setPropietario(actual->getInicial()); // Marcar como propiedad del jugador
            
            // Dar punto al propietario original
            // Buscar jugador correspondiente y asignar puntos según tu cola de jugadores
        } else {
            // Comportamiento normal: asignar propietario y dar puntos
            actual->registrarCuadrado(fila, columna);
            celda->setPropietario(actual->getInicial());
            
            // UNION_FUTURO: duplicar puntos si aplica
            if (gestorPowers->lineaConUnionFuturo(fila, columna, lado)) {
                actual->duplicarUltimoPunto();
            } else {
                actual->incrementarPuntos();
            }
        }
        
        // Recoger PowerUp de la celda si hay
        if (!celda->getPowerUp().empty()) {
            cout << "🎁 ¡Recogiste un PowerUp: " << celda->getPowerUp() << "!" << endl;
            PowerUp* nuevoPower = PowerUp::crearPowerUpAleatorio();
            if (nuevoPower->getTipo() == UNION_FUTURO) nuevoPower->marcarComoReciente();
            
            actual->agregarPowerUp(nuevoPower);
            celda->setPowerUp("");
        }
    }
    
    // También revisar celdas adyacentes que pudieran completarse con la misma línea
    // Cuando se marca una línea, puede afectar a dos celdas adyacentes
    Celda* celdaAdyacente = nullptr;
    
    if (lado == 'S' && fila > 0) {
        celdaAdyacente = obtenerCelda(fila-1, columna);
        if (celdaAdyacente && verificarCuadradoCompleto(fila-1, columna) && celdaAdyacente->getPropietario() == ' ') {
            cout << "🎯 ¡Cuadrado adicional completado en (" << fila-1 << "," << columna << ")!" << endl;
            celdaAdyacente->setPropietario(actual->getInicial());
            actual->registrarCuadrado(fila-1, columna);
            actual->incrementarPuntos();
        }
    }
    else if (lado == 'I' && fila < filas-1) {
        celdaAdyacente = obtenerCelda(fila+1, columna);
        if (celdaAdyacente && verificarCuadradoCompleto(fila+1, columna) && celdaAdyacente->getPropietario() == ' ') {
            cout << "🎯 ¡Cuadrado adicional completado en (" << fila+1 << "," << columna << ")!" << endl;
            celdaAdyacente->setPropietario(actual->getInicial());
            actual->registrarCuadrado(fila+1, columna);
            actual->incrementarPuntos();
        }
    }
    else if (lado == 'L' && columna > 0) {
        celdaAdyacente = obtenerCelda(fila, columna-1);
        if (celdaAdyacente && verificarCuadradoCompleto(fila, columna-1) && celdaAdyacente->getPropietario() == ' ') {
            cout << "🎯 ¡Cuadrado adicional completado en (" << fila << "," << columna-1 << ")!" << endl;
            celdaAdyacente->setPropietario(actual->getInicial());
            actual->registrarCuadrado(fila, columna-1);
            actual->incrementarPuntos();
        }
    }
    else if (lado == 'D' && columna < columnas-1) {
        celdaAdyacente = obtenerCelda(fila, columna+1);
        if (celdaAdyacente && verificarCuadradoCompleto(fila, columna+1) && celdaAdyacente->getPropietario() == ' ') {
            cout << "🎯 ¡Cuadrado adicional completado en (" << fila << "," << columna+1 << ")!" << endl;
            celdaAdyacente->setPropietario(actual->getInicial());
            actual->registrarCuadrado(fila, columna+1);
            actual->incrementarPuntos();
        }
    }
}

