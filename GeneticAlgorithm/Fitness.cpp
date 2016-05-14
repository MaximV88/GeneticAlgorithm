//
//  Fitness.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Fitness.hpp"
#include "Chromosome.hpp"
#define MATCH_PRICE 2

Fitness::Fitness(const std::string& query) {
    
    //Populate the parameters and result strings
    Interpret(query);
}

size_t Fitness::Score(const Chromosome &chromosome) const {
    
    //First find the result using the chromosome's interpretation of the string
    size_t total_value = 0;
    size_t operation_index = 0;
    for (std::vector<std::string>::const_iterator begin = m_parameters.begin(), end = m_parameters.end() ;
         begin != end ;
         begin ++) {
    
        //Manual score filtration using logic
        if (chromosome.Value(begin->at(0)) == 0) throw std::runtime_error("Chromosome contains illigal numbers placement");
        
        switch (m_operations.at(operation_index++)) {
            case Operation::kMultiplication:    total_value += chromosome.Decode(*begin) * chromosome.Decode(*++begin); break;
            case Operation::kDevision:          total_value += chromosome.Decode(*begin) / chromosome.Decode(*++begin); break;
            case Operation::kSubtraction:       total_value += chromosome.Decode(*begin) - chromosome.Decode(*++begin); break;
            case Operation::kAddition:          total_value += chromosome.Decode(*begin) + chromosome.Decode(*++begin); break;
            case Operation::kNone: break;
        }
    }
    
    //Get result string using the chromosome's interpreration
    std::list<std::string> estimated_results = chromosome.Encode(total_value);
    
    //Compare distance between real result and estimated one - get best value which is lowest distance
    std::vector<size_t> scores;
    scores.reserve(estimated_results.size());
    
    for (const auto& estimated_result : estimated_results)
        scores.push_back(EditDistance(estimated_result, m_result));
    
    return (m_optimal_score - *std::min_element(scores.begin(), scores.end()));
}

void Fitness::Interpret(const std::string &query) {
    
    //Get the parameters from the query
    std::string::size_type last_paramter_end = 0;
    
    while (true) {
        
        //First stop at an operator
        std::string::size_type position = query.find_first_of("+-/*=", last_paramter_end);
        
        if (position == std::string::npos) {
            
            //This is the last part - which is the result
            m_result = std::string(query.begin() + last_paramter_end, query.end());
            m_optimal_score = m_result.length();
            break;
        }
        else {
            
            //This must be a parameter - get the current section
            m_parameters.push_back(std::string(query.begin() + last_paramter_end,
                                               query.begin() + position));
            
            //Get the operation
            m_operations.push_back(ParseOperation(*(query.begin() + position)));
            
            //Increment the position so it wont take into account the operator
            last_paramter_end = position + 1;
        }
    }
}

Fitness::Operation Fitness::ParseOperation(char operation) const {
    
    //Match enum according to related operation
    if (operation == '+')       return Fitness::Operation::kAddition;
    else if (operation == '-')  return Fitness::Operation::kSubtraction;
    else if (operation == '/')  return Fitness::Operation::kDevision;
    else if (operation == '*')  return Fitness::Operation::kMultiplication;
    else                        return Fitness::Operation::kNone;
}

size_t Fitness::OptimalScore() const { return m_optimal_score; }

size_t Fitness::EditDistance(const std::string& s1, const std::string& s2) const {
    
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