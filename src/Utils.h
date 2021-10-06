//
//  utils.hpp
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/19/17.
//
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>

class Utils{
public:
    //frames of video
    static float scale(float x, float inmin, float inmax, float outmin, float outmax);
};


#endif /* utils_hpp */
