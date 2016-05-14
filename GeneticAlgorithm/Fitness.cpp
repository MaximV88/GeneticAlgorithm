//
//  Fitness.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Fitness.hpp"
#include "EditDistanceFitness.hpp"
#include "Chromosome.hpp"

Fitness* Fitness::CreateFitness(const std::string& query, Fitness::Type type) {
    
    switch (type) {
        case Type::kEditDistance: return new EditDistanceFitness(query);
    }
}

Fitness::Fitness(const std::string& query) {
    
    //Populate the parameters and result strings
    Interpret(query);
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

size_t Fitness::Score(const Chromosome &chromosome) const {
    
    //First find the result using the chromosome's interpretation of the string
    size_t total_value = 0;
    size_t operation_index = 0;
    for (std::vector<std::string>::const_iterator begin = m_parameters.begin(), end = m_parameters.end() ;
         begin != end ;
         begin ++) {
        
        //Number starting with 0 is illigal
        if (chromosome.Value(begin->at(0)) == 0)
            throw std::runtime_error("Chromosome contains illigal numbers placement that results with first character starting at 0");
        
        switch (m_operations.at(operation_index++)) {
            case Operation::kMultiplication:    total_value += chromosome.Decode(*begin) * chromosome.Decode(*++begin); break;
            case Operation::kDevision: {
                
                size_t denominator = chromosome.Decode(*++begin);
                
                //Devision by 0 is illigal
                if (denominator == 0)
                    throw std::runtime_error("Chromosome contains illigal numbers placement that results in devision by 0");
                
                total_value += chromosome.Decode(*begin) / denominator;
                
                break;
            }
            case Operation::kSubtraction:       total_value += chromosome.Decode(*begin) - chromosome.Decode(*++begin); break;
            case Operation::kAddition:          total_value += chromosome.Decode(*begin) + chromosome.Decode(*++begin); break;
            case Operation::kNone: break;
        }
    }
    
    //Get result string using the chromosome's interpreration
    return ResolveScore(chromosome.Encode(total_value));
}

Fitness::Operation Fitness::ParseOperation(char operation) const {
    
    //Match enum according to related operation
    if (operation == '+')       return Fitness::Operation::kAddition;
    else if (operation == '-')  return Fitness::Operation::kSubtraction;
    else if (operation == '/')  return Fitness::Operation::kDevision;
    else if (operation == '*')  return Fitness::Operation::kMultiplication;
    else                        return Fitness::Operation::kNone;
}

const std::string& Fitness::GetResult() const { return m_result; }
