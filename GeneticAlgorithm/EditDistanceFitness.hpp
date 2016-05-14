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

class EditDistanceFitness : public Fitness {
public:
    
    EditDistanceFitness(const std::string& query);
    
    /**
     * Works by estimating edit distance between two strings.
     */
    virtual size_t ResolveScore(const std::list<std::string>& estimated_results) const;
  
    virtual size_t OptimalScore() const;

protected:
    
    size_t EditDistance(const std::string& s1, const std::string& s2) const;
    
};
#endif /* EditDistanceFitness_hpp */
