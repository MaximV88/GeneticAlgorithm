//
//  ClosenessFitness.hpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 14/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef ClosenessFitness_hpp
#define ClosenessFitness_hpp
#include "Fitness.hpp"

class ClosenessFitness : public Fitness {
public:
    
    /**
     * Constructor.
     *
     * @param query     The query that the fitness is based on.
     */
    ClosenessFitness(const std::string& query);
    
    /**
     * Returns true if the scores are based on descending or ascending order.
     *
     * @return True if descending (lower is better), false if ascending (higher is better).
     */
    virtual bool Descending() const;
    
protected:
    
    /**
     * Resolves the score between two strings. The higher the score, the better then match.
     *
     * @param estimated_result      The result produced by the chromosome.
     * @param real_result           The real result.
     * @param chromosome            The chromosome that the score is calculated for.
     * @return                      An estimation on how close the estimated result to the real result.
     */
    size_t ResolveScore(const std::string& estimated_result, const std::string& real_result, const Chromosome& chromosome) const;
    
    /**
     * Resolves the best possible score for the real result.
     *
     * @param result        The final result that the chromosome is to match.
     * @return              The best score.
     */
    virtual size_t ResolveOptimalScore(const std::string& result) const;

    
};

#endif /* ClosenessFitness_hpp */
