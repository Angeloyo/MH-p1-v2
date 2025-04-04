#include <cassert>
#include <greedy.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <random.hpp>
#include "mddproblem.h"
#include <set>

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
  
  // Obtener problema MDD
  MDDProblem* mddProblem = dynamic_cast<MDDProblem*>(problem);
  int n = mddProblem->getN();
  int m = mddProblem->getM();
  
  // Contador de evaluaciones
  int evals = 0;
  
  // Inicializar conjuntos de elementos disponibles y seleccionados
  set<int> available;
  for (int i = 0; i < n; i++) {
    available.insert(i);
  }
  
  // Vector para la solución (inicialmente vacío)
  tSolution solution;
  
  // Seleccionar primer elemento aleatorio
  int rand_index = Random::get<int>(0, available.size() - 1);
  auto it = available.begin();
  advance(it, rand_index);
  int first_elem = *it;
  
  // Añadir primer elemento a la solución
  solution.push_back(first_elem);
  available.erase(first_elem);
  
  // Seleccionar los m-1 elementos restantes
  while (solution.size() < m && evals < maxevals) {
    float best_dispersion = numeric_limits<float>::max();
    int best_elem = -1;
    
    // Probar cada elemento no seleccionado
    for (int candidate : available) {
      // Crear solución temporal con el candidato
      tSolution temp_solution = solution;
      temp_solution.push_back(candidate);
      
      // Evaluar
      float dispersion = problem->fitness(temp_solution);
      evals++;
      
      // Actualizar el mejor si mejora
      if (dispersion < best_dispersion) {
        best_dispersion = dispersion;
        best_elem = candidate;
      }
      
      // Limitar evaluaciones
      if (evals >= maxevals) break;
    }
    
    // Si alcanzamos el límite de evaluaciones
    if (evals >= maxevals) break;
    
    // Añadir el mejor elemento a la solución
    solution.push_back(best_elem);
    available.erase(best_elem);
  }
  
  // Ordenar la solución final (opcional)
  sort(solution.begin(), solution.end());
  
  // Calcular fitness final
  tFitness final_fitness = problem->fitness(solution);
  evals++;
  
  return ResultMH(solution, final_fitness, evals);
}
