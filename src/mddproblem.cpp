#include "mddproblem.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random.hpp>

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
    // Calcular sumas para elementos seleccionados
    vector<float> sums;
    
    for (int i = 0; i < n; i++) {
        if (solution[i]) {
            float sum = 0.0;
            for (int j = 0; j < n; j++) {
                if (i != j && solution[j]) {
                    sum += distances[i][j];
                }
            }
            sums.push_back(sum);
        }
    }
    
    // Dispersión = max - min
    float maxSum = *max_element(sums.begin(), sums.end());
    float minSum = *min_element(sums.begin(), sums.end());
    
    return maxSum - minSum;
}

tSolution MDDProblem::createSolution() {
    tSolution solution(n, false); // Inicialmente nada seleccionado
    
    // Seleccionar m elementos aleatoriamente
    vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }
    
    Random::shuffle(indices);
    
    for (int i = 0; i < m; i++) {
        solution[indices[i]] = true;
    }
    
    return solution;
}

size_t MDDProblem::getSolutionSize() {
    return n;
}

std::pair<tDomain, tDomain> MDDProblem::getSolutionDomainRange() {
    return std::make_pair(false, true);
} 