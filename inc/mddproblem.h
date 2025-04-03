#pragma once
#include <problem.h>
#include <vector>
#include <string>

class MDDProblem : public Problem {
private:
    int n;      // número total de elementos
    int m;      // número de elementos a seleccionar
    std::vector<std::vector<float>> distances;  // matriz de distancias
    std::string filename;

public:
    MDDProblem(const std::string& filename);
    
    // Método para obtener m
    int getM() const { return m; }
    
    // Métodos obligatorios de Problem
    tFitness fitness(const tSolution &solution) override;
    tSolution createSolution() override;
    size_t getSolutionSize() override;
    std::pair<tDomain, tDomain> getSolutionDomainRange() override;
}; 