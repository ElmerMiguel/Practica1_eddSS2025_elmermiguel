#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego(int filas, int columnas) : juegoTerminado(false)
{
    tablero = new Tablero(filas, columnas);
    jugadores = new ColaC();
}

// DESTRUCTOR
Juego::~Juego()
{
    delete tablero;
    delete jugadores;
    tablero = nullptr;
    jugadores = nullptr;
}

// COPY CONSTRUCTOR
Juego::Juego(const Juego &otro) : juegoTerminado(otro.juegoTerminado)
{
    tablero = new Tablero(*otro.tablero);
    jugadores = new ColaC(*otro.jugadores);
}

// ASSIGNMENT OPERATOR
Juego &Juego::operator=(const Juego &otro)
{
    if (this != &otro)
    {
        juegoTerminado = otro.juegoTerminado;

        delete tablero;
        delete jugadores;

        tablero = new Tablero(*otro.tablero);
        jugadores = new ColaC(*otro.jugadores);
    }
    return *this;
}

void Juego::agregarJugador(const Jugador &jugador)
{
    // Crear una copia del jugador para almacenar en la cola
    Jugador *nuevoJugador = new Jugador(jugador);
    jugadores->encolar(nuevoJugador);
}



void Juego::iniciar()
{
    cout << "=== INICIANDO JUEGO ===" << endl;
    

    int numJugadores = 0;
    if (!jugadores->estaVacia()) {
        Jugador* inicial = jugadores->frente_cola();
        Jugador* actual = inicial;
        do {
            numJugadores++;
            jugadores->encolar(jugadores->desencolar());
            actual = jugadores->frente_cola();
        } while (actual != inicial);
    }
    
    tablero->getGestorPowers()->inicializarRonda(numJugadores);
    cout << "Ronda inicializada con " << numJugadores << " jugadores." << endl;

    while (!juegoTerminado)
    {
        procesarTurno();

        if (verificarFinJuego())
        {
            juegoTerminado = true;
            cout << "¡Juego terminado!" << endl;
            mostrarEstado();
        }
    }
}


