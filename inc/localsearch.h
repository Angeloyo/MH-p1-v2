#pragma once
#include <mh.h>

enum ExplorationMode {
    randLS,   // Exploración aleatoria del entorno
    heurLS    // Exploración basada en heurística
};

/**
 * Implementación de Búsqueda Local para MDD
 * - Usa el operador de intercambio (Int)
 * - Aplica estrategia del primer mejor
 */
class LocalSearch : public MH {
private:
    ExplorationMode mode; // Modo de exploración del entorno
    
public:
    LocalSearch(ExplorationMode mode = randLS);
    virtual ~LocalSearch() {}
    
    /**
     * Busca la mejor solución usando búsqueda local
     * 
     * @param problem El problema a optimizar
     * @param maxevals Máximo número de evaluaciones permitidas
     * @return Resultado con la mejor solución encontrada y su fitness
     */
    ResultMH optimize(Problem *problem, int maxevals) override;
}; 