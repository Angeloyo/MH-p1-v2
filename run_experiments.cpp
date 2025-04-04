#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <random.hpp>
#include "mddproblem.h"
#include "randomsearch.h"
#include "greedy.h"
#include "localsearch.h"

using namespace std;
using Random = effolkronium::random_static;

// Estructura para almacenar los resultados de un algoritmo
struct AlgorithmResult {
    float fitness;
    float time_seconds;
    float desv;
};

// Estructura para almacenar todos los resultados de un caso
struct CaseResult {
    string case_name;
    int n;
    int m;
    float best_known;
    vector<AlgorithmResult> random_results;
    vector<AlgorithmResult> greedy_results;
    vector<AlgorithmResult> randls_results;
    vector<AlgorithmResult> heurls_results;
};

// Carga los mejores costes conocidos desde el archivo
map<string, float> load_best_costs(const string& filename) {
    map<string, float> best_costs;
    ifstream file(filename);
    
    if (!file) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return best_costs;
    }
    
    string case_name;
    int n, m;
    float cost;
    
    while (file >> case_name >> n >> m >> cost) {
        best_costs[case_name] = cost;
    }
    
    cout << "Cargados " << best_costs.size() << " mejores costes conocidos." << endl;
    return best_costs;
}

// Calcula la desviación respecto al mejor valor conocido
float calculate_desv(float obtained, float best_known) {
    if (best_known == 0) {
        return (obtained == 0) ? 0.0f : 100.0f;
    }
    return 100.0f * (obtained - best_known) / best_known;
}

