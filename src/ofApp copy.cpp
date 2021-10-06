#include "ofApp.h"
#include <opencv2/opencv.hpp>

using namespace ofxCv;
using namespace cv;

Mat frame;
ofImage thisImage;


//definied as extern in ofApp.h
float camThresh;
float camSoftness;
float camInvert;
float camOpacity;
int camShaderActive;

int CAM_DEV_ID;
int CAM_W;
int CAM_H;
float VID_SCALE;
int THUMB_W;
int THUMB_H;
int CAM_FPS;

//recording layers
int LAYER_COUNT;
int FRAME_COUNT;
int LAYER_DRAW;

//render window
int WINDOW_W;
int WINDOW_H;

//MIDI Controller Numbers
int MIDI_DRAW;
int MIDI_PORT;
int MIDI_REC_CC;
int MIDI_REC_01;
int MIDI_REC_02;
int MIDI_REC_03;
int MIDI_REC_04;
int MIDI_REC_05;
int MIDI_REC_06;
int MIDI_REC_07;
int MIDI_REC_08;

int MIDI_SCALE;
int MIDI_XPOS;
int MIDI_YPOS;

int MIDI_SHADER_BYPASS;
int MIDI_SHADER_THRESH;
int MIDI_SHADER_SOFT;
int MIDI_SHADER_INVERT_ON;
int MIDI_SHADER_INVERT_OFF;
int MIDI_SHADER_OPACITY;
int MIDI_SHADER_CAM_OPACITY;
int MIDI_SHADER_CAM_THRESH;
int MIDI_SHADER_CAM_SOFT;
int MIDI_SHADER_CAM_INVERT_ON;
int MIDI_SHADER_CAM_INVERT_OFF;

//default layer settings
float LAYER_SCALE;
float SHADER_THRESH;
float SHADER_INVERT;
float SHADER_SOFT;
float SHADER_OPACITY;
int SHADER_ACTIVE;
string SHADER_MASK;

