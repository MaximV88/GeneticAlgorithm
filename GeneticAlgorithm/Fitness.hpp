//
//  Fitness.hpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef Fitness_hpp
#define Fitness_hpp
#include <stdlib.h>
#include <vector>
#include <string>
class Chromosome;

class Fitness {
public:

    Fitness(const std::string& query);
    
    /**
     * Works by estimating edit distance between two strings.
     */
    size_t Score(const Chromosome& chromosome) const;
    
    size_t OptimalScore() const;
    
private:
    
    enum class Operation {
        kAddition,
        kSubtraction,
        kMultiplication,
        kDevision,
        kNone
    };
    
    size_t m_optimal_score;
    
    void Interpret(const std::string& query);
    
    size_t EditDistance(const std::string& s1, const std::string& s2) const;
    
    Operation ParseOperation(char operation) const;
    
    std::vector<Operation> m_operations;
    std::vector<std::string> m_parameters;
    std::string m_result;
    
};

#endif /* Fitness_hpp */
