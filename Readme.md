# Totito Chino / Cajas 🎮

Implementación del juego Totito Chino (Timbiriche/Dots and Boxes) en C++ usando estructuras de datos implementadas desde cero.

![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.30%2B-green.svg)
![CLion](https://img.shields.io/badge/IDE-CLion-orange.svg)

## 📋 Descripción

Juego estratégico donde los jugadores se turnan para conectar puntos adyacentes con líneas. El objetivo es completar cuadrados para ganar puntos. Incluye un sistema de PowerUps que añade mecánicas especiales al juego.

### Características Principales
- ✅ Tablero dinámico configurable
- ✅ Sistema de turnos con cola circular
- ✅ 10 PowerUps únicos con efectos especiales
- ✅ Multijugador (2+ jugadores)
- ✅ Estructuras de datos implementadas desde cero
- ✅ Interfaz de consola intuitiva

## 🛠️ Requisitos del Sistema

### Software Requerido
- **CLion**: 2023.1 o superior
- **CMake**: 3.30 o superior
- **Compilador C++**: Compatible con C++20
  - GCC 10+ (Linux)
  - Clang 12+ (macOS/Linux)
  - MSVC 19.29+ (Windows con WSL recomendado)

### Sistema Operativo
- **Recomendado**: Ubuntu 20.04+ / Linux Mint 20+
- **Compatible**: macOS 12+, Windows 11 (con WSL2)
- **Terminal**: Mínimo 80x24 caracteres para mejor experiencia

## 🚀 Compilación en CLion

### Método 1: Importar Proyecto Existente

#### Paso 1: Abrir el Proyecto
1. Abre **CLion**
2. Selecciona `File` → `Open`
3. Navega hasta la carpeta del proyecto: `/home/usuario/proyecto`
4. Selecciona la carpeta y haz clic en `OK`

#### Paso 2: Configuración Automática
CLion detectará automáticamente el archivo `CMakeLists.txt` y configurará el proyecto.

```
🔄 CLion will automatically:
   ├── Detect CMakeLists.txt
   ├── Configure CMake project
   ├── Set up build directory (cmake-build-debug)
   └── Index source files
```

#### Paso 3: Verificar Configuración
1. Ve a `File` → `Settings` → `Build, Execution, Deployment` → `CMake`
2. Verifica que esté configurado:
   - **Build type**: Debug
   - **CMake options**: (vacío por defecto)
   - **Build directory**: `cmake-build-debug`

#### Paso 4: Compilar
1. **Opción A**: Presiona `Ctrl+F9` (Linux/Windows) o `Cmd+F9` (macOS)
2. **Opción B**: Ve a `Build` → `Build Project`
3. **Opción C**: Haz clic en el ícono 🔨 en la barra de herramientas

### Método 2: Proyecto Nuevo desde CMake

Si prefieres crear un proyecto nuevo:

#### Paso 1: Nuevo Proyecto CMake
1. Abre CLion
2. `File` → `New` → `Project`
3. Selecciona `C++ Executable`
4. **Project name**: `Practica1_eddSS2025_elmermiguel`
5. **Location**: Tu directorio preferido
6. **Language standard**: C++20
7. Haz clic en `Create`

#### Paso 2: Copiar Archivos Fuente
Copia todos los archivos del proyecto original manteniendo la estructura:
```bash
cp -r /ruta/original/* /nuevo/proyecto/
```

## 📁 Estructura del Proyecto

```
Practica1_eddSS2025_elmermiguel/
├── CMakeLists.txt              # ⚙️ Configuración de CMake
├── main.cpp                    # 🎯 Punto de entrada
├── Estructuras/                # 📊 Estructuras de datos
│   ├── ArregloT.h/.tpp        # Arreglo dinámico
│   ├── ListaT.h/.cpp          # Lista enlazada
│   ├── ColaC.h/.cpp           # Cola circular
│   └── PilaPower.h/.cpp       # Pila de PowerUps
├── MenuJuego/                  # 🎮 Lógica principal
│   ├── MenuPrincipal.h/.cpp   # Menú principal
│   └── Juego.h/.cpp           # Control del juego
├── Objetos/                    # 👥 Entidades del juego
│   ├── Jugador.h/.cpp         # Jugador
│   ├── PowerUp.h/.cpp         # PowerUps
│   └── GestorPowerUps.h/.cpp  # Gestor de efectos
├── PTablero/                   # 🎲 Tablero de juego
│   ├── Tablero.h/.cpp         # Tablero principal
│   └── Celda.h/.cpp           # Celdas individuales
├── Manuales/                   # 📚 Documentación
│   ├── Tecnico.md             # Manual técnico
│   └── Usuario.md             # Manual de usuario
└── cmake-build-debug/          # 🔧 Archivos de compilación
    └── Practica1Edd           # ✅ Ejecutable final
```

## ⚙️ Configuración de CMake

El archivo `CMakeLists.txt` está configurado con:

```cmake
cmake_minimum_required(VERSION 3.30)
project(Practica1Edd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Agregar todas las fuentes
add_executable(Practica1Edd
    main.cpp
    Estructuras/ListaT.cpp
    Estructuras/ColaC.cpp
    Estructuras/PilaPower.cpp
    MenuJuego/MenuPrincipal.cpp
    MenuJuego/Juego.cpp
    Objetos/Jugador.cpp
    Objetos/PowerUp.cpp
    Objetos/GestorPowerUps.cpp
    PTablero/Tablero.cpp
    PTablero/Celda.cpp
)

# Configurar directorios de headers
target_include_directories(Practica1Edd PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Estructuras
    ${CMAKE_CURRENT_SOURCE_DIR}/MenuJuego
    ${CMAKE_CURRENT_SOURCE_DIR}/Objetos
    ${CMAKE_CURRENT_SOURCE_DIR}/PTablero
)

# Opciones de compilación
target_compile_options(Practica1Edd PRIVATE
    -Wall -Wextra -Wpedantic
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)
```

## 🔧 Configuraciones de Compilación

### Debug (Desarrollo)
```bash
# En CLion: Build Type = Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
- Incluye información de debugging
- Sin optimizaciones
- Símbolos de debug para GDB/LLDB

### Release (Producción)
```bash
# En CLion: Build Type = Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
- Optimizaciones máximas (-O3)
- Sin información de debug
- Tamaño reducido del ejecutable

### RelWithDebInfo (Híbrido)
```bash
# En CLion: Build Type = RelWithDebInfo
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make
```
- Optimizaciones + información de debug
- Ideal para profiling

## 🏃‍♂️ Ejecutar el Proyecto

### Desde CLion
1. **Opción A**: Presiona `Shift+F10` para compilar y ejecutar
2. **Opción B**: Haz clic en ▶️ junto a la configuración `Practica1Edd`
3. **Opción C**: Ve a `Run` → `Run 'Practica1Edd'`

### Desde Terminal
```bash
# Navegar al directorio de build
cd cmake-build-debug

# Ejecutar el programa
./Practica1Edd
```

### Desde Terminal (compilación manual)
```bash
# Crear directorio de build
mkdir build && cd build

# Configurar con CMake
cmake ..

# Compilar
make -j$(nproc)

# Ejecutar
./Practica1Edd
```

## 🎮 Guía Rápida de Uso

1. **Ejecuta el programa**: `./Practica1Edd`
2. **Configura el juego**:
   - Tamaño del tablero (mínimo 3x3)
   - Número de jugadores
   - Nombres de jugadores
3. **Juega**:
   - Coloca líneas entre puntos
   - Completa cuadrados para ganar puntos
   - Usa PowerUps estratégicamente
4. **Gana**: ¡El jugador con más cuadrados completados gana!

## 🛠️ Solución de Problemas

### Error: "CMake not found"
```bash
# Ubuntu/Debian
sudo apt install cmake

# macOS
brew install cmake
```

### Error: "C++20 not supported"
Verifica tu compilador:
```bash
# Verificar GCC
gcc --version  # Debe ser 10+

# Verificar Clang
clang --version  # Debe ser 12+
```

### Error de Compilación en CLion
1. `File` → `Invalidate Caches and Restart`
2. `Tools` → `CMake` → `Reset Cache and Reload Project`
3. Asegúrate de que la configuración de CMake esté en Debug

### Problemas de Visualización
- Asegúrate de que tu terminal soporte UTF-8
- Usa un terminal de al menos 80x24 caracteres
- En Windows, usa WSL2 para mejor compatibilidad

### Error: "Permission denied"
```bash
# Dar permisos de ejecución
chmod +x ./Practica1Edd
```

## 📊 Debugging en CLion

### Configurar Puntos de Interrupción
1. Haz clic en el margen izquierdo junto al número de línea
2. Se creará un punto rojo de interrupción
3. Ejecuta en modo debug: `Shift+F9`

### Variables de Entorno
```bash
# Para mejor output en terminal
export TERM=xterm-256color
export LC_ALL=en_US.UTF-8
```

## 📚 Documentación Adicional

- [**Manual de Usuario**](Manuales/Usuario.md) - Cómo jugar el juego
- [**Manual Técnico**](Manuales/Tecnico.md) - Documentación técnica completa
- [**Especificaciones**](Edd_doc/00Practica1.md) - Requerimientos del proyecto

## 🏗️ Desarrollo

### Agregar Nuevos Archivos
1. Crea el archivo `.cpp/.h` en el directorio apropiado
2. Añádelo al `CMakeLists.txt`:
   ```cmake
   add_executable(Practica1Edd
       # ... archivos existentes ...
       NuevoDirectorio/NuevoArchivo.cpp
   )
   ```
3. CLion recargará automáticamente la configuración

### Testing
```bash
# Compilar en modo Debug para testing
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Ejecutar con Valgrind (opcional)
valgrind --leak-check=full ./Practica1Edd
```

## 📋 Checklist de Compilación

- [ ] CLion instalado y actualizado
- [ ] Proyecto abierto correctamente
- [ ] CMake configurado (Debug/Release)
- [ ] Compilación exitosa sin errores
- [ ] Ejecutable generado en `cmake-build-debug/`
- [ ] Programa ejecuta correctamente
- [ ] Terminal con tamaño adecuado (80x24+)

## 🎯 Comandos Útiles CLion

| Acción | Atajo | Menú |
|--------|-------|------|
| Compilar | `Ctrl+F9` | Build → Build Project |
| Ejecutar | `Shift+F10` | Run → Run |
| Debug | `Shift+F9` | Run → Debug |
| Recargar CMake | `Ctrl+Shift+O` | Tools → CMake → Reload |
| Limpiar Build | - | Build → Clean |

---

## 🎉 ¡Listo para Jugar!

Si has seguido todos los pasos, deberías tener el juego compilado y listo para ejecutar. ¡Diviértete jugando Totito Chino!

---

**Desarrollado por:** ElmerMiguel  
**Fecha:** Agosto 2025  
**Práctica:** Estructuras de Datos SS2025
