//
//  oscMapper.h
//  livedraw-cam-loop-syphon
//
//  Created by Ali Momeni on 10/6/21.
//

#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"


class OscMapper{
public:
    float posx[8];
    float posy[8];
    float scale[8];
    float opacity[8];
    
    // OSC addresses from controller
    const string POS_X = "/Position/x";
    const string POS_Y = "/Position/y";
    const string SCALE = "/State";
    const string OPACITY = "/Opacity";
    
    // OSC address to Millumin
    const string LAYER_PREFIX = "/Layer";
    const string POSITION_OUT = "/position";
    const string OPACITY_OUT = "/opacity";
    const string SCALE_OUT = "/scale";
    
    ofxOscSender sender;
    ofxOscMessage outmsg;
    
    void setup(int port);
    void process(ofxOscMessage msg);
    void send(string address, float args[]);
};
