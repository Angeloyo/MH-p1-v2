#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <chrono>
#include "mddproblem.h"
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

    // Inicializar semilla
    Random::seed(seed);
    
    // Crear el problema MDD
    MDDProblem problem(filename);
    
    // Crear los algoritmos de búsqueda local
    LocalSearch randLS_alg(randLS);
    LocalSearch heurLS_alg(heurLS);
    
    // Ejecutar BL con exploración aleatoria
    cout << "Ejecutando búsqueda local con exploración ALEATORIA (randLS)..." << endl;
    
    // Iniciar medición de tiempo para randLS
    auto start_rand = chrono::high_resolution_clock::now();
    
    ResultMH result_rand = randLS_alg.optimize(&problem, 100000);
    
    // Finalizar medición de tiempo para randLS
    auto end_rand = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_rand = end_rand - start_rand;
    
    cout << "Fitness (randLS): " << result_rand.fitness << endl;
    cout << "Evaluaciones: " << result_rand.evaluations << endl;
    cout << "Tiempo de ejecución: " << elapsed_rand.count() << " segundos" << endl;
    cout << "Elementos seleccionados: ";
    for (int idx : result_rand.solution) {
        cout << idx << " ";
    }
    cout << endl << endl;
    
    // Reiniciar semilla para comparación justa
    Random::seed(seed);
    
    // Ejecutar BL con exploración heurística
    cout << "Ejecutando búsqueda local con exploración HEURÍSTICA (heurLS)..." << endl;
    
    // Iniciar medición de tiempo para heurLS
    auto start_heur = chrono::high_resolution_clock::now();
    
    ResultMH result_heur = heurLS_alg.optimize(&problem, 100000);
    
    // Finalizar medición de tiempo para heurLS
    auto end_heur = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_heur = end_heur - start_heur;
    
    cout << "Fitness (heurLS): " << result_heur.fitness << endl;
    cout << "Evaluaciones: " << result_heur.evaluations << endl;
    cout << "Tiempo de ejecución: " << elapsed_heur.count() << " segundos" << endl;
    cout << "Elementos seleccionados: ";
    for (int idx : result_heur.solution) {
        cout << idx << " ";
    }
    cout << endl;

    return 0;
} 