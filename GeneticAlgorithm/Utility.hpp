//
//  Utility.hpp
//  GeneticAlgorithm
//
//  Created by Maxim Vainshtein on 14/05/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp
#include <string>
#define NAMESPACE_UTILITY_BEGIN namespace utility {
#define NAMESPACE_UTILITY_END }

NAMESPACE_UTILITY_BEGIN

bool ThrowDice(float probability);

/**
 * Returns a probability between 0-1.
 */
float RandomProbability();

/**
 * Returns the letters that make up the input as a set.
 */
std::string Alphabet(const std::string& input);

NAMESPACE_UTILITY_END
#endif /* Utility_hpp */
