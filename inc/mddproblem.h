#pragma once
#include <problem.h>
#include <vector>
#include <string>

// Información para factorización específica de MDD
class MDDFactoringInfo : public SolutionFactoringInfo {
public:
    std::vector<float> sums; // Suma de distancias para cada elemento seleccionado
    
    MDDFactoringInfo(const std::vector<float>& s) : sums(s) {}
    MDDFactoringInfo(int size) : sums(size, 0.0f) {}
};

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

    // En inc/mddproblem.h
    int getN() const { return n; }
    
    // Método para obtener distancia entre dos elementos
    float getDistance(int i, int j) const { return distances[i][j]; }
    
    // Métodos obligatorios de Problem
    tFitness fitness(const tSolution &solution) override;
    
    // Métodos para factorización
    SolutionFactoringInfo* generateFactoringInfo(const tSolution &solution) override;
    
    tFitness fitness(const tSolution &solution,
                   SolutionFactoringInfo *solution_info,
                   unsigned pos_change, tDomain new_value) override;
                   
    void updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info,
                                  const tSolution &solution,
                                  unsigned pos_change,
                                  tDomain new_value) override;
    
    // Otros métodos obligatorios                
    tSolution createSolution() override;
    size_t getSolutionSize() override;
    std::pair<tDomain, tDomain> getSolutionDomainRange() override;
}; 