void Juego::procesarTurno() {
    if (jugadores->estaVacia())
        return;

    Jugador *actual = jugadores->frente_cola();

    cout << "\n=== Turno de: " << actual->getNombre()
         << " (" << actual->getInicial() << ") ===" << endl;

    mostrarTablero();
    actual->mostrarInfo();

    bool puedeColocarSegundaLinea = false;
    bool ignorarBloqueos = false;

    // -------------------- POWERUPS --------------------
    if (actual->tienePowerUps()) {
        char usarPower;
        cout << "¿Usar un PowerUp? (s/n): ";
        cin >> usarPower;

        if (usarPower == 's' || usarPower == 'S') {
            actual->mostrarPowerUps();
            PowerUp *powerUsado = actual->usarPowerUp();

            if (powerUsado != nullptr) {
                actual->registrarPowerUpUsado();

                switch (powerUsado->getTipo()) {
                    case PASE:
                        cout << "\n🎮 Usando PowerUp: PASE ⏭️" << endl;
                        tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                        cout << "\nEstado del tablero después de usar el PowerUp:" << endl;
                        mostrarTablero();
                        jugadores->encolar(jugadores->desencolar()); // Terminar turno
                        delete powerUsado;
                        tablero->procesarFinTurno();
                        return;

                    case ESCURRIDIZO:
                        cout << "\n🎮 Usando PowerUp: ESCURRIDIZO 🛡️" << endl;
                        actual->activarEscurridizo(3);
                        tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                        cout << "\nEstado del tablero después de usar el PowerUp:" << endl;
                        mostrarTablero();
                        break;

                    case DOBLE_LINEA:
                        cout << "\n🎮 Usando PowerUp: DOBLE LÍNEA ➕" << endl;
                        puedeColocarSegundaLinea = true;
                        tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                        cout << "\nEstado del tablero después de usar el PowerUp:" << endl;
                        mostrarTablero();
                        break;

                    case LLAVE_SECRETA:
                        cout << "\n🎮 Usando PowerUp: LLAVE SECRETA 🔑" << endl;
                        ignorarBloqueos = true;
                        tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                        cout << "\nEstado del tablero después de usar el PowerUp:" << endl;
                        mostrarTablero();
                        break;

                    case EXPLOSIVOS: {
                        cout << "\n🎮 Usando PowerUp: EXPLOSIVOS 💣" << endl;
                        tablero->mostrarPuntosDisponibles();
                        int filaEliminar, columnaEliminar;
                        cout << "Ingrese coordenadas del punto a eliminar (fila columna): ";
                        cin >> filaEliminar >> columnaEliminar;

                        if (tablero->puntoEsValido(filaEliminar, columnaEliminar)) {
                            tablero->eliminarPunto(filaEliminar, columnaEliminar);
                            tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                            cout << "🎯 Explosivos usado exitosamente!" << endl;
                            cout << "\nEstado del tablero después de usar el PowerUp:" << endl;
                            mostrarTablero();
                        } else {
                            cout << "❌ Coordenadas inválidas. PowerUp no consumido." << endl;
                            actual->agregarPowerUp(powerUsado);
                            powerUsado = nullptr;
                        }
                        break;
                    }

                    case NUEVAS_TIERRAS:
                        cout << "\n🎮 Usando PowerUp: NUEVAS TIERRAS 🌐" << endl;
                        if (tablero->puedeExpandir()) {
                            tablero->expandirTablero();
                            tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                            cout << "\nEstado del tablero después de usar el PowerUp:" << endl;
                            mostrarTablero();
                        } else {
                            cout << "❌ No se puede expandir más el tablero." << endl;
                            actual->agregarPowerUp(powerUsado);
                            powerUsado = nullptr;
                        }
                        break;

                    default: {
                        cout << "\n🎮 Usando PowerUp: " << powerUsado->getTipoString() << " (" << powerUsado->getDescripcion() << ")" << endl;
                        int fila, columna;
                        char lado;
                        cout << "Ingrese fila, columna y lado para aplicar el PowerUp: ";
                        cin >> fila >> columna >> lado;
                        tablero->usarPowerUp(powerUsado, fila, columna, lado, actual->getInicial());
                        cout << "\nEstado del tablero después de usar el PowerUp:" << endl;
                        mostrarTablero();
                        break;
                    }
                }

                // Manejo seguro de UNION_FUTURO
                if (powerUsado != nullptr && powerUsado->getTipo() == UNION_FUTURO) {
                    if (powerUsado->esRecienObtenido()) {
                        cout << "⏳ UNIÓN A FUTURO no se puede usar inmediatamente." << endl;
                        actual->agregarPowerUp(powerUsado);
                        powerUsado = nullptr;
                    } else {
                        powerUsado->marcarComoViejo();
                        cout << "\nEstado del tablero después de usar UNIÓN A FUTURO:" << endl;
                        mostrarTablero();
                    }
                }

                if (powerUsado != nullptr)
                    delete powerUsado;
                
                // Mostrar efectos activos después de usar PowerUp
                cout << "\n🔮 Efectos activos actualmente:" << endl;
                tablero->mostrarEfectosActivos();
            }
        }
    }

    // -------------------- MARCAR LÍNEA --------------------
    bool turnoExitoso = false;
    do {
        int fila, columna;
        char lado;
        cout << "\n📍 Selecciona donde colocar tu línea:" << endl;
        cout << "   Fila (0-" << (tablero->getFilas()-1) << "): ";
        cin >> fila;
        cout << "   Columna (0-" << (tablero->getColumnas()-1) << "): ";
        cin >> columna;
        cout << "   Dirección [W=↑ A=← S=↓ D=→]: ";
        cin >> lado;

        // Normalizar dirección
        if (lado == 'w' || lado == 'W') lado = 'S';
        else if (lado == 's' || lado == 'S') lado = 'I';
        else if (lado == 'a' || lado == 'A') lado = 'L';
        else if (lado == 'd' || lado == 'D') lado = 'D';
        else { cout << "❌ Dirección inválida.\n"; continue; }

        // Verificar bloqueos
        if (!ignorarBloqueos && tablero->getGestorPowers()->lineaBloqueada(fila, columna, lado)) {
            cout << "Esta línea está bloqueada. Intente otra." << endl;
            continue;
        }

        if (tablero->marcarLinea(fila, columna, lado, actual, jugadores)) {
            cout << "Línea marcada exitosamente!" << endl;

            // Revisar trampas
            tablero->procesarTrampa(fila, columna, lado, actual, jugadores);

            // Revisar cuadrado completo y PowerUps
            tablero->procesarCuadrado(fila, columna, lado, actual);

            turnoExitoso = true;

            // Avanzar turno si no se completó cuadrado
            if (!tablero->verificarCuadradoCompleto(fila, columna)) {
                Jugador *jugadorActual = jugadores->desencolar();
                jugadores->encolar(jugadorActual);
            }
        } else {
            cout << "Movimiento inválido. Intente de nuevo." << endl;
        }

    } while (!turnoExitoso);

    // -------------------- DOBLE LÍNEA --------------------
    if (puedeColocarSegundaLinea) {
        cout << "\n¡LÍNEA ADICIONAL! Marca otra línea:" << endl;
        int fila2, columna2;
        char lado2;
        cout << "   Fila (0-" << (tablero->getFilas()-1) << "): ";
        cin >> fila2;
        cout << "   Columna (0-" << (tablero->getColumnas()-1) << "): ";
        cin >> columna2;
        cout << "   Dirección [W=↑ A=← S=↓ D=→]: ";
        cin >> lado2;

        // Normalizar dirección
        if (lado2 == 'w' || lado2 == 'W') lado2 = 'S';
        else if (lado2 == 's' || lado2 == 'S') lado2 = 'I';
        else if (lado2 == 'a' || lado2 == 'A') lado2 = 'L';
        else if (lado2 == 'd' || lado2 == 'D') lado2 = 'D';
        else { 
            cout << "❌ Dirección inválida. PowerUp desperdiciado.\n"; 
            return;
        }

        if (tablero->marcarLinea(fila2, columna2, lado2, actual, jugadores)) {
            cout << "¡Segunda línea marcada!" << endl;
            tablero->procesarCuadrado(fila2, columna2, lado2, actual);
        }

    }

    actual->procesarFinTurnoJugador();
    tablero->procesarFinTurno();
}




