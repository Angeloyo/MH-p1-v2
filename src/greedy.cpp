#include <cassert>
#include <greedy.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <random.hpp>
#include "mddproblem.h"

using namespace std;
using Random = effolkronium::random_static;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

/**
 * Implementación del algoritmo Greedy para el problema MDD
 * 1. Selecciona un elemento inicial aleatorio
 * 2. En cada paso, añade el elemento que minimice la dispersión
 * 3. Repite hasta seleccionar m elementos
 *
 * @param problem El problema a optimizar
 * @param maxevals Número máximo de evaluaciones permitidas
 * @return Resultado con la mejor solución encontrada y su fitness
 */
ResultMH GreedySearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  
  // Preparar solución vacía
  size_t n = problem->getSolutionSize();
  tSolution solution(n, false);
  
  // Contador de evaluaciones
  int evals = 0;
  
  // Inicializar conjunto de elementos disponibles
  vector<int> available;
  for (size_t i = 0; i < n; i++) {
    available.push_back(i);
  }
  
  // Obtener instancia MDDProblem para acceder a m
  MDDProblem* mddProblem = dynamic_cast<MDDProblem*>(problem);
  int m = mddProblem->getM();
  
  // Vector para almacenar los elementos seleccionados
  vector<int> selected;
  
  // Seleccionar primer elemento aleatorio
  int rand_index = Random::get<int>(0, available.size() - 1);
  int first_elem = available[rand_index];
  solution[first_elem] = true;
  selected.push_back(first_elem);
  available.erase(available.begin() + rand_index);
  
  // Seleccionar los m-1 elementos restantes
  while (selected.size() < m && evals < maxevals) {
    float best_dispersion = numeric_limits<float>::max();
    int best_idx = -1;
    
    // Evaluar cada elemento no seleccionado
    for (size_t i = 0; i < available.size(); i++) {
      int candidate = available[i];
      
      // Añadir temporalmente el candidato a la solución
      solution[candidate] = true;
      
      // Evaluar la nueva solución
      float dispersion = problem->fitness(solution);
      evals++;
      
      // Quitar el candidato para probar el siguiente
      solution[candidate] = false;
      
      // Actualizar el mejor si mejora
      if (dispersion < best_dispersion) {
        best_dispersion = dispersion;
        best_idx = i;
      }
      
      // Limitar evaluaciones
      if (evals >= maxevals) break;
    }
    
    // Si alcanzamos el límite de evaluaciones
    if (evals >= maxevals) break;
    
    // Añadir el mejor elemento encontrado
    int best_elem = available[best_idx];
    solution[best_elem] = true;
    selected.push_back(best_elem);
    available.erase(available.begin() + best_idx);
  }
  
  // Calcular fitness final
  tFitness final_fitness = problem->fitness(solution);
  evals++;
  
  return ResultMH(solution, final_fitness, evals);
}
