//
//  constants.h
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/16/17.
//
//

#pragma once

#include "ofMain.h"

#ifndef constants_h
#define constants_h


#endif /* constants_h */

#define CAM_W               1280
#define CAM_H               720
#define CAM_FPS             30

//recording layers
#define LAYER_COUNT         5
#define FRAME_COUNT         100

//render window
#define WINDOW_W            1400
#define WINDOW_H            650

//MIDI Controller Numbers
#define MIDI_PORT           0
#define MIDI_REC            64
#define MIDI_SCALE          72
#define MIDI_XPOS           80
#define MIDI_YPOS           90

#define MIDI_SHADER_BYPASS   100
#define MIDI_SHADER_THRESH   101
#define MIDI_SHADER_SOFT     102
#define MIDI_SHADER_INVERT   103

//default layer settings
#define LAYER_SCALE         .2
#define SHADER_THRESH       0.5
#define SHADER_INVERT       0
#define SHADER_SOFT         0.05
#define SHADER_ACTIVE       1