void Juego::mostrarTablero()
{
    tablero->mostrarTablero();
}

void Juego::mostrarEstado()
{
    cout << "\n═══════ ESTADO FINAL ═══════" << endl;

    // Mostrar progreso del tablero
    tablero->tableroCompleto();

    // Calcular islas para cada jugador
    if (!jugadores->estaVacia())
    {
        Jugador *inicial = jugadores->frente_cola();
        Jugador *actual = inicial;

        do
        {
            actual->calcularIslaMasGrande(tablero);
            jugadores->encolar(jugadores->desencolar());
            actual = jugadores->frente_cola();
        } while (actual != inicial);
    }

    // Determinar ganador con los 7 niveles de desempate
    Jugador *ganador = determinarGanadorCompleto();

    // Mostrar estadísticas finales
    mostrarEstadisticasFinales();

    // Anunciar ganador
    if (ganador != nullptr)
    {
        cout << "\n🏆 ¡GANADOR: " << ganador->getNombre()
             << " (" << ganador->getInicial() << ")!" << endl;
        cout << "Ganó con " << ganador->getPuntos() << " puntos." << endl;
    }
    else
    {
        cout << "\n🤝 ¡EMPATE TÉCNICO! No hay ganador definido." << endl;
    }
}

bool Juego::verificarFinJuego()
{
    // Verificar si el tablero está completo
    if (tablero->tableroCompleto())
    {
        cout << "\n¡TABLERO COMPLETO! El juego ha terminado." << endl;
        return true;
    }

    // Verificar si quedan jugadores activos
    if (jugadores->estaVacia())
    {
        cout << "\n¡No quedan jugadores! El juego ha terminado." << endl;
        return true;
    }

    // Opción manual para terminar (solo para testing y depuración)
    char continuar;
    cout << "¿Continuar jugando? (s/n): ";
    cin >> continuar;
    
    if (continuar == 'n' || continuar == 'N') {
        // Verificar que realmente queremos terminar antes de completar
        if (!tablero->tableroCompleto()) {
            char confirmar;
            cout << "⚠️ El tablero aún no está completo. ¿Seguro que deseas terminar? (s/n): ";
            cin >> confirmar;
            return (confirmar == 's' || confirmar == 'S');
        }
        return true;
    }
    return false;
}

