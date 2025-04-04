#include "localsearch.h"
#include "mddproblem.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <random.hpp>
#include <unordered_set>
#include <limits>

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
        
        if (mode == heurLS) {
            // Estrategia para heurLS: evaluar todos los posibles intercambios y elegir el mejor
            tFitness best_fitness = current_fitness;
            int best_sel = -1;
            int best_nonsel = -1;
            int best_pos = -1;
            
            // Para cada elemento seleccionado
            for (int pos = 0; pos < m; pos++) {
                int sel_elem = solution[pos];
                
                // Para cada elemento no seleccionado
                for (int nonsel : not_selected) {
                    // Evaluar el intercambio
                    tFitness new_fitness = problem->fitness(solution, info, pos, nonsel);
                    evals++;
                    
                    // Actualizar el mejor intercambio si mejora
                    if (new_fitness < best_fitness) {
                        best_fitness = new_fitness;
                        best_sel = sel_elem;
                        best_nonsel = nonsel;
                        best_pos = pos;
                    }
                    
                    // Si se alcanza el límite de evaluaciones, interrumpir
                    if (evals >= maxevals) break;
                }
                
                if (evals >= maxevals) break;
            }
            
            // Si se encontró un mejor intercambio, aplicarlo
            if (best_sel != -1 && best_fitness < current_fitness) {
                // Actualizar conjuntos
                selected.erase(best_sel);
                selected.insert(best_nonsel);
                not_selected.erase(best_nonsel);
                not_selected.insert(best_sel);
                
                // Actualizar información factorizada ANTES de actualizar la solución
                problem->updateSolutionFactoringInfo(info, solution, best_pos, best_nonsel);
                
                // Actualizar solución DESPUÉS de la información factorizada
                solution[best_pos] = best_nonsel;
                
                // Actualizar fitness
                current_fitness = best_fitness;
                
                // Indicar que hubo mejora
                improved = true;
            }
        } else {
            // Modo randLS: estrategia original de primer mejor (no cambia)
            // Vector de posiciones a explorar (orden aleatorio)
            vector<int> positions;
            for (int i = 0; i < m; i++) {
                positions.push_back(i);
            }
            Random::shuffle(positions.begin(), positions.end());
            
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
                        
                        // Actualizar información factorizada ANTES de actualizar la solución
                        problem->updateSolutionFactoringInfo(info, solution, pos, candidate);
                        
                        // Actualizar solución DESPUÉS de la información factorizada
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
    }
    
    // Liberar la memoria de la información de factorización
    delete info;
    
    return ResultMH(solution, current_fitness, evals);
} 