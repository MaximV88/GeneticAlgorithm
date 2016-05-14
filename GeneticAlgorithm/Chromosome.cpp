//
//  Chromosome.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Chromosome.hpp"
#include "Utility.hpp"
#include <iterator>
#include <tgmath.h>
#include <vector>
#include <string>

Chromosome Chromosome::Crossover(const Chromosome &first, const Chromosome &second, float point) {
    
    elements crossed_over;
    
    //Insert first half
    size_t first_half = point * first.m_interpretations.size();
    
    elements::const_iterator first_half_end = first.m_interpretations.begin();
    std::advance(first_half_end, first_half);
    
    crossed_over.insert(first.m_interpretations.begin(), first_half_end);

    //Insert second half
    elements::const_iterator second_half_start = second.m_interpretations.begin();
    std::advance(second_half_start, first_half);

    crossed_over.insert(second_half_start, second.m_interpretations.end());

    return Chromosome(crossed_over);
}

Chromosome::Chromosome(const std::string& alphabet) {
    
    for (auto begin = alphabet.begin(), end = alphabet.end() ;
         begin != end ;
         begin ++) {

        
        m_interpretations.insert({{ *begin, std::rand() % 10 }});
    }
    
}

Chromosome::Chromosome(const elements& interpretations) :
m_interpretations(interpretations)
{ }

Chromosome Chromosome::Mutate(const Chromosome& chromosome, float probability) {
    
    elements mutated;
    
    for (auto& interpretation : chromosome.m_interpretations) {
        
        //Mutate to a range of 0-9
        mutated.insert({{ interpretation.first, (utility::ThrowDice(probability)) ? std::rand() % 10 : interpretation.second }});
    }
    
    return Chromosome(mutated);
}

size_t Chromosome::Decode(const std::string &input) const {
    
    //Iterate over positions of characters and find corresponding values in map
    size_t result = 0;
    size_t index = input.length() - 1;
    for (std::string::const_iterator begin = input.begin(), end = input.end() ;
         begin != end ;
         begin ++, index--) {
        
        auto search = m_interpretations.find(*begin);
        if (search == m_interpretations.end()) {
            
            //Throw exception to notify that the chromosome is invalid as it is missing a mapping to a required value
            throw std::runtime_error("Chromosome cannot decode value due to missing representations.");
        }
        else result += std::pow(10, index) * search->second;
    }
    
    return result;
}

std::list<std::string> Chromosome::Encode(size_t value) const {
    
    std::string converted = std::to_string(value);
    std::list<std::string> results;
    
    for (std::string::const_iterator begin = converted.begin(), end = converted.end() ;
         begin != end ;
         begin ++) {
        
        //Find possible matches to the current letter
        std::vector<char> matches;
        bool found = false;
        for (const auto& element : m_interpretations) {
            if (element.second == (*begin - '0')) {
                matches.push_back(element.first);
                found = true;
            }
        }

        //Cant continue due to invalid representation
        if (!found) throw std::runtime_error("Chromosome cannot encode value due to missing representations.");
        
        if (results.empty()) {
        
            //If no results are found then populate with current matches
            for (const auto match : matches)
                results.push_back(std::string(1, match));
            
        }
        else {

            std::list<std::string> updated_results;
            
            //Otherwise add the matches to each result found thus far
            for (const auto& result : results)
                for (const auto match : matches)
                    updated_results.push_back(result + match);

            //Now replace results with the updated results
            results = updated_results;
        }
    }
 
    return results;
}

short Chromosome::Value(char letter) const {
    return m_interpretations.find(letter)->second;
}

std::string Chromosome::Representation(const std::string &representation) const {
    return "";
}
