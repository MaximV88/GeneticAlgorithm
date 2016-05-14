//
//  GeneticAlgorithm.hpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp
#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include "Chromosome.hpp"
#include "Fitness.hpp"

class GeneticAlgorithm {
public:
    
    /**
     *
     * @param population_size           says how many chromosomes are in population (in one generation).
     *                                  If there are too few chromosomes, GA have a few possibilities to
     *                                  perform crossover and only a small part of search space is explored.
     *
     * @param crossover_probability     says how often will be crossover performed.
     *                                  If there is no crossover, offspring is exact copy of parents.
     *                                  If there is a crossover, offspring is made
     *                                  from parts of parents' chromosome.
     *
     * @param mutation_probability      says how often will be parts of chromosome mutated.
     *                                  If there is no mutation, offspring is taken after crossover (or copy) 
     *                                  without any change. If mutation is performed,
     *                                  part of chromosome is changed.
     *
     */
    GeneticAlgorithm(size_t population_size,
                     float crossover_probability = 1,
                     float mutation_probability = 0.1);
    
    /**
     * Finds the best solution to the query at the given number of generations.
     *
     * @param query         The query string.
     * @param type          The type of fitness to use.
     * @param generations   Number of generations to perform.
     */
    Chromosome FindSolution(const std::string& query,
                            Fitness::Type type,
                            size_t generations = 0);
    
    /**
     * Destructor.
     */
    ~GeneticAlgorithm();
    
private:
    
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
    
};
#endif /* GeneticAlgorithm_hpp */