Jugador *Juego::determinarGanadorCompleto()
{
    if (jugadores->estaVacia())
        return nullptr;

    // Recopilar todos los jugadores en un arreglo para facilitar comparación
    ArregloT<Jugador *> listaJugadores;

    Jugador *inicial = jugadores->frente_cola();
    Jugador *actual = inicial;

    do
    {
        listaJugadores.agregar(actual);
        jugadores->encolar(jugadores->desencolar());
        actual = jugadores->frente_cola();
    } while (actual != inicial);

    cout << "\n═══════ PROCESO DE DESEMPATE ═══════" << endl;

    // NIVEL 1: Mayor cantidad de puntos
    cout << "🥇 NIVEL 1: Mayor cantidad de puntos" << endl;
    ArregloT<Jugador *> ganadores = filtrarPorPuntos(listaJugadores);
    mostrarCandidatos(ganadores, "puntos");

    if (ganadores.getTamaño() == 1)
    {
        cout << "✅ Ganador definido por puntos!" << endl;
        return ganadores[0];
    }

    // NIVEL 2: Mayor cantidad de cuadrados cerrados
    cout << "\n🥈 NIVEL 2: Mayor cantidad de cuadrados cerrados" << endl;
    ganadores = filtrarPorCuadrados(ganadores);
    mostrarCandidatos(ganadores, "cuadrados");

    if (ganadores.getTamaño() == 1)
    {
        cout << "✅ Ganador definido por cuadrados!" << endl;
        return ganadores[0];
    }

    // NIVEL 3: Ganador por filas
    cout << "\n🥉 NIVEL 3: Ganador por filas" << endl;
    ganadores = filtrarPorFilas(ganadores);
    mostrarCandidatos(ganadores, "filas");

    if (ganadores.getTamaño() == 1)
    {
        cout << "✅ Ganador definido por dominio de filas!" << endl;
        return ganadores[0];
    }

    // NIVEL 4: Ganador por columnas
    cout << "\n🏅 NIVEL 4: Ganador por columnas" << endl;
    ganadores = filtrarPorColumnas(ganadores);
    mostrarCandidatos(ganadores, "columnas");

    if (ganadores.getTamaño() == 1)
    {
        cout << "✅ Ganador definido por dominio de columnas!" << endl;
        return ganadores[0];
    }

    // NIVEL 5: Isla más grande
    cout << "\n🏆 NIVEL 5: Isla más grande" << endl;
    ganadores = filtrarPorIsla(ganadores);
    mostrarCandidatos(ganadores, "isla");

    if (ganadores.getTamaño() == 1)
    {
        cout << "✅ Ganador definido por isla más grande!" << endl;
        return ganadores[0];
    }

    // NIVEL 6: Mayor cantidad de PowerUps usados
    cout << "\n⚡ NIVEL 6: Mayor cantidad de PowerUps usados" << endl;
    ganadores = filtrarPorPowerUps(ganadores);
    mostrarCandidatos(ganadores, "powerups");

    if (ganadores.getTamaño() == 1)
    {
        cout << "✅ Ganador definido por uso de PowerUps!" << endl;
        return ganadores[0];
    }

    // NIVEL 7: Empate técnico
    cout << "\n🤝 NIVEL 7: Empate técnico" << endl;
    cout << "No se pudo determinar un ganador único." << endl;

    return nullptr; // Empate técnico
}

