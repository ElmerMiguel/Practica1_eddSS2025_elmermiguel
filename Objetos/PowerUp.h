#ifndef POWERUP_H
#define POWERUP_H

#include <string>

class PowerUp {
private:
    std::string tipo;
    std::string descripcion;
    bool usado;

public:
    PowerUp(const std::string& tipo, const std::string& descripcion = "");
    
    // Regla de los 3
    ~PowerUp() = default;                          // Destructor por defecto
    PowerUp(const PowerUp& otro);                 // Copy constructor
    PowerUp& operator=(const PowerUp& otro);      // Assignment operator
    
    // Getters
    std::string getTipo() const { return tipo; }
    std::string getDescripcion() const { return descripcion; }
    bool estaUsado() const { return usado; }
    
    // Setters
    void setUsado(bool estado) { usado = estado; }
    void setDescripcion(const std::string& desc) { descripcion = desc; }
    
    // Métodos útiles
    void usar();
    void mostrarInfo() const;
    
    // Factory method para crear PowerUps específicos
    static PowerUp* crearPowerUp(const std::string& tipo);
};

#endif