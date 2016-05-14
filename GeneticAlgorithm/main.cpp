//
//  main.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 13/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "GeneticAlgorithm.hpp"
#include "Fitness.hpp"
#include "Chromosome.hpp"
#include <iostream>
#include <ctime>

int main(int argc, const char * argv[]) {

    //Use current time as seed for random generator
    std::srand((unsigned)std::time(0));
    
    /*
     * The fitness can now estimate the score for
     * chromosomes based on information parsed from
     * the query (parameters and result strings).
     */
    Fitness fitness("SEND/MORE=MONEY"); //9341 * 1093 = 10434
    
    GeneticAlgorithm algorithm(1000, "SENDMORY", 0.5, 0.3);
    
    Chromosome solution = algorithm.FindSolution(fitness);
    
    std::cout << solution.Representation("MONEY");
    
    return 0;
}
