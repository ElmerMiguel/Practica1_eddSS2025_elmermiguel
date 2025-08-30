#ifndef POWERUP_H
#define POWERUP_H

#include <string>

enum TipoPowerUp {
    DOBLE_LINEA,    // DL - Permite colocar dos líneas consecutivas
    TRAMPA_SECRETA, // TS - Línea trampa que roba puntos
    BLOQUEO,        // BL - Línea bloqueada durante una ronda
    PASE,           // PS - Pasa turno y va al final
    LLAVE_SECRETA,  // LS - Ignora un bloqueo
    ESCURRIDIZO,    // ES - Protege contra trampas
    UNION_FUTURO,   // UF - Doble punto si el mismo jugador la usa después
    A_QUE_COSTO,    // AC - Punto para quien corresponde, casilla para quien puso la línea
    NUEVAS_TIERRAS, // NT - Expande el tablero hacia afuera
    EXPLOSIVOS      // EX - Elimina un punto del mapa
};

class PowerUp {
private:
    TipoPowerUp tipo;
    std::string simbolo;
    std::string descripcion;
    bool usado;
    bool activo;
    int filaEfecto, columnaEfecto;
    char ladoEfecto;
    bool recienObtenido;



public:
    PowerUp(TipoPowerUp tipo);  // Constructor simple
    PowerUp(TipoPowerUp tipo, const std::string& simbolo);  // Constructor completo
    
    // Regla de los 3
    ~PowerUp() = default;
    PowerUp(const PowerUp& otro);
    PowerUp& operator=(const PowerUp& otro);
    
    // Getters
    TipoPowerUp getTipo() const { return tipo; }
    std::string getSimbolo() const { return simbolo; }
    std::string getDescripcion() const { return descripcion; }
    std::string getTipoString() const;
    bool estaUsado() const { return usado; }
    bool estaActivo() const { return activo; }
    int getFilaEfecto() const { return filaEfecto; }
    int getColumnaEfecto() const { return columnaEfecto; }
    char getLadoEfecto() const { return ladoEfecto; }
    
    // Setters
    void setUsado(bool estado) { usado = estado; }
    void setActivo(bool estado) { activo = estado; }
    void setEfecto(int fila, int columna, char lado);
    
    // Metodos principales
    void usar();
    void mostrarInfo() const;
    
    // Factory method para crear PowerUps aleatorios
    static PowerUp* crearPowerUpAleatorio();
    static PowerUp* crearPowerUp(TipoPowerUp tipo);
    static PowerUp* crearPowerUpDesdeSimboloCelda(const std::string& simbolo);
    
    // Metodos para verificar efectos
    bool afectaLinea(int fila, int columna, char lado) const;
    bool esRecienObtenido() const { return recienObtenido; }
    void marcarComoViejo() { recienObtenido = false; }
    void marcarComoReciente() { recienObtenido = true; }
    
};

#endif