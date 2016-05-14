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
    
    GeneticAlgorithm algorithm(100, 0.5, 0.01);
    
    std::cout << algorithm.FindSolution("SEND+MORE=MONEY", Fitness::Type::kCloseness, 0) << std::endl;

    return 0;
}
