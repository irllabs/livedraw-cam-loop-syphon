//
//  vidLayer.cpp
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/16/17.
//
//

#include "ofApp.h"
#include "VidLayer.h"

// #include "Consts.h"


void VidLayer::setup(int thisID, int bufSize){
    //array of textures
    //vidFrames.resize(bufSize);
    
    //array of FBOs
    vidFrames.resize(bufSize);
    
    myID = thisID;
    state = 0;                //stop all playheads; 0 = stop, 1 = record, 2 = play
    playHead = 0;             //set all playheads to 0
    playDir = 1;              // 1 for forward, -1 for reverse
    recHead = 0;              //set all playheads to 0
    recCount = 0;             //set all record framecounts to 0
    recMax = bufSize;
    syphon.setName(ofToString(myID));
    
    // x = THUMB_W * scale * myID +THUMB_W * scale/2;
    //y = THUMB_H * scale + THUMB_H/2;

    x = THUMB_W * (myID+1);
    y = 0;
    //x = WINDOW_W/2;
    //y = WINDOW_H/2;
    
    //does this allocate the FBO?
    for (int k = 0;k<bufSize;k++){
        vidFrames[k].allocate(CAM_W, CAM_H);
        vidFrames[k].setAnchorPercent(0., 0.);
    }
}

void VidLayer::draw(ofTexture thisTexture){
    if (state==2 && recCount>0) {
        //while playing the recording:

        //ofLog(OF_LOG_NOTICE, "playing at " + ofToString(myID)+ " at "+ ofToString(playHead));
        vidFrames[playHead].draw(x, y , THUMB_W, float(CAM_H) / float(CAM_W) * float(THUMB_W));
        syphon.publishTexture(&vidFrames[playHead].getTexture());
    
    } else if (state == 1 || state == 3) {
        //while recording or "thru", draw live feed
        ofTexture livefeed;
        livefeed = thisTexture;
        livefeed.draw(x, y , THUMB_W, float(CAM_H) / float(CAM_W) * float(THUMB_W));
        syphon.publishTexture(&livefeed);
    }
}

void VidLayer::update(ofTexture theTexture){
    if (state == 0) {
        // stop
    } else if (state == 1) {
        //ofLog(OF_LOG_NOTICE, "recording update2 " + ofToString(myID)+ " at "+ ofToString(recHead));
        
        //draw texture onto FBO to record current frame
        vidFrames[recHead].begin();
        theTexture.draw(0,0);
        vidFrames[recHead].end();
        
        //advance record head
        recHead = (recHead + 1 ) % recMax;
    } else if (state == 2) {
        //advance playhead
        playHead = (playHead + playDir * 1);
        if (playHead >= recCount ) {playDir = -1; playHead = recCount - 1;}
        if (playHead <= 0 ) {playDir = 1; playHead = 0;}
    } else if (state == 3) {
        //do nothing
    }
    
}

void VidLayer::setState(int thisState){
    //if we're not already recording, start recording
    
    switch (thisState) {
        //stop
        case 0:
            if (state != thisState) {
                //ofLog(OF_LOG_NOTICE, "STOP on layer " + ofToString(myID));

                state=0;            //set state to 1
            }
        //record
        case 1:
            if (state != thisState) {
                //ofLog(OF_LOG_NOTICE, "RECORD on layer " + ofToString(myID));
                state=1;            //set state to 1
                recHead = 0;        //reset record head
            }
        //play
        case 2:
            if (state != thisState) {
                //ofLog(OF_LOG_NOTICE, "PLAY on layer " + ofToString(myID));
                state=2;
                recCount = recHead;     //set record frame count to last frame recorded
                playHead = recCount - 1;           //set playhead to 0
                playDir = -1;
            }
        //thru
        case 3:
            //ofLog(OF_LOG_NOTICE, "THRU on layer " + ofToString(myID));
            if (state != thisState) {
                state = 3;
            }
    }
    
}

int VidLayer::getState() {
    return(state);
}

void VidLayer::setSpeed(float speed){
    
}