void Juego::mostrarEstadisticasFinales()
{
    cout << "\n═══════ ESTADÍSTICAS FINALES ═══════" << endl;
    
    // Mostrar el total de cuadrados en el tablero
    int totalCuadrosTablero = tablero->getFilas() * tablero->getColumnas();
    int cuadrosCompletados = 0;
    int cuadrosPorJugador[128] = {0}; // Usamos un arreglo para contar cuadrados por jugador (ASCII)
    
    // Contar cuántos cuadrados están completados realmente y quién es su propietario
    NodoLista* nodoActual = tablero->getCeldas()->getPrimero();
    while (nodoActual != nullptr) {
        if (nodoActual->celda->estaCompleta()) {
            cuadrosCompletados++;
            char propietario = nodoActual->celda->getPropietario();
            if (propietario != ' ') {
                cuadrosPorJugador[propietario]++;
            }
        }
        nodoActual = nodoActual->siguiente;
    }
    
    cout << "Tablero: " << tablero->getFilas() << "x" << tablero->getColumnas() 
         << " = " << totalCuadrosTablero << " cuadrados totales" << endl;
    cout << "Cuadrados completados: " << cuadrosCompletados << "/" << totalCuadrosTablero 
         << " (" << (cuadrosCompletados*100/totalCuadrosTablero) << "%)" << endl << endl;

    if (!jugadores->estaVacia())
    {
        Jugador *inicial = jugadores->frente_cola();
        Jugador *actual = inicial;

        do
        {
            char inicialJugador = actual->getInicial();
            int cuadrosCapturados = cuadrosPorJugador[inicialJugador];
            
            cout << "\n👤 " << actual->getNombre() << " (" << inicialJugador << "):" << endl;
            cout << "   Puntos: " << actual->getPuntos() << endl;
            cout << "   Cuadrados capturados: " << cuadrosCapturados 
                 << " de " << cuadrosCompletados << " (" 
                 << (cuadrosCompletados > 0 ? (cuadrosCapturados*100/cuadrosCompletados) : 0) 
                 << "%)" << endl;
            cout << "   PowerUps usados: " << actual->getPowerUpsUsados() << endl;
            cout << "   Isla más grande: " << actual->getIslaMasGrande() << endl;

            // Mostrar dominio por filas
            cout << "   Filas dominadas: ";
            for (int i = 0; i < tablero->getFilas(); i++)
            {
                if (actual->getCuadradosFila(i) > 0)
                {
                    cout << i << "(" << actual->getCuadradosFila(i) << ") ";
                }
            }
            cout << endl;

            // Mostrar dominio por columnas
            cout << "   Columnas dominadas: ";
            for (int i = 0; i < tablero->getColumnas(); i++)
            {
                if (actual->getCuadradosColumna(i) > 0)
                {
                    cout << i << "(" << actual->getCuadradosColumna(i) << ") ";
                }
            }
            cout << endl;

            jugadores->encolar(jugadores->desencolar());
            actual = jugadores->frente_cola();
        } while (actual != inicial);
    }
}

ArregloT<Jugador *> Juego::filtrarPorPuntos(const ArregloT<Jugador *> &candidatos)
{
    ArregloT<Jugador *> ganadores;
    int maxPuntos = -1;

    // Encontrar el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getPuntos() > maxPuntos)
        {
            maxPuntos = candidatos[i]->getPuntos();
        }
    }

    // Filtrar los que tienen el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getPuntos() == maxPuntos)
        {
            ganadores.agregar(candidatos[i]);
        }
    }

    return ganadores;
}

ArregloT<Jugador *> Juego::filtrarPorCuadrados(const ArregloT<Jugador *> &candidatos)
{
    ArregloT<Jugador *> ganadores;
    int maxCuadrados = -1;

    // Encontrar el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getTotalCuadrados() > maxCuadrados)
        {
            maxCuadrados = candidatos[i]->getTotalCuadrados();
        }
    }

    // Filtrar los que tienen el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getTotalCuadrados() == maxCuadrados)
        {
            ganadores.agregar(candidatos[i]);
        }
    }

    return ganadores;
}

ArregloT<Jugador *> Juego::filtrarPorFilas(const ArregloT<Jugador *> &candidatos)
{
    ArregloT<Jugador *> ganadores;
    int maxFilasDominadas = -1;

    // Contar filas dominadas para cada candidato
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        int filasDominadas = 0;

        for (int fila = 0; fila < tablero->getFilas(); fila++)
        {
            // Verificar si este jugador domina esta fila
            bool domina = true;
            int cuadradosEnFila = candidatos[i]->getCuadradosFila(fila);

            if (cuadradosEnFila == 0)
            {
                domina = false;
            }
            else
            {
                // Verificar que tenga más cuadrados que otros en esta fila
                for (int j = 0; j < candidatos.getTamaño(); j++)
                {
                    if (i != j && candidatos[j]->getCuadradosFila(fila) >= cuadradosEnFila)
                    {
                        domina = false;
                        break;
                    }
                }
            }

            if (domina)
                filasDominadas++;
        }

        if (filasDominadas > maxFilasDominadas)
        {
            maxFilasDominadas = filasDominadas;
        }
    }

    // Filtrar los que tienen el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        int filasDominadas = 0;

        for (int fila = 0; fila < tablero->getFilas(); fila++)
        {
            bool domina = true;
            int cuadradosEnFila = candidatos[i]->getCuadradosFila(fila);

            if (cuadradosEnFila == 0)
            {
                domina = false;
            }
            else
            {
                for (int j = 0; j < candidatos.getTamaño(); j++)
                {
                    if (i != j && candidatos[j]->getCuadradosFila(fila) >= cuadradosEnFila)
                    {
                        domina = false;
                        break;
                    }
                }
            }

            if (domina)
                filasDominadas++;
        }

        if (filasDominadas == maxFilasDominadas)
        {
            ganadores.agregar(candidatos[i]);
        }
    }

    return ganadores;
}

