//
//  EditDistanceFitness.hpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 14/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef EditDistanceFitness_hpp
#define EditDistanceFitness_hpp
#include "Fitness.hpp"

/**
 * Works by estimating edit distance between two strings.
 */
class EditDistanceFitness : public Fitness {
public:
    
    EditDistanceFitness(const std::string& query);
    

protected:

    /**
     * Resolves the score between two strings. The higher the score, the better then match.
     *
     * @param estimated_result      The result produced by the chromosome.
     * @param real_result           The real result.
     * @return                      An estimation on how close the estimated result to the real result.
     */
    virtual size_t ResolveScore(const std::string& estimated_result, const std::string& real_result) const;
    
    /**
     * Resolves the best possible score for the real result.
     *
     * @param result    The final result that the chromosome is to match.
     * @return          The best score.
     */
    virtual size_t ResolveOptimalScore(const std::string& result) const;
    
};
#endif /* EditDistanceFitness_hpp */
