//
//  Fitness.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Fitness.hpp"
#include "EditDistanceFitness.hpp"
#include "ClosenessFitness.hpp"
#include "Chromosome.hpp"
#include <algorithm>

/**
 * Implementation.
 */
class Fitness::Impl {
public:
    
    /**
     * Constructor.
     *
     * @param query     The query that the fitness is based on.
     * @param parent    The parent of the implementation.
     */
    Impl(const std::string& query, Fitness& parent);
    
    /**
     * Calculates the score of the chromosome.
     *
     * @param chromosome    The chromosome to calculate the score for.
     * @return              Score of input chromosome.
     */
    size_t Score(const Chromosome& chromosome) const;
    
    /**
     * Calculates the optimal score for best possible chromosome.
     *
     * @return  Score of the perfect chromosome.
     */
    size_t OptimalScore() const;

private:
    
    /**
     * Parses the input query into managable pieces of information,
     * populates member variables.
     *
     * @param query     The query that the fitness needs to perform on.
     */
    void Interpret(const std::string& query);
    
    ///Reference to the parent of the implementation.
    Fitness& m_parent;
    
    ///Contains all the operations that are to be made.
    std::vector<Operation> m_operations;
    
    ///Contains all the parameters that the operations are performed on.
    std::vector<std::string> m_parameters;
    
    ///Stores the final result.
    std::string m_result;
    
};

#pragma mark - Implementation functions

Fitness::Impl::Impl(const std::string& query, Fitness& parent) :
m_parent(parent) {
    
    //Populate the parameters and result strings
    Interpret(query);
}

void Fitness::Impl::Interpret(const std::string &query) {
    
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
            m_operations.push_back(static_cast<Operation>(*(query.begin() + position)));
            
            //Increment the position so it wont take into account the operator
            last_paramter_end = position + 1;
        }
    }
}

size_t Fitness::Impl::Score(const Chromosome &chromosome) const {
    
    //First find the result using the chromosome's interpretation of the string
    int total_value = 0;
    size_t operation_index = 0;
    for (std::vector<std::string>::const_iterator begin = m_parameters.begin(), end = m_parameters.end() ;
         begin != end ;
         begin ++) {
        
        //Number starting with 0 is illigal
        auto current_parameter = begin;
        auto next_parameter = ++begin;
        
        if (((current_parameter->length() > 1) && (chromosome.Value(current_parameter->at(0)) == 0)) ||
            ((next_parameter->length() > 1) && (chromosome.Value(next_parameter->at(0)) == 0)))
            throw std::runtime_error("Chromosome contains illigal numbers placement that results with first character starting at 0");
        
        
        switch (m_operations.at(operation_index++)) {
            case Operation::kMultiplication:    total_value += chromosome.Decode(*current_parameter) * chromosome.Decode(*next_parameter); break;
            case Operation::kDevision: {
                
                size_t denominator = chromosome.Decode(*next_parameter);
                
                //Devision by 0 is illigal - possible for single length parameters
                if (denominator == 0)
                    throw std::runtime_error("Chromosome contains illigal numbers placement that results in devision by 0");
                
                total_value += chromosome.Decode(*current_parameter) / denominator;
                
                break;
            }
            case Operation::kSubtraction:       total_value += chromosome.Decode(*current_parameter) - chromosome.Decode(*next_parameter); break;
            case Operation::kAddition:          total_value += chromosome.Decode(*current_parameter) + chromosome.Decode(*next_parameter); break;
            case Operation::kNone: break;
        }
    }
    
    //Get result string using the chromosome's interpreration
    std::list<std::string> estimated_results = chromosome.Encode(total_value);
    
    //Compare distance between real result and estimated one - get best value which is lowest distance
    std::vector<size_t> scores;
    scores.reserve(estimated_results.size());
    
    for (const auto& estimated_result : estimated_results)
        scores.push_back(m_parent.ResolveScore(estimated_result, m_result, chromosome));
        
    return static_cast<int>(*std::max_element(scores.begin(), scores.end()));
}

size_t Fitness::Impl::OptimalScore() const { return m_parent.ResolveOptimalScore(m_result); }

#pragma mark - Fitness functions

Fitness* Fitness::CreateFitness(const std::string& query, Fitness::Type type) {
    
    switch (type) {
        case Type::kEditDistance:   return new EditDistanceFitness(query);
        case Type::kCloseness:      return new ClosenessFitness(query);
    }
}

Fitness::Fitness(const std::string& query) :
m_pimpl(new Impl(query, *this))
{ }

Fitness::~Fitness() { }

size_t Fitness::Score(const Chromosome &chromosome) const {
    return m_pimpl->Score(chromosome);
}

size_t Fitness::OptimalScore() const {
    return m_pimpl->OptimalScore();
}
