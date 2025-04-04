#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <chrono>
#include "mddproblem.h"
#include "greedy.h"

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
    
    // Crear el algoritmo Greedy
    GreedySearch alg;
    
    // Iniciar medición de tiempo
    auto start = chrono::high_resolution_clock::now();
    
    // Ejecutar el algoritmo
    cout << "Ejecutando Greedy en " << filename << " con semilla " << seed << endl;
    ResultMH result = alg.optimize(&problem, 100000);
    
    // Finalizar medición de tiempo
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    
    // Mostrar resultados
    cout << "Mejor fitness: " << result.fitness << endl;
    cout << "Evaluaciones: " << result.evaluations << endl;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;
    cout << "Elementos seleccionados: ";
    for (int idx : result.solution) {
        cout << idx << " ";
    }
    cout << endl;

    return 0;
} 