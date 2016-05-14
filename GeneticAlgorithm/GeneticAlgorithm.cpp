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
#include <cmath>

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
                                   float crossover_probability,
                                   float mutation_probability) :
m_population_size(population_size),
m_crossover_probability(crossover_probability),
m_mutation_probability(mutation_probability) {
    
}

Chromosome GeneticAlgorithm::FindSolution(const std::string& query, Fitness::Type type, size_t generations) {

    m_alphabet = utility::Alphabet(query);
    
    /*
     * The fitness can now estimate the score for
     * chromosomes based on information parsed from
     * the query (parameters and result strings).
     */
    std::unique_ptr<Fitness> fitness(Fitness::CreateFitness(query, type));
    
    //Create chromosomes with random values for each letter
    for (size_t index = 0 ; index < m_population_size ; index++)
        m_chromosomes.push_back(scored_chromosome(Chromosome(m_alphabet), 0));
    
    //Ensuring that the number of counted generations is above 0 means that it will be equal and stop
    size_t counted_generations = generations ? 0 : 1;
    
    while (true) {
        
        //Update Chromosomes so they contain valid chromosomes with scores
        for (std::vector<scored_chromosome>::iterator begin = m_chromosomes.begin(), end = m_chromosomes.end() ;
             begin != end ;
             begin++) {
            
            begin->second = FindValidScoreOrReplace(*fitness, begin->first, m_alphabet);
            
            //Check for valid results
            if (begin->second == fitness->OptimalScore()) {
             
                std::cout << "Generations: " << counted_generations << '\n';
                return begin->first;
            }
            
        }

        std::sort(m_chromosomes.begin(), m_chromosomes.end(), [&](scored_chromosome& lhs, scored_chromosome& rhs){
            
            //The order should be descending
            return lhs.second > rhs.second;
        });
        
        //Reached limit of generations
        if (counted_generations++ == generations) {
            
            std::cout << "Generations: " << counted_generations << '\n';
            return m_chromosomes.begin()->first;
        }
        
        //Perform changes to the chromosomes themselfs
        for (std::vector<scored_chromosome>::iterator begin = m_chromosomes.begin(), end = m_chromosomes.end() ;
             begin != end ;
             begin++) {
            
            //Mutate with a probability and take only better options
            Chromosome mutated = Chromosome::Mutate(begin->first, m_mutation_probability);
            size_t mutated_score;
            try {
                
                if ((mutated_score = fitness->Score(mutated)) > begin->second) {
                    
                    begin->first = mutated;
                    begin->second = mutated_score;
                }
            }
            catch (...) { }
            
            //Crossover with a probability only if its beneficial
            if (utility::ThrowDice(m_crossover_probability)) {
                
                auto first_chromosome = begin;
                auto second_chromosome = m_chromosomes.begin() + arc4random() % static_cast<int>(std::round(m_chromosomes.size() / 5.0f));
                
                //Avoid crossing over with self
                if (first_chromosome == second_chromosome) continue;
                
                //Perform flips to chromosomes in order to randomize parts that are exchanged to avoid local maximum
                if (utility::ThrowDice(0.5)) { auto temp = first_chromosome; first_chromosome = second_chromosome; second_chromosome = temp; }
                
                Chromosome crossover = Chromosome::Crossover(first_chromosome->first, second_chromosome->first, 0.5);
                
                size_t crossover_score = 0;
                try {
                    
                    if ((crossover_score = fitness->Score(crossover)) > begin->second) {
                        begin->first = crossover;
                        begin->second = crossover_score;
                    }
                }
                catch (...) { }
            }
            
        }
    }
}
