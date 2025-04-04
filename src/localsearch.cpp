#include "localsearch.h"
#include "mddproblem.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <random.hpp>
#include <unordered_set>

using namespace std;
using Random = effolkronium::random_static;

// Constructor
LocalSearch::LocalSearch(ExplorationMode mode) : mode(mode) {}

// Método principal de optimización
ResultMH LocalSearch::optimize(Problem *problem, int maxevals) {
    assert(maxevals > 0);
    
    // Obtener problema MDD
    MDDProblem* mddProblem = dynamic_cast<MDDProblem*>(problem);
    
    // Tamaño del problema
    int n = mddProblem->getN();
    int m = mddProblem->getM();
    
    // Generar solución inicial aleatoria
    tSolution solution = problem->createSolution();
    tFitness current_fitness = problem->fitness(solution);
    
    // Generar información para factorización
    SolutionFactoringInfo* info = problem->generateFactoringInfo(solution);
    
    // Contadores
    int evals = 1; // Ya evaluamos la solución inicial
    bool improved = true;
    
    // Crear conjuntos de elementos seleccionados y no seleccionados
    unordered_set<int> selected(solution.begin(), solution.end());
    unordered_set<int> not_selected;
    
    // Inicializar elementos no seleccionados
    for (int i = 0; i < n; i++) {
        if (selected.find(i) == selected.end()) {
            not_selected.insert(i);
        }
    }
    
    // Bucle principal de búsqueda local
    while (improved && evals < maxevals) {
        improved = false;
        
        // En modo heurLS, ordenar elementos por su contribución al fitness
        vector<pair<int, float>> contributions;
        
        if (mode == heurLS) {
            // Probar el efecto de reemplazar cada elemento seleccionado
            for (int i = 0; i < m; i++) {
                int sel = solution[i];
                // Evaluar la contribución eliminando este elemento
                tFitness fitness_without = problem->fitness(solution, info, i, sel);
                float contribution = fitness_without - current_fitness;
                contributions.push_back(make_pair(i, contribution));
            }
            
            // Ordenar por contribución (menor primero)
            sort(contributions.begin(), contributions.end(), 
                [](const pair<int, float>& a, const pair<int, float>& b) {
                    return a.second < b.second;
                });
        }
        
        // Vector de posiciones a explorar (ordenado según el modo)
        vector<int> positions;
        
        if (mode == heurLS) {
            // Usar orden según contribución
            for (auto& pair : contributions) {
                positions.push_back(pair.first);
            }
        } else {
            // Orden aleatorio para randLS
            for (int i = 0; i < m; i++) {
                positions.push_back(i);
            }
            Random::shuffle(positions.begin(), positions.end());
        }
        
        // Para cada posición a explorar
        for (int pos : positions) {
            if (improved || evals >= maxevals) break;
            
            int current_elem = solution[pos];
            
            // Vector de elementos no seleccionados (barajado)
            vector<int> candidates(not_selected.begin(), not_selected.end());
            Random::shuffle(candidates.begin(), candidates.end());
            
            // Probar intercambios
            for (int candidate : candidates) {
                if (evals >= maxevals) break;
                
                // Evaluar el intercambio usando factorización
                tFitness new_fitness = problem->fitness(solution, info, pos, candidate);
                evals++;
                
                // Si hay mejora, aplicar el intercambio
                if (new_fitness < current_fitness) {
                    // Actualizar conjuntos
                    selected.erase(current_elem);
                    selected.insert(candidate);
                    not_selected.erase(candidate);
                    not_selected.insert(current_elem);
                    
                    // Actualizar información factorizada
                    problem->updateSolutionFactoringInfo(info, solution, pos, candidate);
                    
                    // Actualizar solución
                    solution[pos] = candidate;
                    
                    // Actualizar fitness
                    current_fitness = new_fitness;
                    
                    // Marcar mejora y salir (primer mejor)
                    improved = true;
                    break;
                }
            }
        }
    }
    
    // Liberar la memoria de la información de factorización
    delete info;
    
    return ResultMH(solution, current_fitness, evals);
} 