//debugging
int USE_OFXCVPI = 1;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_NOTICE);
    //-------------------------------------
    //--XML-settings-----------------------
    //-------------------------------------
    //read all initial values into extern variables from XML settings file
    ofLog(OF_LOG_NOTICE,"--------Reading XML file for initialization--------");
    XML.loadFile("mySettings.xml");
 
    //render window
    WINDOW_W = XML.getValue("WINDOW_W", 0);
    WINDOW_H = XML.getValue("WINDOW_H", 0);
    

    CAM_DEV_ID = XML.getValue("CAM_DEV_ID", 0);
    CAM_W = XML.getValue("CAM_W", 0);
    CAM_H = XML.getValue("CAM_H", 0);
    CAM_FPS = XML.getValue("CAM_FPS", 30);
    VID_SCALE = float(WINDOW_W) / float(CAM_W);
    ofLog(OF_LOG_NOTICE, "VID_SCALE is: " + ofToString(WINDOW_W) + " divided by " + ofToString(CAM_W) + " so: " + ofToString(VID_SCALE));

    THUMB_W = XML.getValue("THUMB_W", 0);
    THUMB_H = XML.getValue("THUMB_H", 0);
    
    
    //recording layers
    LAYER_COUNT = XML.getValue("LAYER_COUNT", 0);
    FRAME_COUNT = XML.getValue("FRAME_COUNT", 0);
    LAYER_DRAW = XML.getValue("LAYER_DRAW", 1);

    
    //MIDI Controller Numbers= 
    MIDI_DRAW = XML.getValue("MIDI_DRAW", 0);
    MIDI_PORT = XML.getValue("MIDI_PORT", 0);
    MIDI_SCALE = XML.getValue("MIDI_SCALE", 0);
    MIDI_XPOS = XML.getValue("MIDI_XPOS", 0);
    MIDI_YPOS = XML.getValue("MIDI_YPOS", 0);
    MIDI_REC_CC = XML.getValue("MIDI_REC_CC", 64);


    //MIDI Note numbers

    MIDI_REC_01 = XML.getValue("MIDI_REC_01", 0);
    MIDI_REC_02 = XML.getValue("MIDI_REC_02", 0);
    MIDI_REC_03 = XML.getValue("MIDI_REC_03", 0);
    MIDI_REC_04 = XML.getValue("MIDI_REC_04", 0);
    MIDI_REC_05 = XML.getValue("MIDI_REC_05", 0);
    MIDI_REC_06 = XML.getValue("MIDI_REC_06", 0);
    MIDI_REC_07 = XML.getValue("MIDI_REC_07", 0);
    MIDI_REC_08 = XML.getValue("MIDI_REC_08", 0);
    
    MIDI_SHADER_BYPASS = XML.getValue("MIDI_SHADER_BYPASS", 0);
    MIDI_SHADER_THRESH = XML.getValue("MIDI_SHADER_THRESH", 0);
    MIDI_SHADER_SOFT = XML.getValue("MIDI_SHADER_SOFT", 0);
    MIDI_SHADER_INVERT_ON = XML.getValue("MIDI_SHADER_INVERT_ON", 0);
    MIDI_SHADER_INVERT_OFF = XML.getValue("MIDI_SHADER_INVERT_OFF", 0);
    MIDI_SHADER_OPACITY = XML.getValue("MIDI_SHADER_OPACITY", 0);
    MIDI_SHADER_CAM_OPACITY = XML.getValue("MIDI_SHADER_CAM_OPACITY", 0);
    MIDI_SHADER_CAM_THRESH = XML.getValue("MIDI_SHADER_CAM_THRESH", 0);
    MIDI_SHADER_CAM_SOFT = XML.getValue("MIDI_SHADER_CAM_SOFT", 0);
    MIDI_SHADER_CAM_INVERT_ON = XML.getValue("MIDI_SHADER_CAM_INVERT_ON", 0);
    MIDI_SHADER_CAM_INVERT_OFF = XML.getValue("MIDI_SHADER_CAM_INVERT_OFF", 0);
    
    //default layer settings
    LAYER_SCALE = XML.getValue("LAYER_SCALE", 0.0);
    SHADER_THRESH = XML.getValue("SHADER_THRESH", 0.0);
    SHADER_INVERT = XML.getValue("SHADER_INVERT", 0.0);

    SHADER_SOFT = XML.getValue("SHADER_SOFT", 0.0);
    SHADER_OPACITY = XML.getValue("SHADER_OPACITY", 0.0);

    SHADER_ACTIVE = XML.getValue("SHADER_ACTIVE", 0);

    //how do you retrieve a string from xml?
    //SHADER_MASK = XML.getValue("SHADER_MASK");

    // //ofSetLogLevel(OF_LOG_VERBOSE);

    //-------------------------------------
    //--SETUP Shader-----------------------
    //-------------------------------------

    ofSetFrameRate(CAM_FPS);
    ofLog(OF_LOG_NOTICE,"--------setting up shaders--------");
    // setup Shader

    #ifdef TARGET_OPENGLES
        camShader.load("shadersES2/livedraw");
    #else
        if(ofIsGLProgrammableRenderer()){
            camShader.load("shadersGL3/livedraw");
        }else{
            camShader.load("shadersGL2/livedraw");
        }
    #endif
    
    camShaderMask.load("img_mask_1080c.png");
    camShaderMask.resize(CAM_W,CAM_H);
    camShaderMaskTex = camShaderMask.getTexture();
    camThresh = SHADER_THRESH;
    camSoftness = SHADER_SOFT;
    camInvert = SHADER_INVERT;
    camOpacity = SHADER_OPACITY;
    camShaderActive = SHADER_ACTIVE;
    
    //shader loading is done in the vidLayer class's setup method
    
    
    //---------------------------------------
    //--SETUP CAMERA-------------------------
    //---------------------------------------

    ofLog(OF_LOG_NOTICE,"--------setting up camera--------");
    
    gst.setPipeline("v4l2src device=/dev/video0 ! image/jpeg,framerate=30/1,width=1920,height=1080 ! jpegparse ! jpegdec ! video/x-raw ! videoconvert ! video/x-raw,width=1920,height=1080,format=BGRx,framerate=30/1 ! videoconvert  ! videorate" , OF_PIXELS_RGB, true, CAM_W,CAM_H);
    gst.startPipeline();
    gst.play();
    gsttex.allocate(CAM_W,CAM_H,GL_RGB);

    // vector<ofVideoDevice> devices = videoGrabber.listDevices();
    
    // for(unsigned int i = 0; i < devices.size(); i++){
    //     if(devices[i].bAvailable){
    //         ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
    //     }else{
    //         ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
    //     }
    // }
    
    // videoGrabber.setDeviceID(CAM_DEV_ID);
    // videoGrabber.setDesiredFrameRate(CAM_FPS);
    // videoGrabber.initGrabber(CAM_W, CAM_H);

    // ofSetVerticalSync(true);
    
    //-------------------------------------
    //--SETUP MIDI-------------------------
    //-------------------------------------
    // print input ports to console
    ofLog(OF_LOG_NOTICE,"--------setting up MIDI--------");

    midiIn.listInPorts(); // via instance
    //ofxMidiIn::listPorts(); // via static as well
    
    // open port by number (you may need to change this)
    midiIn.openPort(MIDI_PORT);
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);
    
    //-------------------------------------------
    //--SETUP VID Layers-------------------------
    //-------------------------------------------
    ofLog(OF_LOG_NOTICE,"--------setting up VidLayers--------");

    //initialize video layers
    ofLog(OF_LOG_NOTICE, "Initializing layers vector to size "+ ofToString(LAYER_COUNT));
    vidLayers.resize(LAYER_COUNT);
    
    for (int k=0;k<LAYER_COUNT;k++){
        ofLog(OF_LOG_NOTICE, "Initializing layer " + ofToString(k)+ " with frame-count "+ ofToString(FRAME_COUNT));
        vidLayers[k].setup(k,FRAME_COUNT);
    }
    
    //------------------------------------------
    ofLog(OF_LOG_NOTICE,"--------DONE with setup--------");
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0, 0, 0);
    // gate latest camera frame, assign it to a texture
    // videoGrabber.update();
    gst.update();

    // if(videoGrabber.isFrameNew()){
    //     nowTexture = videoGrabber.getTexture();
    // }
    if(gst.isFrameNew()){
           nowTexture.loadData(gst.getPixels(), GL_RGB);
    }

    // update video layers
    for (int k=0;k<LAYER_COUNT;k++){
        //ofLog(OF_LOG_NOTICE,"...update layer "+ ofToString(k));
        //first method: use textures
        //nowPixels = videoGrabber.getPixels();
        //vidLayers[k].update(nowPixels);
    
        //second method: use FBOs
        vidLayers[k].update2(nowTexture);
        }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    
    //draw processed cam
    if (SHADER_ACTIVE == 1) {
        //ofLog(OF_LOG_NOTICE, "---------drawing up with Shader");

        camShader.begin();
        camShader.setUniformTexture("maskTex", camShaderMask.getTexture(), 1);
        camShader.setUniform1f("thresh", camThresh);
        camShader.setUniform1f("softness", camSoftness);
        camShader.setUniform1f("invert", camInvert);
        camShader.setUniform1f("opacity", camOpacity);
        //videoGrabber.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);
        nowTexture.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);

        camShader.end();
    } else {
        //ofLog(OF_LOG_NOTICE, "---------drawing up with NO Shader");
        //videoGrabber.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);
        nowTexture.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);
    }

    
    //itereate through all recording buffers
    
    if (LAYER_DRAW == 1) {
        for (int k=0;k<LAYER_COUNT;k++){
            //ofLog(OF_LOG_NOTICE,"...drawing layer "+ ofToString(k));
            vidLayers[k].draw(nowTexture);
        }
    }
    
    if (MIDI_DRAW == 1) { drawMIDI();}

    
}

