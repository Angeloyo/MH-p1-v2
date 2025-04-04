#include "mddproblem.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random.hpp>
#include <set>

using namespace std;
using Random = effolkronium::random_static;

MDDProblem::MDDProblem(const string& filename) : filename(filename) {
    // Abrir y leer archivo
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir " << filename << endl;
        exit(1);
    }

    // Leer n y m
    file >> n >> m;
    
    // Inicializar matriz de distancias
    distances.resize(n, vector<float>(n, 0.0));
    
    // Leer distancias
    int i, j;
    float dist;
    while (file >> i >> j >> dist) {
        distances[i][j] = dist;
        distances[j][i] = dist; // Matriz simétrica
    }
    
    file.close();
}

tFitness MDDProblem::fitness(const tSolution &solution) {
    // Calcular sumas para cada elemento seleccionado
    vector<float> sums(solution.size(), 0.0);
    
    // Para cada par de índices seleccionados, sumar sus distancias
    for (int i = 0; i < solution.size(); i++) {
        for (int j = 0; j < solution.size(); j++) {
            if (i != j) {
                int idx1 = solution[i];
                int idx2 = solution[j];
                
                // Asegurarse de que los índices son válidos
                if (idx1 < 0 || idx1 >= n || idx2 < 0 || idx2 >= n) {
                    cerr << "Error: Índices fuera de rango en fitness()" << endl;
                    return numeric_limits<tFitness>::max();
                }
                
                sums[i] += distances[idx1][idx2];
            }
        }
    }
    
    // Si solo hay un elemento en la solución, no hay dispersión
    if (solution.size() <= 1) {
        return 0.0;
    }
    
    // Dispersión = max - min
    float maxSum = *max_element(sums.begin(), sums.end());
    float minSum = *min_element(sums.begin(), sums.end());
    
    return maxSum - minSum;
}

// Generar información para factorización
SolutionFactoringInfo* MDDProblem::generateFactoringInfo(const tSolution &solution) {
    MDDFactoringInfo* info = new MDDFactoringInfo(m);
    
    // Calcular sumas para cada elemento seleccionado
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (i != j) {
                int idx1 = solution[i];
                int idx2 = solution[j];
                info->sums[i] += distances[idx1][idx2];
            }
        }
    }
    
    return info;
}

// Fitness factorizado - calcular efecto de reemplazar elemento en pos_change por new_value
tFitness MDDProblem::fitness(const tSolution &solution,
                           SolutionFactoringInfo *solution_info,
                           unsigned pos_change, tDomain new_value) {
    MDDFactoringInfo* info = dynamic_cast<MDDFactoringInfo*>(solution_info);
    
    // Calcular nueva suma para la posición cambiada
    float newSum = 0.0;
    int oldValue = solution[pos_change];
    
    // Calcular suma para el nuevo elemento
    for (int i = 0; i < m; i++) {
        if (i != pos_change) {
            int otherIdx = solution[i];
            newSum += distances[new_value][otherIdx];
        }
    }
    
    // Crear copia de las sumas actuales
    vector<float> newSums = info->sums;
    
    // Actualizar las sumas del resto de elementos
    for (int i = 0; i < m; i++) {
        if (i != pos_change) {
            int otherIdx = solution[i];
            // Restar la distancia al elemento antiguo
            newSums[i] -= distances[oldValue][otherIdx];
            // Sumar la distancia al nuevo elemento
            newSums[i] += distances[new_value][otherIdx];
        }
    }
    
    // Reemplazar la suma del elemento cambiado
    newSums[pos_change] = newSum;
    
    // Calcular dispersión
    float maxSum = *max_element(newSums.begin(), newSums.end());
    float minSum = *min_element(newSums.begin(), newSums.end());
    
    return maxSum - minSum;
}

// Actualizar información de factorización
void MDDProblem::updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info,
                                         const tSolution &solution,
                                         unsigned pos_change,
                                         tDomain new_value) {
    MDDFactoringInfo* info = dynamic_cast<MDDFactoringInfo*>(solution_info);
    int oldValue = solution[pos_change];
    
    // Actualizar sumas de todos los elementos
    for (int i = 0; i < m; i++) {
        if (i != pos_change) {
            int otherIdx = solution[i];
            // Actualizar suma del elemento en posición i
            info->sums[i] -= distances[oldValue][otherIdx];
            info->sums[i] += distances[new_value][otherIdx];
        }
    }
    
    // Recalcular suma para el elemento cambiado
    info->sums[pos_change] = 0.0;
    for (int i = 0; i < m; i++) {
        if (i != pos_change) {
            int otherIdx = solution[i];
            info->sums[pos_change] += distances[new_value][otherIdx];
        }
    }
}

tSolution MDDProblem::createSolution() {
    // Crear conjunto de elementos seleccionados aleatoriamente
    set<int> uniqueValues;
    
    // Seleccionar m elementos únicos aleatorios
    while (uniqueValues.size() < m) {
        int randomValue = Random::get<int>(0, n - 1);
        uniqueValues.insert(randomValue);
    }
    
    // Convertir a vector y ordenar
    tSolution solution(uniqueValues.begin(), uniqueValues.end());
    sort(solution.begin(), solution.end());
    
    return solution;
}

size_t MDDProblem::getSolutionSize() {
    return m; // Ahora devuelve m en lugar de n
}

std::pair<tDomain, tDomain> MDDProblem::getSolutionDomainRange() {
    return std::make_pair(0, n - 1); // Rango de índices válidos
} 