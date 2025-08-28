#include "ArregloT.h"

template<typename T>
ArregloT<T>::ArregloT() : tamaño(0), capacidad(10) {
    datos = new T[capacidad];
}

template<typename T>
ArregloT<T>::ArregloT(int capacidadInicial) : tamaño(0), capacidad(capacidadInicial) {
    datos = new T[capacidad];
}

template<typename T>
ArregloT<T>::~ArregloT() {
    delete[] datos;
    datos = nullptr;
}

template<typename T>
ArregloT<T>::ArregloT(const ArregloT& otro) : tamaño(otro.tamaño), capacidad(otro.capacidad) {
    datos = new T[capacidad];
    for (int i = 0; i < tamaño; i++) {
        datos[i] = otro.datos[i];
    }
}

template<typename T>
ArregloT<T>& ArregloT<T>::operator=(const ArregloT& otro) {
    if (this != &otro) {
        delete[] datos;
        
        tamaño = otro.tamaño;
        capacidad = otro.capacidad;
        datos = new T[capacidad];
        
        for (int i = 0; i < tamaño; i++) {
            datos[i] = otro.datos[i];
        }
    }
    return *this;
}

template<typename T>
void ArregloT<T>::agregar(const T& elemento) {
    if (tamaño >= capacidad) {
        redimensionar();
    }
    datos[tamaño] = elemento;
    tamaño++;
}

template<typename T>
T& ArregloT<T>::obtener(int indice) {
    return datos[indice];
}

template<typename T>
const T& ArregloT<T>::obtener(int indice) const {
    return datos[indice];
}

template<typename T>
bool ArregloT<T>::contiene(const T& elemento) const {
    for (int i = 0; i < tamaño; i++) {
        if (datos[i] == elemento) {
            return true;
        }
    }
    return false;
}

template<typename T>
void ArregloT<T>::redimensionar() {
    int nuevaCapacidad = capacidad * 2;
    T* nuevosDatos = new T[nuevaCapacidad];
    
    for (int i = 0; i < tamaño; i++) {
        nuevosDatos[i] = datos[i];
    }
    
    delete[] datos;
    datos = nuevosDatos;
    capacidad = nuevaCapacidad;
}