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

    //Help message for empty input
    if (argc == 1) {
        
        std::cout
        << "Input the parameters in the following way:\n"
        << "Requested expression such as 'SEND+MORE=MONEY.\n"
        << "Number of chromosomes (populations).\n"
        << "Crossover probability (with range of 0...1).\n"
        << "Mutation probability (with range of 0...1).\n"
        << "Type of fitness function: 1 = Edit Distance. 2 = Closeness.\n"
        << "Number of generations (0 for no limit)."
        << std::endl;
        return 0;
    }
    
    
    //Use current time as seed for random generator
    std::srand((unsigned)std::time(0));
    
    GeneticAlgorithm algorithm(std::stoi(argv[2]), std::stof(argv[3]), std::stof(argv[4]));
    
    std::cout << algorithm.FindSolution(argv[1], static_cast<Fitness::Type>(std::stoi(argv[5])), std::stoi(argv[6])) << std::endl;

    return 0;
}
