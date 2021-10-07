//
//  oscMapper.cpp
//  livedraw-cam-loop-syphon
//
//  Created by Ali Momeni on 10/6/21.
//

#include "OscMapper.h"

void OscMapper::setup(int port) {
    // open an outgoing connection to HOST:PORT

    sender.setup("127.0.0.1", port);
}

void OscMapper::process(ofxOscMessage msg) {
    string address;
    ofxOscMessage m;
    if(msg.getAddress().compare(0, POS_X.length(), POS_X) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != posy[i] ) {
                posx[i] = msg.getArgAsFloat(i);
                // send it out over OSC to Millumin
                address = LAYER_PREFIX + "/" + ofToString(i) + POSITION_OUT;
                float args[2] = {posx[i],posy[i]};
                send(address, args);
            }
        }
    } else
    if(msg.getAddress().compare(0, POS_Y.length(), POS_Y) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != posy[i] ) {
                posy[i] = msg.getArgAsFloat(i);
                // send it out over OSC to Millumin
                float args[2] = {posx[i],posy[i]};
                send(LAYER_PREFIX + "/" + ofToString(i) + POSITION_OUT,
                     args);
            }
        }
    } else
    if(msg.getAddress().compare(0, OPACITY.length(), OPACITY) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != opacity[i] ) {
                opacity[i] = msg.getArgAsFloat(i);
                float args[1] = {opacity[i]};
                send(LAYER_PREFIX + "/" + ofToString(i) + OPACITY_OUT,
                     args);
            }
        }
    } else
    if(msg.getAddress().compare(0, SCALE.length(), SCALE) == 0) {
        for(int i = 0; i < 8; i++){
            if (msg.getArgAsFloat(i) != scale[i] ) {
                scale[i] = msg.getArgAsFloat(i);
                ofxOscMessage m;
                float args[1] = {scale[i]};
                send(LAYER_PREFIX + "/" + ofToString(i) + SCALE_OUT,
                     args);
            }
        }
    }
    
}

void OscMapper::send(string address, float args[]) {
    ofxOscMessage m;
    m.setAddress(address);
    for (int i=0; i < sizeof(args); i++) {
        m.addFloatArg(args[i]);
    }
    sender.sendMessage(m, false);
}