ArregloT<Jugador *> Juego::filtrarPorColumnas(const ArregloT<Jugador *> &candidatos)
{
    ArregloT<Jugador *> ganadores;
    int maxColumnasDominadas = -1;

    // Similar a filtrarPorFilas pero para columnas
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        int columnasDominadas = 0;

        for (int columna = 0; columna < tablero->getColumnas(); columna++)
        {
            bool domina = true;
            int cuadradosEnColumna = candidatos[i]->getCuadradosColumna(columna);

            if (cuadradosEnColumna == 0)
            {
                domina = false;
            }
            else
            {
                for (int j = 0; j < candidatos.getTamaño(); j++)
                {
                    if (i != j && candidatos[j]->getCuadradosColumna(columna) >= cuadradosEnColumna)
                    {
                        domina = false;
                        break;
                    }
                }
            }

            if (domina)
                columnasDominadas++;
        }

        if (columnasDominadas > maxColumnasDominadas)
        {
            maxColumnasDominadas = columnasDominadas;
        }
    }

    // Filtrar
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        int columnasDominadas = 0;

        for (int columna = 0; columna < tablero->getColumnas(); columna++)
        {
            bool domina = true;
            int cuadradosEnColumna = candidatos[i]->getCuadradosColumna(columna);

            if (cuadradosEnColumna == 0)
            {
                domina = false;
            }
            else
            {
                for (int j = 0; j < candidatos.getTamaño(); j++)
                {
                    if (i != j && candidatos[j]->getCuadradosColumna(columna) >= cuadradosEnColumna)
                    {
                        domina = false;
                        break;
                    }
                }
            }

            if (domina)
                columnasDominadas++;
        }

        if (columnasDominadas == maxColumnasDominadas)
        {
            ganadores.agregar(candidatos[i]);
        }
    }

    return ganadores;
}

ArregloT<Jugador *> Juego::filtrarPorIsla(const ArregloT<Jugador *> &candidatos)
{
    ArregloT<Jugador *> ganadores;
    int maxIsla = -1;

    // Encontrar el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getIslaMasGrande() > maxIsla)
        {
            maxIsla = candidatos[i]->getIslaMasGrande();
        }
    }

    // Filtrar los que tienen el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getIslaMasGrande() == maxIsla)
        {
            ganadores.agregar(candidatos[i]);
        }
    }

    return ganadores;
}

ArregloT<Jugador *> Juego::filtrarPorPowerUps(const ArregloT<Jugador *> &candidatos)
{
    ArregloT<Jugador *> ganadores;
    int maxPowerUps = -1;

    // Encontrar el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getPowerUpsUsados() > maxPowerUps)
        {
            maxPowerUps = candidatos[i]->getPowerUpsUsados();
        }
    }

    // Filtrar los que tienen el máximo
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        if (candidatos[i]->getPowerUpsUsados() == maxPowerUps)
        {
            ganadores.agregar(candidatos[i]);
        }
    }

    return ganadores;
}

void Juego::mostrarCandidatos(const ArregloT<Jugador *> &candidatos, const std::string &criterio)
{
    cout << "   Candidatos restantes (" << candidatos.getTamaño() << "): ";
    for (int i = 0; i < candidatos.getTamaño(); i++)
    {
        cout << candidatos[i]->getNombre() << "(" << candidatos[i]->getInicial() << ")";
        if (i < candidatos.getTamaño() - 1)
            cout << ", ";
    }
    cout << endl;
}