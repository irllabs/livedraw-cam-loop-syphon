//
//  vidLayer.h
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/16/17.
//
//

#ifndef vidLayer_hpp
#define vidLayer_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxSyphon.h"

class vidLayer{
public:
    //frames of video
    //array of textures
    //vector <ofTexture> vidFrames;
    
    //array of FBOs
    vector <ofFbo> vidFrames;
    
    //properties
    int x;
    int y;
    float scale;            //scale, 1 = camera input size
    float speed;            //playback speed, 1 = normal playspeed
    int myID;               //layer id
    int state;              //0 = stop | 1 = record | 2 = play
    int playHead;           //playhead location in frames
    int playDir;            //1 for forward; -1 for reverse
    int recHead;            //record location in frames
    int recCount;           //record frame counter
    int recMax;             //maximum buffer size
    float opacity;
    
    ofxSyphonServer syphon;
    
    //methods
    void setup(int thisID, int bufSize);
    void draw(ofTexture thisTexture);
    void update(ofTexture thisTexture);
    
    void setState(int thisState);
    int getState();
    
    void setSpeed(float speed);
//    void setPos(int positionX, int positionY);
//    void setXPos(int positionX);
//    void setYPos(int positionY);
//    void setScale(float scale);
//    void setOpacity(float opacity);
//    void setThresh(float thresh);
//    void setInvert(float invert);
    
    int setFrame(int thisFrame, ofPixels thisPixels);

    int shaderActive;
    float thresh;
    float softness;
    float invert;
    void setShaderParams(float thresh, float softness, float invert);
    
    
};

#endif /* vidLayer_hpp */
