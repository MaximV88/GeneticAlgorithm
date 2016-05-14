//
//  GeneticAlgorithm.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "GeneticAlgorithm.hpp"
#include "Fitness.hpp"
#include "Utility.hpp"
#include <vector>
#include <iostream>

size_t FindValidScoreOrReplace(const Fitness& fitness, Chromosome& chromosome, const std::string& alphabet) {
    
    size_t score = 0;

    bool found_valid = false;
    while (!found_valid) {
        
        found_valid = true;
        try { score = fitness.Score(chromosome); }
        catch (...) {
            
            //The chromosome is invalid - replace it
            chromosome = Chromosome(alphabet);
            
            //Retry to find out if the replacement chromosome is valid
            found_valid = false;
        }
    }

    return score;
}

GeneticAlgorithm::GeneticAlgorithm(size_t population_size,
                                   const std::string& alphabet,
                                   float crossover_probability,
                                   float mutation_probability) :
m_crossover_probability(crossover_probability),
m_mutation_probability(mutation_probability),
m_alphabet(alphabet) {
    
    //Create chromosomes with random values for each letter
    for (size_t index = 0 ; index < population_size ; index++)
        m_chromosomes.push_back(scored_chromosome(Chromosome(alphabet), 0));
}

Chromosome GeneticAlgorithm::FindSolution(const Fitness& fitness) {

    size_t generations = 0;
    
    while (true) {
        
        ++generations;

        //Update Chromosomes so they contain valid chromosomes with scores
        for (std::vector<scored_chromosome>::iterator begin = m_chromosomes.begin(), end = m_chromosomes.end() ;
             begin != end ;
             begin++) {
            
            begin->second = FindValidScoreOrReplace(fitness, begin->first, m_alphabet);
            
            //Check for valid results
            if (begin->second == fitness.OptimalScore())
                return begin->first;
            
        }

        std::sort(m_chromosomes.begin(), m_chromosomes.end(), [&](scored_chromosome& lhs, scored_chromosome& rhs){
            
            //The order should be descending
            return lhs.second > rhs.second;
        });
        
        //Perform changes to the chromosomes themselfs after the first 20%
        for (std::vector<scored_chromosome>::iterator begin = m_chromosomes.begin(), end = m_chromosomes.end() ;
             begin != end ;
             begin++) {
            
            //Mutate with a probability and take only better options
            Chromosome mutated = Chromosome::Mutate(begin->first, m_mutation_probability);
            try { if (fitness.Score(mutated) > begin->second) begin->first = mutated; }
            catch (...) { }
            
            //Crossover with a probability only if its beneficial
            if (begin != m_chromosomes.begin() && utility::ThrowDice(m_crossover_probability)) {
                Chromosome crossover = Chromosome::Crossover(begin->first, (m_chromosomes.begin() + arc4random() % m_chromosomes.size())->first, utility::RandomProbability());
                
                try { if (fitness.Score(crossover) > begin->second) begin->first = crossover; }
                catch (...) { }
            }
            
        }
        
        if (generations % 100 == 0) std::cout << generations << '\n';
    }
    
}