// Función para ejecutar un algoritmo y obtener resultado con tiempo
AlgorithmResult run_algorithm(const string& algo_name, Problem* problem, int max_evals, long seed, float best_known) {
    AlgorithmResult result;
    
    // Inicializar semilla
    Random::seed(seed);
    
    auto start = chrono::high_resolution_clock::now();
    
    // Ejecutar el algoritmo correspondiente
    tFitness best_fitness = 0;
    
    if (algo_name == "random") {
        RandomSearch alg;
        ResultMH alg_result = alg.optimize(problem, max_evals);
        best_fitness = alg_result.fitness;
    } else if (algo_name == "greedy") {
        GreedySearch alg;
        ResultMH alg_result = alg.optimize(problem, max_evals);
        best_fitness = alg_result.fitness;
    } else if (algo_name == "randls") {
        LocalSearch alg(randLS);
        ResultMH alg_result = alg.optimize(problem, max_evals);
        best_fitness = alg_result.fitness;
    } else if (algo_name == "heurls") {
        LocalSearch alg(heurLS);
        ResultMH alg_result = alg.optimize(problem, max_evals);
        best_fitness = alg_result.fitness;
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    
    // Almacenar resultados
    result.fitness = best_fitness;
    result.time_seconds = elapsed.count();
    result.desv = calculate_desv(best_fitness, best_known);
    
    return result;
}

// Función para guardar los resultados detallados
void save_detailed_results(const vector<CaseResult>& results, const string& filename) {
    ofstream file(filename);
    
    if (!file) {
        cerr << "Error: No se pudo crear el archivo " << filename << endl;
        return;
    }
    
    // Encabezado
    file << "Caso,n,m,MejorConocido,Algoritmo,Repeticion,Fitness,Tiempo,Desv" << endl;
    
    // Datos
    for (const auto& case_result : results) {
        // Random (1 ejecución)
        for (size_t i = 0; i < case_result.random_results.size(); i++) {
            file << case_result.case_name << ","
                 << case_result.n << ","
                 << case_result.m << ","
                 << case_result.best_known << ","
                 << "Random" << ","
                 << i+1 << ","
                 << case_result.random_results[i].fitness << ","
                 << case_result.random_results[i].time_seconds << ","
                 << case_result.random_results[i].desv << endl;
        }
        
        // Greedy (1 ejecución)
        for (size_t i = 0; i < case_result.greedy_results.size(); i++) {
            file << case_result.case_name << ","
                 << case_result.n << ","
                 << case_result.m << ","
                 << case_result.best_known << ","
                 << "Greedy" << ","
                 << i+1 << ","
                 << case_result.greedy_results[i].fitness << ","
                 << case_result.greedy_results[i].time_seconds << ","
                 << case_result.greedy_results[i].desv << endl;
        }
        
        // randLS (5 ejecuciones)
        for (size_t i = 0; i < case_result.randls_results.size(); i++) {
            file << case_result.case_name << ","
                 << case_result.n << ","
                 << case_result.m << ","
                 << case_result.best_known << ","
                 << "RandLS" << ","
                 << i+1 << ","
                 << case_result.randls_results[i].fitness << ","
                 << case_result.randls_results[i].time_seconds << ","
                 << case_result.randls_results[i].desv << endl;
        }
        
        // heurLS (5 ejecuciones)
        for (size_t i = 0; i < case_result.heurls_results.size(); i++) {
            file << case_result.case_name << ","
                 << case_result.n << ","
                 << case_result.m << ","
                 << case_result.best_known << ","
                 << "HeurLS" << ","
                 << i+1 << ","
                 << case_result.heurls_results[i].fitness << ","
                 << case_result.heurls_results[i].time_seconds << ","
                 << case_result.heurls_results[i].desv << endl;
        }
    }
    
    cout << "Resultados detallados guardados en " << filename << endl;
}

// Función para guardar resultados por caso (promediados)
void save_case_results(const vector<CaseResult>& results, const string& filename) {
    ofstream file(filename);
    
    if (!file) {
        cerr << "Error: No se pudo crear el archivo " << filename << endl;
        return;
    }
    
    // Encabezado
    file << "Caso,n,m,MejorConocido,Random_Fitness,Random_Tiempo,Random_Desv,Greedy_Fitness,Greedy_Tiempo,Greedy_Desv,RandLS_Fitness,RandLS_Tiempo,RandLS_Desv,HeurLS_Fitness,HeurLS_Tiempo,HeurLS_Desv" << endl;
    
    // Datos
    for (const auto& case_result : results) {
        // Calcular promedios para randLS y heurLS
        float randls_avg_fitness = 0, randls_avg_time = 0, randls_avg_desv = 0;
        float heurls_avg_fitness = 0, heurls_avg_time = 0, heurls_avg_desv = 0;
        
        for (const auto& result : case_result.randls_results) {
            randls_avg_fitness += result.fitness;
            randls_avg_time += result.time_seconds;
            randls_avg_desv += result.desv;
        }
        
        randls_avg_fitness /= case_result.randls_results.size();
        randls_avg_time /= case_result.randls_results.size();
        randls_avg_desv /= case_result.randls_results.size();
        
        for (const auto& result : case_result.heurls_results) {
            heurls_avg_fitness += result.fitness;
            heurls_avg_time += result.time_seconds;
            heurls_avg_desv += result.desv;
        }
        
        heurls_avg_fitness /= case_result.heurls_results.size();
        heurls_avg_time /= case_result.heurls_results.size();
        heurls_avg_desv /= case_result.heurls_results.size();
        
        // Escribir línea
        file << case_result.case_name << ","
             << case_result.n << ","
             << case_result.m << ","
             << case_result.best_known << ","
             << case_result.random_results[0].fitness << ","
             << case_result.random_results[0].time_seconds << ","
             << case_result.random_results[0].desv << ","
             << case_result.greedy_results[0].fitness << ","
             << case_result.greedy_results[0].time_seconds << ","
             << case_result.greedy_results[0].desv << ","
             << randls_avg_fitness << ","
             << randls_avg_time << ","
             << randls_avg_desv << ","
             << heurls_avg_fitness << ","
             << heurls_avg_time << ","
             << heurls_avg_desv << endl;
    }
    
    cout << "Resultados por caso guardados en " << filename << endl;
}

// Función para guardar resultados por tamaño
void save_size_results(const vector<CaseResult>& results, const string& filename) {
    ofstream file(filename);
    
    if (!file) {
        cerr << "Error: No se pudo crear el archivo " << filename << endl;
        return;
    }
    
    // Encabezado
    file << "Tamaño,Algoritmo,Desv,Tiempo" << endl;
    
    // Procesar por tamaños (50, 100, 150, etc.)
    vector<int> sizes = {25, 50, 100, 125, 150}; // Tamaños de los casos
    
    for (int size : sizes) {
        // Acumuladores para cada algoritmo
        float random_desv = 0, random_time = 0;
        float greedy_desv = 0, greedy_time = 0;
        float randls_desv = 0, randls_time = 0;
        float heurls_desv = 0, heurls_time = 0;
        
        // Contador de casos con este tamaño
        int count = 0;
        
        // Sumar valores para este tamaño
        for (const auto& case_result : results) {
            if (case_result.n == size) {
                count++;
                
                // Random
                random_desv += case_result.random_results[0].desv;
                random_time += case_result.random_results[0].time_seconds;
                
                // Greedy
                greedy_desv += case_result.greedy_results[0].desv;
                greedy_time += case_result.greedy_results[0].time_seconds;
                
                // RandLS (promedio de 5 ejecuciones)
                float randls_case_desv = 0, randls_case_time = 0;
                for (const auto& result : case_result.randls_results) {
                    randls_case_desv += result.desv;
                    randls_case_time += result.time_seconds;
                }
                randls_desv += randls_case_desv / case_result.randls_results.size();
                randls_time += randls_case_time / case_result.randls_results.size();
                
                // HeurLS (promedio de 5 ejecuciones)
                float heurls_case_desv = 0, heurls_case_time = 0;
                for (const auto& result : case_result.heurls_results) {
                    heurls_case_desv += result.desv;
                    heurls_case_time += result.time_seconds;
                }
                heurls_desv += heurls_case_desv / case_result.heurls_results.size();
                heurls_time += heurls_case_time / case_result.heurls_results.size();
            }
        }
        
        if (count > 0) {
            // Calcular promedios
            random_desv /= count;
            random_time /= count;
            greedy_desv /= count;
            greedy_time /= count;
            randls_desv /= count;
            randls_time /= count;
            heurls_desv /= count;
            heurls_time /= count;
            
            // Escribir resultados para este tamaño
            file << size << ",Random," << random_desv << "," << random_time << endl;
            file << size << ",Greedy," << greedy_desv << "," << greedy_time << endl;
            file << size << ",RandLS," << randls_desv << "," << randls_time << endl;
            file << size << ",HeurLS," << heurls_desv << "," << heurls_time << endl;
        }
    }
    
    cout << "Resultados por tamaño guardados en " << filename << endl;
}

// Función para guardar resultados globales
void save_global_results(const vector<CaseResult>& results, const string& filename) {
    ofstream file(filename);
    
    if (!file) {
        cerr << "Error: No se pudo crear el archivo " << filename << endl;
        return;
    }
    
    // Encabezado
    file << "Algoritmo,Desv,Tiempo" << endl;
    
    // Acumuladores para cada algoritmo
    float random_desv = 0, random_time = 0;
    float greedy_desv = 0, greedy_time = 0;
    float randls_desv = 0, randls_time = 0;
    float heurls_desv = 0, heurls_time = 0;
    
    // Sumar valores para todos los casos
    for (const auto& case_result : results) {
        // Random
        random_desv += case_result.random_results[0].desv;
        random_time += case_result.random_results[0].time_seconds;
        
        // Greedy
        greedy_desv += case_result.greedy_results[0].desv;
        greedy_time += case_result.greedy_results[0].time_seconds;
        
        // RandLS (promedio de 5 ejecuciones)
        float randls_case_desv = 0, randls_case_time = 0;
        for (const auto& result : case_result.randls_results) {
            randls_case_desv += result.desv;
            randls_case_time += result.time_seconds;
        }
        randls_desv += randls_case_desv / case_result.randls_results.size();
        randls_time += randls_case_time / case_result.randls_results.size();
        
        // HeurLS (promedio de 5 ejecuciones)
        float heurls_case_desv = 0, heurls_case_time = 0;
        for (const auto& result : case_result.heurls_results) {
            heurls_case_desv += result.desv;
            heurls_case_time += result.time_seconds;
        }
        heurls_desv += heurls_case_desv / case_result.heurls_results.size();
        heurls_time += heurls_case_time / case_result.heurls_results.size();
    }
    
    // Calcular promedios
    int case_count = results.size();
    if (case_count > 0) {
        random_desv /= case_count;
        random_time /= case_count;
        greedy_desv /= case_count;
        greedy_time /= case_count;
        randls_desv /= case_count;
        randls_time /= case_count;
        heurls_desv /= case_count;
        heurls_time /= case_count;
        
        // Escribir resultados globales
        file << "Random," << random_desv << "," << random_time << endl;
        file << "Greedy," << greedy_desv << "," << greedy_time << endl;
        file << "RandLS," << randls_desv << "," << randls_time << endl;
        file << "HeurLS," << heurls_desv << "," << heurls_time << endl;
    }
    
    cout << "Resultados globales guardados en " << filename << endl;
}

int main(int argc, char* argv[]) {
    // Validar argumentos
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <directorio_datos>" << endl;
        return 1;
    }
    
    // Directorio con archivos de datos
    string data_dir = argv[1];
    if (data_dir.back() != '/') {
        data_dir += '/';
    }
    
    // Cargar mejores costes conocidos
    map<string, float> best_costs = load_best_costs("best_costs.txt");
    if (best_costs.empty()) {
        cerr << "Error: No se pudieron cargar los mejores costes conocidos." << endl;
        return 1;
    }
    
    // Definir semillas para las 5 ejecuciones
    vector<long> seeds = {1234, 5678, 9012, 3456, 7890};
    
    // Definir número máximo de evaluaciones
    int max_evals = 100000;
    
    // Vector para almacenar todos los resultados
    vector<CaseResult> all_results;
    
    // Para cada caso del problema
    for (int case_num = 1; case_num <= 50; case_num++) {
        // Construir nombre del archivo de datos
        ostringstream case_filename;
        case_filename << "GKD-b_" << case_num;
        
        // Determinar n y m basado en el caso
        int n, m;
        if (case_num <= 5) {
            n = 25; m = 2;
        } else if (case_num <= 10) {
            n = 25; m = 7;
        } else if (case_num <= 15) {
            n = 50; m = 5;
        } else if (case_num <= 20) {
            n = 50; m = 15;
        } else if (case_num <= 25) {
            n = 100; m = 10;
        } else if (case_num <= 30) {
            n = 100; m = 30;
        } else if (case_num <= 35) {
            n = 125; m = 12;
        } else if (case_num <= 40) {
            n = 125; m = 37;
        } else if (case_num <= 45) {
            n = 150; m = 15;
        } else {
            n = 150; m = 45;
        }
        
        case_filename << "_n" << n << "_m" << m << ".txt";
        string filename = data_dir + case_filename.str();
        string case_name = case_filename.str();
        case_name = case_name.substr(0, case_name.length() - 4); // Eliminar la extensión .txt
        
        // Obtener mejor coste conocido para este caso
        float best_known = best_costs[case_name];
        
        cout << "Ejecutando caso " << case_name << " (" << case_num << "/50)" << endl;
        
        try {
            // Crear estructura para este caso
            CaseResult case_result;
            case_result.case_name = case_name;
            case_result.n = n;
            case_result.m = m;
            case_result.best_known = best_known;
            
            // Cargar problema
            MDDProblem problem(filename);
            
            // 1. RandomSearch - Una ejecución
            cout << "  RandomSearch..." << flush;
            case_result.random_results.push_back(run_algorithm("random", &problem, max_evals, seeds[0], best_known));
            cout << " Fitness: " << case_result.random_results[0].fitness << endl;
            
            // 2. GreedySearch - Una ejecución
            cout << "  GreedySearch..." << flush;
            case_result.greedy_results.push_back(run_algorithm("greedy", &problem, max_evals, seeds[0], best_known));
            cout << " Fitness: " << case_result.greedy_results[0].fitness << endl;
            
            // 3. RandLS - Cinco ejecuciones
            cout << "  LocalSearch (randLS)..." << flush;
            for (long seed : seeds) {
                case_result.randls_results.push_back(run_algorithm("randls", &problem, max_evals, seed, best_known));
                cout << " " << case_result.randls_results.back().fitness << flush;
            }
            cout << endl;
            
            // 4. HeurLS - Cinco ejecuciones
            cout << "  LocalSearch (heurLS)..." << flush;
            for (long seed : seeds) {
                case_result.heurls_results.push_back(run_algorithm("heurls", &problem, max_evals, seed, best_known));
                cout << " " << case_result.heurls_results.back().fitness << flush;
            }
            cout << endl;
            
            // Añadir resultados del caso al vector general
            all_results.push_back(case_result);
            
            // Cada 5 casos, guardar resultados parciales (por si hay interrupción)
            if (case_num % 5 == 0 || case_num == 50) {
                save_detailed_results(all_results, "results_detailed.csv");
                save_case_results(all_results, "results_by_case.csv");
                save_size_results(all_results, "results_by_size.csv");
                save_global_results(all_results, "results_global.csv");
            }
            
        } catch (const exception& e) {
            cerr << "Error procesando caso " << case_name << ": " << e.what() << endl;
        }
    }
    
    cout << "Todos los experimentos completados." << endl;
    
    return 0;
} 