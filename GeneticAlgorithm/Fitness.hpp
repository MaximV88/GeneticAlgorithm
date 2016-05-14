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

    /**
     * Types of Fitness calculations that are available.
     */
    enum class Type {
        kEditDistance,
        kCloseness
    };
    
    /**
     * Factory function. 
     * Creates a Fitness based on the input type.
     *
     * @param query     The query that the fitness is based on.
     * @param type      Type of operation that the fitness has.
     */
    static Fitness* CreateFitness(const std::string& query, Fitness::Type type);
    
    /**
     * Calculates the score of the chromosome.
     *
     * @param chromosome    The chromosome to calculate the score for.
     * @return              Score of input chromosome.
     */
    size_t Score(const Chromosome& chromosome) const;
    
    /**
     * Calculates the optimal score for best possible chromosome.
     *
     * @return  Score of the perfect chromosome.
     */
    size_t OptimalScore() const;
    
    /**
     * Returns true if the scores are based on descending or ascending order.
     *
     * @return True if descending (lower is better), false if ascending (higher is better).
     */
    virtual bool Descending() const = 0;
    
    /**
     * Destructur.
     */
    virtual ~Fitness();
    
protected:
    
    /**
     * Types of mathematical operations that can be performed.
     */
    enum Operation {
        kAddition       = '+',
        kSubtraction    = '-',
        kMultiplication = '*',
        kDevision       = '/',
        kNone
    };
    
    /**
     * Constructor.
     *
     * @param query     The query that the fitness is based on.
     */
    Fitness(const std::string& query);
    
    /**
     * Resolves the score between two strings. The higher the score, the better then match.
     *
     * @param estimated_result      The result produced by the chromosome.
     * @param real_result           The real result.
     * @param chromosome            The chromosome that the score is calculated for.
     * @return                      An estimation on how close the estimated result to the real result.
     */
    virtual size_t ResolveScore(const std::string& estimated_result, const std::string& real_result, const Chromosome& chromosome) const = 0;
    
    /**
     * Resolves the best possible score for the real result.
     *
     * @param result        The final result that the chromosome is to match.
     * @return              The best score.
     */
    virtual size_t ResolveOptimalScore(const std::string& result) const = 0;

private:
    
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
    
};

#endif /* Fitness_hpp */
