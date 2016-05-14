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

std::string special_letters = "/*+-=";

bool utility::ThrowDice(float probability) {
    return RandomProbability() < probability;
}

float utility::RandomProbability() {
    return (double)std::rand() / RAND_MAX;
}

std::string utility::Alphabet(const std::string &input) {
    
    //Copy the input and remove duplicates
    std::string alphabet = input;
    
    std::sort(alphabet.begin(), alphabet.end());
    alphabet.erase(std::unique(alphabet.begin(), alphabet.end()), alphabet.end());
    
    //Remove special characters
    alphabet.erase(std::remove_if(alphabet.begin(), alphabet.end(), [&](char letter) {
        return (special_letters.find(letter) != std::string::npos);
    }), alphabet.end());
    
    return alphabet;
}