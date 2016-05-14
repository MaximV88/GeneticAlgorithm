//
//  Chromosome.hpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef Chromosome_hpp
#define Chromosome_hpp
#include <map>
#include <list>
#include <string>

class Chromosome {
public:

    typedef std::map<char, short> elements;
    
    /**
     * Crossover selects genes from parent chromosomes and creates a new offspring.
     *
     * @param first     The first chromosome.
     * @param second    The second chromosome.
     * @param point     The crossover point.
     * @return          A new child chromosome that is composed from parts of both parents.
     */
    static Chromosome Crossover(const Chromosome& first,
                                const Chromosome& second,
                                float point = 0.5);
    

    /**
     * Mutation changes randomly the chromosome, this is to prevent falling all
     * solutions in population into a local optimum of solved problem.
     *
     * @param probability   The probability of performing mutation of each element of the chromosome.
     */
    static Chromosome Mutate(const Chromosome& chromosome, float probability = 0.1);
    
    /**
     * Constructor.
     * Creates a chromosome with a random value per each letter.
     *
     * @param alphabet The alphabet that the chromosome works on.
     */
    Chromosome(const std::string& alphabet);
    
    /**
     * Constructor.
     * Creates a chromosome with specific values.
     */
    Chromosome(const elements& interpretations);

    
    size_t Decode(const std::string& input) const;
    
    std::list<std::string> Encode(size_t value) const;
    
    short Value(char letter) const;
    
    std::string Representation(const std::string& representation) const;
    
    friend std::ostream& operator<<(std::ostream& out, const Chromosome& chromosome);
    
private:
        
    elements m_interpretations;
    
};
#endif /* Chromosome_hpp */
