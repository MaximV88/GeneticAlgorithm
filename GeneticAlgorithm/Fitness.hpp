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
#include <list>
class Chromosome;

class Fitness {
public:

    enum class Type {
        kEditDistance
    };
    
    static Fitness* CreateFitness(const std::string& query, Fitness::Type type);
    
    Fitness(const std::string& query);
    
    size_t Score(const Chromosome& chromosome) const;
    
    virtual size_t OptimalScore() const = 0;
    
protected:
    
    enum class Operation {
        kAddition,
        kSubtraction,
        kMultiplication,
        kDevision,
        kNone
    };
    
    virtual size_t ResolveScore(const std::list<std::string>& estimated_results) const = 0;
    
    std::vector<Operation> m_operations;
    std::vector<std::string> m_parameters;
    
    virtual const std::string& GetResult() const;
    
    void Interpret(const std::string& query);
        
    Operation ParseOperation(char operation) const;
    
private:
    
    std::string m_result;

    
};

#endif /* Fitness_hpp */
