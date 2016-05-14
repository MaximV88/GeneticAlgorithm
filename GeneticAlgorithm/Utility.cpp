//
//  Utility.cpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 14/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Utility.hpp"
#include <iostream>
#include <ctime>

using namespace utility;

bool utility::ThrowDice(float probability) {
    return RandomProbability() < probability;
}

float utility::RandomProbability() {
    return (double)std::rand() / RAND_MAX;
}