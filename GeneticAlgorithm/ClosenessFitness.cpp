//
//  ClosenessFitness.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 14/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "ClosenessFitness.hpp"
#include "Chromosome.hpp"
#include <algorithm>

ClosenessFitness::ClosenessFitness(const std::string& query) :
Fitness(query)
{ }

size_t ClosenessFitness::ResolveScore(const std::string& estimated_result,
                                         const std::string& real_result,
                                         const Chromosome& chromosome) const {

    return abs(static_cast<int>(chromosome.Decode(estimated_result)) -
               static_cast<int>(chromosome.Decode(real_result)));
}

size_t ClosenessFitness::ResolveOptimalScore(const std::string &result) const {
    return 0;
}

bool ClosenessFitness::Descending() const { return true; }