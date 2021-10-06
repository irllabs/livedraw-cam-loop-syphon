//
//  utils.cpp
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/19/17.
//
//

#include "Utils.h"

float Utils::scale(float x, float inmin, float inmax, float outmin, float outmax) {
    float output = (x-inmin)/(inmax-inmin)*(outmax-outmin)+outmin;
    return output;
}

