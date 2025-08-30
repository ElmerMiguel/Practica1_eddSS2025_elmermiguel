#ifndef ARREGLOT_H
#define ARREGLOT_H

template<typename T>
class ArregloT {
private:
    T* datos;
    int tamaño;
    int capacidad;

public:
    ArregloT();
    ArregloT(int capacidadInicial);
    
    // Regla de los 3
    ~ArregloT();
    ArregloT(const ArregloT& otro);
    ArregloT& operator=(const ArregloT& otro);
    
    void agregar(const T& elemento);
    T& obtener(int indice);
    const T& obtener(int indice) const;
    bool contiene(const T& elemento) const;
    int getTamaño() const { return tamaño; }
    bool estaVacio() const { return tamaño == 0; }
    
    // Operador [] para acceso directo
    T& operator[](int indice) { return datos[indice]; }
    const T& operator[](int indice) const { return datos[indice]; }

private:
    void redimensionar();
};

#include "ArregloT.tpp"  // Incluir implementación de template

#endif