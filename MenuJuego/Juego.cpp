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

    // Reducir turnos de efectos del jugador actual
    if (actual->tieneEfectoEscurridizo())
    {
        // Se maneja internamente en Jugador
    }

    cout << "\n=== Turno de: " << actual->getNombre()
         << " (" << actual->getInicial() << ") ===" << endl;

    mostrarTablero();
    actual->mostrarInfo();

    // Variables para controlar efectos especiales
    bool puedeColocarSegundaLinea = false;
    bool ignorarBloqueos = false;

    // Preguntar si quiere usar un PowerUp
    if (actual->tienePowerUps())
    {
        char usarPower;
        cout << "¿Usar un PowerUp? (s/n): ";
        cin >> usarPower;

        if (usarPower == 's' || usarPower == 'S')
        {
            actual->mostrarPowerUps();
            PowerUp *powerUsado = actual->usarPowerUp();
            if (powerUsado != nullptr)
            {
                actual->registrarPowerUpUsado();

                // Manejar efectos especiales según el tipo
                switch (powerUsado->getTipo())
                {
                case PASE:
                    tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                    // Mover al final de la cola
                    jugadores->encolar(jugadores->desencolar());
                    delete powerUsado;
                    tablero->procesarFinTurno();
                    return; // Terminar turno sin marcar línea

                case ESCURRIDIZO:
                    actual->activarEscurridizo(3);
                    tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                    break;

                case DOBLE_LINEA:
                    puedeColocarSegundaLinea = true;
                    tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                    break;

                case LLAVE_SECRETA:
                    ignorarBloqueos = true;
                    tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                    break;

                
case EXPLOSIVOS: {
    cout << "💥 ¡EXPLOSIVOS ACTIVADO!" << endl;
    
    // Mostrar puntos disponibles
    tablero->mostrarPuntosDisponibles();
    
    // Pedir coordenadas del punto a eliminar
    int filaEliminar, columnaEliminar;
    cout << "Ingrese coordenadas del punto a eliminar (fila columna): ";
    cin >> filaEliminar >> columnaEliminar;
    
    // Validar y eliminar punto
    if (tablero->puntoEsValido(filaEliminar, columnaEliminar)) {
        tablero->eliminarPunto(filaEliminar, columnaEliminar);
        tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
        cout << "🎯 Explosivos usado exitosamente!" << endl;
    } else {
        cout << "❌ Coordenadas inválidas. PowerUp no consumido." << endl;
        // Devolver el PowerUp al jugador
        actual->agregarPowerUp(powerUsado);
        powerUsado = nullptr; // Evitar que se borre
    }
    break;
}

                case NUEVAS_TIERRAS:
                    if (tablero->puedeExpandir())
                    {
                        tablero->expandirTablero();
                        tablero->usarPowerUp(powerUsado, 0, 0, ' ', actual->getInicial());
                    }
                    else
                    {
                        cout << "❌ No se puede expandir más el tablero." << endl;
                        // Devolver el PowerUp al jugador si no se puede usar
                        actual->agregarPowerUp(powerUsado);
                        powerUsado = nullptr; // Evitar que se borre
                    }
                    break;

                default:
                {
                    cout << "Ingrese fila, columna y lado para aplicar el PowerUp: ";
                    int fila, columna;
                    char lado;
                    cin >> fila >> columna >> lado;
                    tablero->usarPowerUp(powerUsado, fila, columna, lado, actual->getInicial());
                    break;
                }
                }

                delete powerUsado;
            }




            if (powerUsado->getTipo() == UNION_FUTURO && powerUsado->esRecienObtenido()) {
        cout << "❌ UNIÓN A FUTURO no se puede usar inmediatamente." << endl;
        cout << "Debe esperar al menos un turno después de obtenerlo." << endl;
        
        // Devolver el PowerUp al jugador
        actual->agregarPowerUp(powerUsado);
        powerUsado = nullptr; // Evitar que se borre
    } else {
        // Marcar PowerUp como viejo si es necesario
        if (powerUsado->getTipo() == UNION_FUTURO) {
            powerUsado->marcarComoViejo();
        }
        
        actual->registrarPowerUpUsado();

        // Manejar efectos especiales según el tipo
        switch (powerUsado->getTipo()) {
            // ... casos existentes ...
        }

        if (powerUsado != nullptr) {
            delete powerUsado;
        }
    }






        }
    }

    // Marcar primera línea (o única si no tiene doble línea)
    bool turnoExitoso = false;
    do
    {
        int fila, columna;
        char lado;
        cout << "Ingrese fila, columna y lado (S/I/L/D): ";
        cin >> fila >> columna >> lado;

        // Verificar bloqueos (a menos que tenga llave secreta)
        if (!ignorarBloqueos && tablero->getGestorPowers()->lineaBloqueada(fila, columna, lado))
        {
            cout << "Esta línea está bloqueada. Intente otra." << endl;
            continue;
        }

        if (tablero->marcarLinea(fila, columna, lado, actual->getInicial()))
        {
            cout << "Línea marcada exitosamente!" << endl;
            
            // verificar trampa
            if (tablero->getGestorPowers()->lineaConTrampa(fila, columna, lado)) {
                char propietarioTrampa = tablero->getGestorPowers()->obtenerPropietarioTrampa(fila, columna, lado);
            
                if (propietarioTrampa != ' ') {
                    // Verificar protección Escurridizo
                    if (!tablero->getGestorPowers()->jugadorTieneEscurridizo(actual->getInicial())) {
                        cout << "💥 ¡TRAMPA ACTIVADA por jugador " << propietarioTrampa << "!" << endl;
            
                        // Buscar al trapero y moverlo al frente
                        Jugador* trapero = nullptr;
                        int posiciones = 0;
            
                        // Contar jugadores y encontrar al trapero
                        Jugador* busqueda = jugadores->frente_cola();
                        do {
                            if (busqueda->getInicial() == propietarioTrampa) {
                                trapero = busqueda;
                                break;
                            }
                            jugadores->encolar(jugadores->desencolar());
                            busqueda = jugadores->frente_cola();
                            posiciones++;
                        } while (busqueda != actual && posiciones < 10); // Evitar loop infinito
            
                        // Mover al trapero al frente (solo si no es el jugador actual)
                        if (trapero != nullptr && trapero != actual) {
                            // Extraer al trapero de su posición
                            Jugador* extraido = jugadores->desencolar();
            
                            // Rotar hasta encontrar la posición correcta
                            while (extraido != trapero && posiciones > 0) {
                                jugadores->encolar(extraido);
                                extraido = jugadores->desencolar();
                                posiciones--;
                            }
            
                            // Poner al trapero al frente después del turno actual
                            cout << "🎯 " << trapero->getNombre() << " se mueve al frente de la cola!" << endl;
            
                            // Mantener al jugador actual al frente por ahora
                            jugadores->encolar(extraido); // Poner al trapero en segunda posición
                        } else {
                            cout << "El trapero ya está jugando o es el jugador actual." << endl;
                        }
                    }
                }
            }
            turnoExitoso = true;

            // Verificar si se completó un cuadrado
            if (tablero->verificarCuadradoCompleto(fila, columna))
            {
                
                char propietarioPunto = tablero->getGestorPowers()->obtenerPropietarioAQueCosto(fila, columna, lado);

                if (propietarioPunto != ' ')
                {
                    // Efecto A Qué Costo activo
                    cout << "💰 ¡A Qué Costo activado!" << endl;

                    // Buscar al jugador que debe recibir el punto
                    Jugador *jugadorPunto = nullptr;
                    Jugador *inicialBusqueda = jugadores->frente_cola();
                    Jugador *actualBusqueda = inicialBusqueda;

                    do
                    {
                        if (actualBusqueda->getInicial() == propietarioPunto)
                        {
                            jugadorPunto = actualBusqueda;
                            break;
                        }
                        jugadores->encolar(jugadores->desencolar());
                        actualBusqueda = jugadores->frente_cola();
                    } while (actualBusqueda != inicialBusqueda);

                    // Dar punto al propietario original
                    if (jugadorPunto != nullptr)
                    {
                        jugadorPunto->incrementarPuntos();
                        jugadorPunto->registrarCuadrado(fila, columna);
                        cout << "   Punto para: " << jugadorPunto->getNombre() << " (" << propietarioPunto << ")" << endl;
                    }

                    // Dar casilla a quien completó
                    actual->registrarCuadrado(fila, columna);
                    Celda *celda = tablero->obtenerCelda(fila, columna);
                    if (celda != nullptr)
                    {
                        celda->setPropietario(actual->getInicial());
                    }
                    cout << "   Casilla para: " << actual->getNombre() << " (" << actual->getInicial() << ")" << endl;
                }
                else
                {
                    // Comportamiento normal
                    actual->registrarCuadrado(fila, columna);

                    // Asignar propietario a la celda
                    Celda *celda = tablero->obtenerCelda(fila, columna);
                    if (celda != nullptr)
                    {
                        celda->setPropietario(actual->getInicial());
                    }

                    // Verificar efectos especiales de puntuación
                    if (tablero->getGestorPowers()->lineaConUnionFuturo(fila, columna, lado))
                    {
                        actual->duplicarUltimoPunto(); // Doble punto
                    }
                    else
                    {
                        actual->incrementarPuntos();
                    }
                }

                cout << "¡Cuadrado completado en (" << fila << "," << columna << ")!" << endl;

                // Verificar si la celda tiene PowerUp
                Celda *celda = tablero->obtenerCelda(fila, columna);
                if (celda != nullptr && !celda->getPowerUp().empty())
                {
                    cout << "¡Recogiste un PowerUp: " << celda->getPowerUp() << "!" << endl;
                    PowerUp *nuevoPower = PowerUp::crearPowerUpAleatorio();
                    actual->agregarPowerUp(nuevoPower);
                    celda->setPowerUp("");



                
    if (nuevoPower->getTipo() == UNION_FUTURO) {
        nuevoPower->marcarComoReciente();
        cout << "⏳ UNIÓN A FUTURO debe esperar un turno antes de usarse." << endl;
    }
    
    actual->agregarPowerUp(nuevoPower);
    celda->setPowerUp("");
                }

                // El mismo jugador continúa (no se avanza turno)
            }
            else
            {
                // Avanzar al siguiente jugador
                Jugador *jugadorActual = jugadores->desencolar();
                jugadores->encolar(jugadorActual);
            }
        }
        else
        {
            cout << "Movimiento inválido. Intente de nuevo." << endl;
        }
    } while (!turnoExitoso);

    // Si tiene doble línea, permitir segunda línea
    if (puedeColocarSegundaLinea)
    {
        cout << "\n¡LÍNEA ADICIONAL! Puedes marcar otra línea:" << endl;

        int fila2, columna2;
        char lado2;
        cout << "Ingrese fila, columna y lado para segunda línea: ";
        cin >> fila2 >> columna2 >> lado2;

        if (tablero->marcarLinea(fila2, columna2, lado2, actual->getInicial()))
        {
            cout << "¡Segunda línea marcada!" << endl;

            if (tablero->verificarCuadradoCompleto(fila2, columna2))
            {
                
                actual->registrarCuadrado(fila2, columna2);

                Celda *celda2 = tablero->obtenerCelda(fila2, columna2);
                if (celda2 != nullptr)
                {
                    celda2->setPropietario(actual->getInicial());
                }

                actual->incrementarPuntos();
                cout << "¡Segundo cuadrado completado!" << endl;
            }
        }
    }

    actual->procesarFinTurnoJugador();
    // Procesar fin de turno
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

    // Opción manual para terminar (mantener por ahora para testing)
    char continuar;
    cout << "¿Continuar jugando? (s/n): ";
    cin >> continuar;
    return (continuar == 'n' || continuar == 'N');
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

    if (!jugadores->estaVacia())
    {
        Jugador *inicial = jugadores->frente_cola();
        Jugador *actual = inicial;

        do
        {
            cout << "\n👤 " << actual->getNombre() << " (" << actual->getInicial() << "):" << endl;
            cout << "   Puntos: " << actual->getPuntos() << endl;
            cout << "   Cuadrados totales: " << actual->getTotalCuadrados() << endl;
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