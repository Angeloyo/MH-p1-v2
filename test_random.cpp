#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include "mddproblem.h"
#include "randomsearch.h"

using namespace std;
using Random = effolkronium::random_static;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_de_datos> [semilla]" << endl;
        return 1;
    }

    string filename = argv[1];
    long int seed = (argc > 2) ? atoi(argv[2]) : 42;

    // Inicializar semilla
    Random::seed(seed);
    
    // Crear el problema MDD
    MDDProblem problem(filename);
    
    // Crear el algoritmo aleatorio
    RandomSearch alg;
    
    // Ejecutar el algoritmo con 100000 evaluaciones
    cout << "Ejecutando RandomSearch en " << filename << " con semilla " << seed << endl;
    ResultMH result = alg.optimize(&problem, 100000);
    
    // Mostrar resultados
    cout << "Mejor fitness: " << result.fitness << endl;
    cout << "Evaluaciones: " << result.evaluations << endl;

    // Mostrar elementos seleccionados
    cout << "Elementos seleccionados: ";
    for (size_t i = 0; i < result.solution.size(); i++) {
        if (result.solution[i]) {
            cout << i << " ";
        }
    }
    cout << endl;

    return 0;
} 