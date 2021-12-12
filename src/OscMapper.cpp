//
//  oscMapper.cpp
//  livedraw-cam-loop-syphon
//
//  Created by Ali Momeni on 10/6/21.
//

#include "ofApp.h"
#include "OscMapper.h"

void OscMapper::setup(int port, vector<VidLayer> &layers) {
//void OscMapper::setup(int port) {
    // open an outgoing connection to HOST:PORT
    vidLayers = &layers;
    //ofLog() << "Setting up OSC Sender for target 127.0.0.1 port " << ofToString(port);
    sender.setup("127.0.0.1", port);
}

void OscMapper::process(ofxOscMessage msg) {
    string address;
    ofxOscMessage m;
    float posOut[2];
    string argString;
    // Logging incoming messages
    for (int i = 0; i < msg.getNumArgs();i++) {
        argString = argString + " " + ofToString(msg.getArgAsFloat(i));
    }
    ofLog() << "OSC Received: " << msg.getAddress() << " " << argString;
    
    
    if(msg.getAddress().compare(0, POS_X.length(), POS_X) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != posx[i] ) {
                posx[i] = msg.getArgAsFloat(i);
                // send it out over OSC to Millumin
                float args[2] = {mapX(posx[i]),mapY(posy[i])};
                string address = LAYER_PREFIX + ofToString(i) + POSITION_OUT;
                //ofLog() << "pos x: " << ofToString(address) << " " << ofToString(mapX(posx[i])) << " " << ofToString(mapY(posy[i]));
                send(address, args);
            }
        }
    } else
    if(msg.getAddress().compare(0, POS_Y.length(), POS_Y) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != posy[i] ) {
                posy[i] = msg.getArgAsFloat(i);
                // send it out over OSC to Millumin
                float args[2] = {mapX(posx[i]),mapY(posy[i])};
                string address = LAYER_PREFIX + ofToString(i) + POSITION_OUT;
                //ofLog() << "pos y: " << ofToString(address) << " " << ofToString(mapX(posx[i])) << " " << ofToString(mapY(posy[i]));
                send(address, args);
            }
        }
    } else
    if(msg.getAddress().compare(0, OPACITY.length(), OPACITY) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != opacity[i] ) {
                opacity[i] = msg.getArgAsFloat(i);
                float args[1] = {opacity[i]};
                send(LAYER_PREFIX + ofToString(i) + OPACITY_OUT,
                     args);
            }
        }
    } else
    if(msg.getAddress().compare(0, SCALE.length(), SCALE) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != scale[i] ) {
                scale[i] = msg.getArgAsFloat(i);
                ofxOscMessage m;
                float args[1] = {ofMap(scale[i],0.,1.,0.,3.)};
                send(LAYER_PREFIX + ofToString(i) + SCALE_OUT,
                     args);
            }
        }
    } else
    if(msg.getAddress().compare(0, STATE.length(), STATE) == 0) {
        int layerNumber = ofToInt(msg.getAddress().erase(0,7))-1; // -1 cuz vidLayer array starts with 0
        
        int newState = (msg.getArgAsInt(0) * 1 + msg.getArgAsInt(1) * 2 + msg.getArgAsInt(2) * 3 + msg.getArgAsInt(3) * 4) - 1;
        //ofLog() << "layer/state : " << ofToString(layerNumber) << " " << ofToString(newState);
        if (newState != (*vidLayers)[layerNumber].getState() ) {
            //ofLog() << "setting : " << ofToString(layerNumber) << " " << ofToString(newState);
            (*vidLayers)[layerNumber].setState(newState);
            // set state of video layer
        }
    } 
}

float OscMapper::mapX(float x) {
    float xScaled = ofMap(x, 0., 1., -1. * float(MILLUMIN_WIDTH), float(MILLUMIN_WIDTH));
    return xScaled;
}

float OscMapper::mapY(float y) {
    float yScaled = ofMap(y, 0., 1., -1. * float(MILLUMIN_HEIGHT), float(MILLUMIN_HEIGHT));
    return yScaled;
}

void OscMapper::send(string address, float args[]) {
    ofxOscMessage m;
    m.setAddress(address);
    for (int i=0; i < sizeof(args); i++) {
        m.addFloatArg(args[i]);
    }
    // ofLog() << "To Millumin: " << address;
    sender.sendMessage(m, false);
}
