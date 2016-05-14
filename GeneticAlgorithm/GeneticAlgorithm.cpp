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

/**
 * Implementation.
 */
class GeneticAlgorithm::Impl {
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
    Impl(size_t population_size, float crossover_probability, float mutation_probability);
    
    /**
     * Finds the best solution to the query at the given number of generations.
     *
     * @param query         The query string.
     * @param type          The type of fitness to use.
     * @param generations   Number of generations to perform.
     */
    Chromosome FindSolution(const std::string& query, Fitness::Type type, size_t generations);

private:
    
    /**
     * Updates the chromosomes that are contained in member variables using the input 
     * fitness object. The function iterates over all the chromosomes and updates it's
     * scores (or replaces invalid chromosomes). If an optimal chromosome is found it 
     * returns it.
     *
     * @param fitness   The fitness to use for score calculation.
     * @return          The location to the optimal chromosome in member variables.
     */
    Chromosome* UpdateChromosomeScores(const Fitness& fitness);
    
    ///Contains all of the chromosomes that are paired to a score.
    std::vector<scored_chromosome> m_chromosomes;
    
    ///Stores the population size.
    size_t m_population_size;
    
    ///Stores the probability to perform a crossover.
    float m_crossover_probability;
    
    ///Stores the probability to perform a mutation.
    float m_mutation_probability;
    
    std::string m_alphabet;
};

#pragma mark - Implementation functions

GeneticAlgorithm::Impl::Impl(size_t population_size,
                             float crossover_probability,
                             float mutation_probability) :
m_population_size(population_size),
m_crossover_probability(crossover_probability),
m_mutation_probability(mutation_probability)
{ }

Chromosome* GeneticAlgorithm::Impl::UpdateChromosomeScores(const Fitness& fitness) {
    
    //Update Chromosomes so they contain valid chromosomes with scores
    for (auto begin = m_chromosomes.begin(), end = m_chromosomes.end() ;
         begin != end ;
         begin++) {
        
        begin->second = FindValidScoreOrReplace(fitness, begin->first, m_alphabet);
        
        //Check for valid results
        if (begin->second == fitness.OptimalScore())
            return &begin->first;
    }
    
    //Didnt find answer
    return NULL;
}

Chromosome GeneticAlgorithm::Impl::FindSolution(const std::string& query, Fitness::Type type, size_t generations) {

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
        
        
        Chromosome* result = UpdateChromosomeScores(*fitness);
        
        //In case a result was found return it
        if (result) {
            
            std::cout << "Generations: " << counted_generations << '\n';
            return *result;
        }

        //Sort so that the best chromosomes are the first
        std::sort(m_chromosomes.begin(), m_chromosomes.end(), [&](scored_chromosome& lhs, scored_chromosome& rhs){
            
            return fitness->Descending()
            ? lhs.second < rhs.second
            : lhs.second > rhs.second;
        });
        
        //Reached limit of generations
        if (counted_generations++ == generations) {
            
            std::cout << "Generations: " << counted_generations - 1 << '\n';
            return m_chromosomes.begin()->first;
        }
        
        //Perform changes to the chromosomes themselfs
        for (std::vector<scored_chromosome>::iterator begin = m_chromosomes.begin(), end = m_chromosomes.end() ;
             begin != end ;
             begin++) {
            
            //Mutate with a probability and take only better options
            Chromosome mutated = Chromosome::Mutate(begin->first, m_mutation_probability);
            try {
                
                size_t mutated_score = fitness->Score(mutated);

                if (fitness->Descending()
                    ? mutated_score < begin->second
                    : mutated_score > begin->second) {
                    
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
                
                try {
                    
                    size_t crossover_score = fitness->Score(crossover);

                    if (fitness->Descending()
                        ? crossover_score < begin->second
                        : crossover_score > begin->second) {
                    
                        begin->first = crossover;
                        begin->second = crossover_score;
                    }
                }
                catch (...) { }
            }
        }
    }
}

#pragma mark - GeneticAlgorithm functions

GeneticAlgorithm::GeneticAlgorithm(size_t population_size, float crossover_probability, float mutation_probability) :
m_pimpl(new Impl(population_size, crossover_probability, mutation_probability))
{ }

Chromosome GeneticAlgorithm::FindSolution(const std::string &query, Fitness::Type type, size_t generations) {
    return m_pimpl->FindSolution(query, type, generations);
}

GeneticAlgorithm::~GeneticAlgorithm() { }

