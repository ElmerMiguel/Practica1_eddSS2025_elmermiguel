# Manual de Usuario - Totito Chino / Cajas

## ¿Qué es Totito Chino?

Totito Chino (también conocido como Timbiriche o Dots and Boxes) es un juego donde conectas puntos con líneas para formar cuadrados. ¡Quien forme más cuadrados gana!

## Objetivo del Juego

- **Meta**: Formar la mayor cantidad de cuadrados posible
- **Cómo formar un cuadrado**: Conectar 4 líneas alrededor de un espacio vacío
- **Puntos**: Cada cuadrado que completes te da 1 punto
- **Bonus**: Al completar un cuadrado obtienes un turno extra

## Configuración Inicial

### 1. Tamaño del Tablero
- Mínimo: 3x3 puntos
- Máximo: Lo que desees (recomendado hasta 10x10 para pantalla)
- El programa te preguntará: "¿Cuántos puntos de ancho?" y "¿Cuántos de alto?"

### 2. Número de Jugadores
- Desde 2 jugadores hasta los que desees
- Cada jugador tendrá una inicial única (A, B, C, etc.)

### 3. Nombres
- Ingresa el nombre de cada jugador
- Si no ingresas nombre, se asignará automáticamente

## Cómo Jugar

### Turno Básico
1. **Tu turno llegará** y verás el tablero
2. **Elige una línea** para colocar:
   - Selecciona las coordenadas (fila, columna)
   - Elige el lado: S(uperior), I(nferior), D(erecha), L(izquierda)
3. **Si completas un cuadrado**:
   - Ganas 1 punto
   - Tu inicial aparece en el cuadrado
   - Obtienes un turno extra
   - Puede que obtengas un PowerUp

### Ejemplo de Colocación
```
   1   2   3
1  ●---●   ●
   |   |    
2  ●   ●---●
       |   |
3  ●---●---●
```

Para conectar el punto (1,1) con (1,2), eliges:
- Fila: 1, Columna: 1, Lado: D (derecha)

## PowerUps - ¡Poderes Especiales!

Los PowerUps aparecen aleatoriamente en el tablero. Cuando completas un cuadrado que tiene un PowerUp, ¡lo obtienes!

### PowerUps Disponibles

| Símbolo | Nombre | ¿Qué hace? |
|---------|--------|------------|
| **DL** | Doble Línea | Puedes colocar 2 líneas seguidas |
| **TS** | Trampa Secreta | Tu línea "roba" puntos del siguiente jugador |
| **BL** | Bloqueo | Bloqueas una línea para que nadie más la use |
| **PS** | Pase | Te saltas tu turno y vas al final |
| **LS** | Llave Secreta | Puedes usar una línea bloqueada |
| **ES** | Escurridizo | Te proteges de una trampa |
| **UF** | Unión a Futuro | Si completas otro cuadrado después, valen 2 puntos |
| **AC** | A Qué Costo | El punto va para quien toca, pero el cuadrado es tuyo |
| **NT** | Nuevas Tierras | ¡Agranda el tablero! |
| **EX** | Explosivos | Elimina un punto del mapa |

### Cómo Usar PowerUps
1. En tu turno, elige "Usar PowerUp" en lugar de colocar línea
2. Selecciona cuál PowerUp usar de tu inventario
3. Sigue las instrucciones específicas del PowerUp

## Controles del Juego

### Menú Principal
- `1` - Nuevo Juego
- `2` - Ver Instrucciones
- `3` - Salir

### Durante el Juego
- `1` - Colocar línea normal
- `2` - Usar PowerUp
- `3` - Ver mi inventario
- `4` - Ver tablero "clarividente" (muestra efectos activos)
- `5` - Ver puntuaciones

### Colocar Líneas
1. Ingresa **fila** (número)
2. Ingresa **columna** (número)
3. Ingresa **lado**:
   - `S` - Superior (arriba)
   - `I` - Inferior (abajo)
   - `D` - Derecha
   - `L` - Izquierda

## Estrategias Básicas

### Para Principiantes
1. **Evita dar cuadrados gratis**: No coloques la tercera línea de un cuadrado
2. **Busca cadenas**: A veces puedes completar varios cuadrados seguidos
3. **Guarda PowerUps**: Úsalos en momentos estratégicos

### PowerUps Útiles
- **Doble Línea**: Perfecto para completar cadenas largas
- **Trampa Secreta**: Colócala donde sepas que el siguiente jugador cerrará
- **Bloqueo**: Úsalo para proteger tus futuras jugadas

## Fin del Juego

### ¿Cuándo termina?
El juego termina cuando **todas las líneas posibles** están colocadas y **todos los cuadrados** están completos.

### ¿Quién gana?
1. **Ganador**: El jugador con más puntos
2. **Empate**: Si hay empate, todos los empatados son ganadores
3. **Puntuación final**: Se muestra el nombre, inicial y puntos de cada jugador

## Consejos y Trucos

### 🎯 Estrategias Avanzadas
- **Sacrificio controlado**: A veces dar 1 cuadrado para ganar 3 vale la pena
- **Control del tablero**: Usa bloqueos para controlar áreas importantes
- **Timing de PowerUps**: Guarda los PowerUps potentes para el final

### 🚀 PowerUps Pro
- **Nuevas Tierras + Doble Línea**: Expande y domina las nuevas áreas
- **Trampa + Escurridizo**: Coloca trampas mientras te proteges
- **Unión a Futuro**: Márcalo en áreas donde planeas volver

### 🎮 Controles Rápidos
- Los números se ingresan directamente
- Las letras para lados no distinguen mayúsculas/minúsculas
- Puedes ver tu inventario en cualquier momento

## Solución de Problemas

### Errores Comunes
- **"Línea ya existe"**: Esa línea ya fue colocada
- **"Coordenadas inválidas"**: Verifica que estén dentro del tablero
- **"No tienes PowerUps"**: Tu inventario está vacío
- **"Línea bloqueada"**: Alguien usó un bloqueo en esa línea

### Si el Juego se Cierra
1. Vuelve a ejecutar el programa
2. El juego comenzará desde cero (no hay guardado automático)

## Requisitos del Sistema

- **Sistema Operativo**: Linux/Unix
- **Terminal**: Cualquier terminal que soporte texto
- **Pantalla**: Recomendado mínimo 80x24 caracteres
- **Teclado**: Entrada numérica y de letras

---

## ¡Empieza a Jugar!

1. Ejecuta el programa: `./Practica1Edd`
2. Sigue las instrucciones en pantalla
3. ¡Diviértete conectando puntos y formando cuadrados!

**¿Listo para ser el maestro del Totito Chino?** 🎮✨

---

*Manual de Usuario v1.0 - Totito Chino*  
*Desarrollado como práctica de Estructuras de Datos*
