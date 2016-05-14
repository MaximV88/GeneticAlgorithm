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
#include "Chromosome.hpp"
class Fitness;
class Chromosome;


class GeneticAlgorithm {
public:

    typedef std::pair<Chromosome, size_t> scored_chromosome;
    
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
                     const std::string& alphabet,
                     float crossover_probability = 1,
                     float mutation_probability = 0.1);
    
    Chromosome FindSolution(const Fitness& fitness);
    
private:
    
    std::vector<scored_chromosome> m_chromosomes;
    
    float m_crossover_probability;
    float m_mutation_probability;
    
    std::string m_alphabet;
    
};
#endif /* GeneticAlgorithm_hpp */
