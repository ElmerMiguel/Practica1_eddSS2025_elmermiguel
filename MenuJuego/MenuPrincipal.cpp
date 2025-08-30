#include "MenuPrincipal.h"
#include <iostream>
#include <cctype>
#include <limits>
#include <string>
using namespace std;



MenuPrincipal::MenuPrincipal() : filas(3), columnas(3), numJugadores(2) {}

void MenuPrincipal::mostrarBienvenida() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════╗" << endl;
    cout << "║                    TOTITO CHINO                          ║" << endl;
    cout << "║                  (Dots and Boxes)                        ║" << endl;
    cout << "║                                                          ║" << endl;
    cout << "║  Conecta puntos para formar cuadrados y ganar puntos!    ║" << endl;
    cout << "║  Usa PowerUps estratégicamente para ganar ventaja.       ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════╝" << endl;
    cout << "\n";
}

void MenuPrincipal::mostrarMenu() {
    int opcion = 0;
    
    do {
        cout << "\n═══════ MENÚ PRINCIPAL ═══════" << endl;
        cout << "1. Configurar y jugar" << endl;
        cout << "2. Ver instrucciones" << endl;
        cout << "3. Ver PowerUps disponibles" << endl;
        cout << "4. Salir" << endl;
        cout << "Selecciona una opción: ";
        
        if (!(cin >> opcion)) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            cout << "❌ Por favor, ingresa un número válido." << endl;
            continue;
        }
        
        switch(opcion) {
            case 1:
                configurarJuego();
                iniciarJuego();
                break;
            case 2:
                mostrarInstrucciones();
                break;
            case 3:
                mostrarPowerUps();
                break;
            case 4:
                cout << "¡Gracias por jugar Totito Chino!" << endl;
                break;
            default:
                cout << "Opción inválida. Intenta de nuevo." << endl;
        }
    } while(opcion != 4);
}

void MenuPrincipal::configurarJuego() {
    cout << "\n═══════ CONFIGURACIÓN DEL JUEGO ═══════" << endl;
    
    // Configurar tamaño del tablero
    do {
        cout << "Ingresa el numero de filas (mínimo 3): ";
        if (!(cin >> filas)) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            cout << "❌ Por favor, ingresa un número válido." << endl;
            filas = 0; // Invalidar el valor para que el bucle continúe
            continue;
        }
        
        cout << "Ingresa el numero de columnas (mínimo 3): ";
        if (!(cin >> columnas)) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            cout << "❌ Por favor, ingresa un número válido." << endl;
            columnas = 0; // Invalidar el valor para que el bucle continúe
            continue;
        }
        
        if (!validarTamaño(filas, columnas)) {
            cout << "❌ Tamaño inválido. Mínimo 3x3." << endl;
        }
    } while (!validarTamaño(filas, columnas));
    
    // Configurar número de jugadores
    do {
        cout << "Ingresa el número de jugadores (2-4): ";
        if (!(cin >> numJugadores)) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            cout << "❌ Por favor, ingresa un número válido." << endl;
            numJugadores = 0; // Invalidar el valor para que el bucle continúe
            continue;
        }
        
        if (numJugadores < 2 || numJugadores > 4) {
            cout << "❌ Número de jugadores debe estar entre 2 y 4." << endl;
        }
    } while (numJugadores < 2 || numJugadores > 4);
    
    cout << "\n✅ Configuración guardada:" << endl;
    cout << "   Tablero: " << filas << "x" << columnas << endl;
    cout << "   Jugadores: " << numJugadores << endl;
}


void MenuPrincipal::configurarJugadores(Juego& juego) {
    cout << "\n═══════ CONFIGURACIÓN DE JUGADORES ═══════" << endl;
    
    ArregloT<char> inicialesUsadas;

    for (int i = 0; i < numJugadores; i++) {
        string nombre;
        char inicial;
        bool inicialValida = false;

        // limpiar antes de leer el nombre de cada jugador
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        while (!inicialValida) {
            cout << "Nombre del jugador " << (i + 1) << ": ";
            getline(cin, nombre);

            if (nombre.empty()) {
                cout << "❌ El nombre no puede estar vacío." << endl;
                continue;
            }

            inicial = toupper(nombre[0]);
            inicialValida = true;

            cout << "   → " << nombre << " tendrá la inicial '" << inicial << "'" << endl;
            Jugador nuevoJugador(nombre, inicial);
            juego.agregarJugador(nuevoJugador);
        }
    }


    cout << "\n✅ Todos los jugadores configurados!" << endl;
}



