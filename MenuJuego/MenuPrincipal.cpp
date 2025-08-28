#include "MenuPrincipal.h"
#include <iostream>
#include <cctype>
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
    int opcion;
    
    do {
        cout << "\n═══════ MENÚ PRINCIPAL ═══════" << endl;
        cout << "1. Configurar y jugar" << endl;
        cout << "2. Ver instrucciones" << endl;
        cout << "3. Ver PowerUps disponibles" << endl;
        cout << "4. Salir" << endl;
        cout << "Selecciona una opción: ";
        cin >> opcion;
        
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
        cout << "Ingresa el número de filas (mínimo 3): ";
        cin >> filas;
        cout << "Ingresa el número de columnas (mínimo 3): ";
        cin >> columnas;
        
        if (!validarTamaño(filas, columnas)) {
            cout << "❌ Tamaño inválido. Mínimo 3x3." << endl;
        }
    } while (!validarTamaño(filas, columnas));
    
    // Configurar número de jugadores
    do {
        cout << "Ingresa el número de jugadores (2-6): ";
        cin >> numJugadores;
        
        if (numJugadores < 2 || numJugadores > 6) {
            cout << "❌ Número de jugadores debe estar entre 2 y 6." << endl;
        }
    } while (numJugadores < 2 || numJugadores > 6);
    
    cout << "\n✅ Configuración guardada:" << endl;
    cout << "   Tablero: " << filas << "x" << columnas << endl;
    cout << "   Jugadores: " << numJugadores << endl;
}


void MenuPrincipal::configurarJugadores(Juego& juego) {
    cout << "\n═══════ CONFIGURACIÓN DE JUGADORES ═══════" << endl;
    
    ArregloT<char> inicialesUsadas;  // CAMBIADO: usar nuestro arreglo
    
    for (int i = 1; i <= numJugadores; i++) {
        string nombre;
        cout << "Nombre del jugador " << i << ": ";
        cin.ignore();
        getline(cin, nombre);
        
        // Asignar inicial única
        char inicial = asignarInicialUnica(nombre, inicialesUsadas);
        inicialesUsadas.agregar(inicial);  // CAMBIADO: usar nuestro método
        
        cout << "   → " << nombre << " tendrá la inicial '" << inicial << "'" << endl;
        
      
        Jugador nuevoJugador(nombre, inicial);
        juego.agregarJugador(nuevoJugador);
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