//--------------------------------------------------------------
void ofApp::drawMIDI() {
    // midi input test
    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 400);
    text.str(""); // clear
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 410);
    text.str(""); // clear
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 440);
    text.str(""); // clear
    //ofDrawRectangle(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 460);
    text.str(""); // clear


    text << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, 480);
    text.str(""); // clear

    text << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, 500);
    text.str(""); // clear


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '1'){
        vidLayers[0].setState(1);
    }
    if(key == '2'){
        vidLayers[1].setState(1);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == '1'){
        vidLayers[0].setState(2);
    }
    if(key == '2'){
        vidLayers[1].setState(2);
    }
}



//--------------------------------------------------------------
void ofApp::exit() {
    
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
}

//--------------------------------------------------------------

/*
float ofApp::getCamThresh(){
    return(camThresh);
}

float ofApp::getCamSoftness(){
    return(camSoftness);
}

float ofApp::getCamInvert(){
    return (camInvert);
}
*/


//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // make a copy of the latest message
    midiMessage = msg;
    int layNum;
    //update midi messages
    
    //record on/off messages
    if (midiMessage.status == MIDI_CONTROL_CHANGE) {

        //record on and off messages
        if (midiMessage.control >=MIDI_REC_CC && midiMessage.control <=(MIDI_REC_CC+LAYER_COUNT) ) {
            layNum = midiMessage.control - MIDI_REC_CC;
            
            if (midiMessage.value==127) {vidLayers[layNum].setState(1);} else {vidLayers[layNum].setState(2);}
        }

        //opacity messages
        if (midiMessage.control >=MIDI_SHADER_OPACITY && midiMessage.control <=(MIDI_SHADER_OPACITY+LAYER_COUNT) ) {
            layNum = midiMessage.control - MIDI_SHADER_OPACITY;
            float scaleVal = 1.;
            scaleVal = Utils::scale(midiMessage.value,0,127,0,1);
            //ofLog(OF_LOG_NOTICE, "Setting scale to " + ofToString(scaleVal));
            
            vidLayers[layNum].setOpacity(scaleVal);
        }

        //thresh messages
        if (midiMessage.control >=MIDI_SHADER_THRESH && midiMessage.control <=(MIDI_SHADER_THRESH+LAYER_COUNT) ) {
            layNum = midiMessage.control - MIDI_SHADER_THRESH;
            float scaleVal = 1.;
            scaleVal = Utils::scale(midiMessage.value,0,127,0,1);
            //ofLog(OF_LOG_NOTICE, "Setting scale to " + ofToString(scaleVal));
            
            vidLayers[layNum].setThresh(scaleVal);
        }

           //scale messages
        if (midiMessage.control >=MIDI_SCALE && midiMessage.control <=(MIDI_SCALE+LAYER_COUNT) ) {
            layNum = midiMessage.control - MIDI_SCALE;
            float scaleVal = 1.;
            if (midiMessage.value < 64) {  scaleVal = Utils::scale(midiMessage.value,0,64,0,1);} else { scaleVal = Utils::scale(midiMessage.value,64,127,1,5);}
            //ofLog(OF_LOG_NOTICE, "Setting scale to " + ofToString(scaleVal));
            
            vidLayers[layNum].setScale(scaleVal);
        }
        

        
        // //X position messages
        // if (midiMessage.control >=MIDI_XPOS && midiMessage.control <=(MIDI_XPOS+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_XPOS ;
        //     float xPosition =Utils::scale(midiMessage.value,0,127,0,WINDOW_W);
        //     vidLayers[layNum].setXPos(xPosition);
        // }
        
        // //Y position messages
        // if (midiMessage.control >=MIDI_YPOS && midiMessage.control <=(MIDI_YPOS+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_YPOS;
        //     float yPosition = Utils::scale(midiMessage.value,0,127,WINDOW_H,0);
            
        //     vidLayers[layNum].setYPos(yPosition);
        // }
        
        // //Shader messages
        
        
        // if (midiMessage.control ==MIDI_SHADER_BYPASS ) {
        //     camShaderActive = Utils::scale(midiMessage.value,0,127,0., 1.);
        // }
        
        if (midiMessage.control ==MIDI_SHADER_CAM_OPACITY ) {
            camOpacity = Utils::scale(midiMessage.value,0,127,0., 1.);
        }
        
        if (midiMessage.control ==MIDI_SHADER_CAM_THRESH ) {
            camThresh = Utils::scale(midiMessage.value,0,127,0., 1.);
        }
        
        // if (midiMessage.control ==MIDI_SHADER_CAM_SOFT ) {
        //     camSoftness = Utils::scale(midiMessage.value,0,127,0., 1.);
        // }
        
        // if (midiMessage.control ==MIDI_SHADER_CAM_INVERT_ON ) {
        //     if (midiMessage.value == 127) {camInvert = 1.;}
        // }
        
        // if (midiMessage.control ==MIDI_SHADER_CAM_INVERT_OFF ) {
        //     if (midiMessage.value == 127) {camInvert = 0.;}
        // }

        // //invert messages

        // if (midiMessage.control >=MIDI_SHADER_INVERT_ON && midiMessage.control <=(MIDI_SHADER_INVERT_ON+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_SHADER_INVERT_ON;
        //     if (midiMessage.value == 127) {vidLayers[layNum].setInvert(1.);}
        // }
        
        // if (midiMessage.control >=MIDI_SHADER_INVERT_OFF && midiMessage.control <=(MIDI_SHADER_INVERT_OFF+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_SHADER_INVERT_ON;
        //     if (midiMessage.value == 127) {vidLayers[layNum].setInvert(0.);}
        // }
    
        
    } else if (midiMessage.status == MIDI_NOTE_ON || midiMessage.status == MIDI_NOTE_OFF) //
    {
        if (midiMessage.pitch == MIDI_REC_01) {layNum = 0; } else
        if (midiMessage.pitch == MIDI_REC_02) {layNum = 1; } else
        if (midiMessage.pitch == MIDI_REC_03) {layNum = 2; } else
        if (midiMessage.pitch == MIDI_REC_04) {layNum = 3; } else
        if (midiMessage.pitch == MIDI_REC_05) {layNum = 4; } else
        if (midiMessage.pitch == MIDI_REC_06) {layNum = 5; } else
        if (midiMessage.pitch == MIDI_REC_07) {layNum = 6; } else
        if (midiMessage.pitch == MIDI_REC_08) {layNum = 7; } 
        
        if (midiMessage.velocity==127) {vidLayers[layNum].setState(1);} else {vidLayers[layNum].setState(2);}
    }

    

 
    
    
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}