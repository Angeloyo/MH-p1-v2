#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <chrono>
#include "mddproblem.h"
#include "randomsearch.h"
#include "greedy.h"
#include "localsearch.h"

using namespace std;
using Random = effolkronium::random_static;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_de_datos> [semilla]" << endl;
        return 1;
    }

    string filename = argv[1];
    long int seed = (argc > 2) ? atoi(argv[2]) : 42;
    
    // Definir el número máximo de evaluaciones
    int max_evals = 100000;
    
    cout << "Ejecutando algoritmos en " << filename << " con semilla " << seed << endl;
    cout << "-----------------------------------------------------" << endl;
    
    // 1. RandomSearch
    {
        // Crear nueva instancia del problema para RandomSearch
        MDDProblem problem(filename);
        
        // Inicializar semilla
        Random::seed(seed);
        
        RandomSearch alg;
        auto start = chrono::high_resolution_clock::now();
        
        ResultMH result = alg.optimize(&problem, max_evals);
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        
        cout << "RANDOM SEARCH:" << endl;
        cout << "  Fitness: " << result.fitness << endl;
        cout << "  Evaluaciones: " << result.evaluations << endl;
        cout << "  Tiempo: " << elapsed.count() << " segundos" << endl;
        cout << "  Solución: [";
        for (size_t i = 0; i < result.solution.size(); i++) {
            cout << result.solution[i];
            if (i < result.solution.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "-----------------------------------------------------" << endl;
    }
    
    // 2. GreedySearch
    {
        // Crear nueva instancia del problema para GreedySearch
        MDDProblem problem(filename);
        
        // Inicializar semilla
        Random::seed(seed);
        
        GreedySearch alg;
        auto start = chrono::high_resolution_clock::now();
        
        ResultMH result = alg.optimize(&problem, max_evals);
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        
        cout << "GREEDY SEARCH:" << endl;
        cout << "  Fitness: " << result.fitness << endl;
        cout << "  Evaluaciones: " << result.evaluations << endl;
        cout << "  Tiempo: " << elapsed.count() << " segundos" << endl;
        cout << "  Solución: [";
        for (size_t i = 0; i < result.solution.size(); i++) {
            cout << result.solution[i];
            if (i < result.solution.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "-----------------------------------------------------" << endl;
    }
    
    // 3. LocalSearch con exploración aleatoria (randLS)
    {
        // Crear nueva instancia del problema para LocalSearch (randLS)
        MDDProblem problem(filename);
        
        // Inicializar semilla
        Random::seed(seed);
        
        LocalSearch alg(randLS);
        auto start = chrono::high_resolution_clock::now();
        
        ResultMH result = alg.optimize(&problem, max_evals);
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        
        cout << "LOCAL SEARCH (randLS):" << endl;
        cout << "  Fitness: " << result.fitness << endl;
        cout << "  Evaluaciones: " << result.evaluations << endl;
        cout << "  Tiempo: " << elapsed.count() << " segundos" << endl;
        cout << "  Solución: [";
        for (size_t i = 0; i < result.solution.size(); i++) {
            cout << result.solution[i];
            if (i < result.solution.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "-----------------------------------------------------" << endl;
    }
    
    // 4. LocalSearch con exploración heurística (heurLS)
    {
        // Crear nueva instancia del problema para LocalSearch (heurLS)
        MDDProblem problem(filename);
        
        // Inicializar semilla
        Random::seed(seed);
        
        LocalSearch alg(heurLS);
        auto start = chrono::high_resolution_clock::now();
        
        ResultMH result = alg.optimize(&problem, max_evals);
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        
        cout << "LOCAL SEARCH (heurLS):" << endl;
        cout << "  Fitness: " << result.fitness << endl;
        cout << "  Evaluaciones: " << result.evaluations << endl;
        cout << "  Tiempo: " << elapsed.count() << " segundos" << endl;
        cout << "  Solución: [";
        for (size_t i = 0; i < result.solution.size(); i++) {
            cout << result.solution[i];
            if (i < result.solution.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
    
    return 0;
} 