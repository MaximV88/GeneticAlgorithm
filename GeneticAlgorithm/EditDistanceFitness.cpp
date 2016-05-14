//
//  EditDistanceFitness.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 14/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "EditDistanceFitness.hpp"
#include "Chromosome.hpp"

EditDistanceFitness::EditDistanceFitness(const std::string& query) :
Fitness(query)
{ }

size_t EditDistanceFitness::ResolveScore(const std::list<std::string> &estimated_results) const {
    
    //Compare distance between real result and estimated one - get best value which is lowest distance
    std::vector<size_t> scores;
    scores.reserve(estimated_results.size());
    
    for (const auto& estimated_result : estimated_results)
        scores.push_back(EditDistance(estimated_result, GetResult()));
    
    int result = static_cast<int>(OptimalScore() - *std::min_element(scores.begin(), scores.end()));
    
    return result;
}

size_t EditDistanceFitness::OptimalScore() const {
    
    //Save for later comparisons with edit distance
    return GetResult().length();
}

size_t EditDistanceFitness::EditDistance(const std::string& s1, const std::string& s2) const {
    
    const std::size_t len1 = s1.size(), len2 = s2.size();
    std::vector<std::vector<size_t>> d(len1 + 1, std::vector<size_t>(len2 + 1));
    
    d[0][0] = 0;
    for(size_t i = 1; i <= len1; ++i) d[i][0] = i;
    for(size_t i = 1; i <= len2; ++i) d[0][i] = i;
    
    for(size_t i = 1; i <= len1; ++i) {
        for(size_t j = 1; j <= len2; ++j) {
            
            d[i][j] = std::min({
                d[i - 1][j] + 1,
                d[i][j - 1] + 1,
                d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)
            });
            
        }
    }
    
    return d[len1][len2];
}
