# Problema de Mínima Dispersión Diferencial (MDDP)

Este proyecto contiene la implementación de diversos algoritmos para resolver el Problema de Mínima Dispersión Diferencial (MDDP), un problema de optimización combinatoria NP-completo. La implementación forma parte de la Práctica 1 de la asignatura Metaheurísticas del Grado en Ingeniería Informática de la Universidad de Granada.

## Descripción del problema

El MDDP consiste en seleccionar un subconjunto de m elementos de un conjunto inicial de n elementos, de forma que se minimice la dispersión entre los elementos escogidos. La dispersión se define como la diferencia entre la máxima y mínima suma de distancias de cada elemento seleccionado al resto de elementos seleccionados.

## Estructura del proyecto

- **inc/**: Archivos de cabecera (.h)
  - `mddproblem.h`: Define la clase para el problema MDDP
  - `randomsearch.h`: Implementa el algoritmo aleatorio
  - `greedy.h`: Implementa el algoritmo Greedy
  - `localsearch.h`: Define la búsqueda local y sus variantes

- **src/**: Archivos de implementación (.cpp)

- **datos_MDD/**: Contiene 50 instancias del problema con diferentes valores de n y m

- **common/**: Utilidades comunes
  - `random.hpp`: Generador de números aleatorios
  - `problem.h`: Clase base para problemas de optimización
  - `solution.h`: Definición del tipo de solución
  - `mh.h`: Clase base para metaheurísticas

- **Ejecutables**:
  - `test_random`: Ejecuta el algoritmo aleatorio
  - `test_greedy`: Ejecuta el algoritmo Greedy
  - `test_localsearch`: Ejecuta la búsqueda local en sus dos variantes
  - `test_all`: Ejecuta todos los algoritmos para comparación
  - `run_experiments`: Ejecuta experimentos completos y genera estadísticas

## Compilación

```bash
mkdir -p build
cd build
cmake ..
make
```

## Uso

Ejemplos de uso básico:

```bash
# Ejecutar búsqueda local con una instancia específica
./build/test_localsearch datos_MDD/GKD-b_10_n25_m7.txt 222

# Ejecutar todos los algoritmos para una instancia
./build/test_all datos_MDD/GKD-b_10_n25_m7.txt 222

# Ejecutar todos los experimentos
./build/run_experiments datos_MDD/
```

## Algoritmos implementados

1. **RandomSearch**: Genera soluciones aleatorias y retorna la mejor
2. **GreedySearch**: Construye la solución paso a paso, seleccionando en cada iteración el mejor candidato
3. **Búsqueda Local (randLS)**: Exploración aleatoria del vecindario
4. **Búsqueda Local (heurLS)**: Exploración heurística del vecindario

## Resultados

El programa `run_experiments` genera varios archivos CSV con estadísticas detalladas:
- `results_detailed.csv`: Resultados de cada ejecución
- `results_by_case.csv`: Resultados promediados por caso
- `results_by_size.csv`: Resultados agrupados por tamaño
- `results_global.csv`: Resultados globales por algoritmo