void MenuPrincipal::iniciarJuego() {
    cout << "\n🎮 ¡Iniciando el juego!" << endl;
    
    // Crear juego con la configuración
    Juego juego(filas, columnas);
    
    // Configurar jugadores
    configurarJugadores(juego);
    
    // Iniciar partida
    juego.iniciar();
}

void MenuPrincipal::mostrarInstrucciones() {
    cout << "\n═══════ INSTRUCCIONES ═══════" << endl;
    cout << "• Conecta puntos adyacentes con líneas (S/I/L/D)" << endl;
    cout << "• Cuando cierres un cuadrado, ganas 1 punto y juegas de nuevo" << endl;
    cout << "• Los PowerUps aparecen aleatoriamente en el tablero" << endl;
    cout << "• Usa PowerUps estratégicamente para ganar ventaja" << endl;
    cout << "• El juego termina cuando todas las líneas estén marcadas" << endl;
    cout << "• Gana quien tenga más puntos al final" << endl;
    
    cout << "\nControles:" << endl;
    cout << "• S = Lado Superior    • I = Lado Inferior" << endl;
    cout << "• L = Lado Izquierdo   • D = Lado Derecho" << endl;
}

void MenuPrincipal::mostrarPowerUps() {
    cout << "\n═══════ POWERUPS DISPONIBLES ═══════" << endl;
    cout << "[DL] Doble Línea     - Coloca dos líneas seguidas" << endl;
    cout << "[TS] Trampa Secreta  - Roba puntos al enemigo" << endl;
    cout << "[BL] Bloqueo         - Bloquea línea por una ronda" << endl;
    cout << "[PS] Pase            - Pasa turno al final" << endl;
    cout << "[LS] Llave Secreta   - Ignora un bloqueo" << endl;
    cout << "[ES] Escurridizo     - Protege contra trampas" << endl;
    cout << "[UF] Unión a Futuro  - Doble punto futuro" << endl;
    cout << "[AC] A Qué Costo     - Punto para dueño, casilla para ti" << endl;
    cout << "[NT] Nuevas Tierras  - Expande el tablero" << endl;
    cout << "[EX] Explosivos      - Elimina punto del mapa" << endl;
}

bool MenuPrincipal::validarTamaño(int filas, int columnas) {
    return filas >= 3 && columnas >= 3;
}

char MenuPrincipal::asignarInicialUnica(const string& nombre, ArregloT<char>& inicialesUsadas) {
    // Intentar usar la primera letra del nombre
    char inicial = toupper(nombre[0]);
    
    // Si ya está usada, buscar otra letra del nombre
    for (int i = 0; i < nombre.length(); i++) {  // CAMBIADO: usar for tradicional
        char candidato = toupper(nombre[i]);
        if (!inicialesUsadas.contiene(candidato)) {  // CAMBIADO: usar nuestro método
            return candidato;
        }
    }
    
    // Si todas las letras del nombre están usadas, usar letras A-Z
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!inicialesUsadas.contiene(c)) {  // CAMBIADO: usar nuestro método
            return c;
        }
    }
    
    // Fallback (muy raro que pase)
    return '?';
}

void MenuPrincipal::configurarTablero() {
    cout << "\n═══════ CONFIGURACIÓN DEL TABLERO ═══════" << endl;
    
    validarEntradaNumerica(filas, 3, 8, "Ingrese número de filas (3-8): ");
    validarEntradaNumerica(columnas, 3, 8, "Ingrese número de columnas (3-8): ");
    
    cout << "✅ Tablero configurado: " << filas << "x" << columnas << endl;
    
    // Crear juego con las nuevas dimensiones
    if (juego != nullptr) {
        delete juego;
    }
    juego = new Juego(filas, columnas);
}

void MenuPrincipal::validarEntradaNumerica(int& valor, int min, int max, const std::string& mensaje) {
    bool entradaValida = false;
    
    while (!entradaValida) {
        cout << mensaje;
        
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                entradaValida = true;
            } else {
                cout << "❌ Error: Debe ser entre " << min << " y " << max << "." << endl;
            }
        } else {
            cout << "❌ Error: Ingrese un número válido." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}