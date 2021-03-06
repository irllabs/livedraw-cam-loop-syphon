#pragma once
//
//  oscMapper.h
//  livedraw-cam-loop-syphon
//
//  Created by Ali Momeni on 10/6/21.
//

#include <stdio.h>
#include "ofMain.h"
#include "VidLayer.h"
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
        const string STATE = "/State";
        const string SCALE = "/Scale/x";
        const string OPACITY = "/Opacity/x";
        const string EFFECT = "/effect";
        const string KEY1_ENABLED = "/effect1/enabled";
        const string KEY1_SENSITIVITY = "/effect1/sensitivity";
        const string KEY1_SMOOTHING = "/effect1/smoothing";
        const string KEY1_REMOVESPILL = "/effect1/removespill";
        const string KEY2_ENABLED = "/effect2/enabled";
        const string KEY2_SENSITIVITY = "/effect2/sensitivity";
        const string KEY2_SMOOTHING = "/effect2/smoothing";
        const string KEY2_REMOVESPILL = "/effect2/removespill";
    
        
        // OSC address to Millumin
        const string LAYER_PREFIX = "/Layer";
        const string POSITION_OUT = "/position";
        const string OPACITY_OUT = "/opacity";
        const string SCALE_OUT = "/scale";
        
        ofxOscSender sender;
        ofxOscMessage outmsg;
        
        vector<VidLayer>* vidLayers;

        void setup(int port, vector<VidLayer> &layers);
        
        // void setup(int port);
        void process(ofxOscMessage msg);
        void send(string address, float args[]);
        float mapX(float x);
        float mapY(float y);

};
