#include "ofApp.h"

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

//OSC
int OSC_PORT;
int OSC_NUM_MSG_STRINGS;
int MILLUMIN_PORT;
int MILLUMIN_WIDTH;
int MILLUMIN_HEIGHT;


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
    
    //OSC
    OSC_PORT = XML.getValue("OSC_PORT",12345);
    OSC_NUM_MSG_STRINGS = XML.getValue("OSC_NUM_MSG_STRINGS",20);
    MILLUMIN_PORT = XML.getValue("MILLUMIN_PORT", 5000);
    MILLUMIN_WIDTH = XML.getValue("MILLUMIN_WIDTH", 1920);
    MILLUMIN_HEIGHT = XML.getValue("MILLUMIN_HEIGHT", 1080);
    //---------------------------------------
    //--SETUP CAMERA-------------------------
    //---------------------------------------

    ofLog(OF_LOG_NOTICE,"--------setting up camera--------");
    ofSetFrameRate(CAM_FPS);

    vector<ofVideoDevice> devices = videoGrabber.listDevices();
    
    for(unsigned int i = 0; i < devices.size(); i++){
         if(devices[i].bAvailable){
             ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
         }else{
             ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
         }
    }
    
     videoGrabber.setDeviceID(CAM_DEV_ID);
     videoGrabber.setDesiredFrameRate(CAM_FPS);
     videoGrabber.initGrabber(CAM_W, CAM_H);

     ofSetVerticalSync(true);
    
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
    
    //-------------------------------------------
    //--SETUP OSC--------------------------------
    //-------------------------------------------
    ofLog(OF_LOG_NOTICE,"--------setting up OSC--------");
    // listen on the given port
    ofLog() << "listening for osc messages on port " << OSC_PORT;
    receiver.setup(OSC_PORT);
    oscMapper.setup(MILLUMIN_PORT, vidLayers);
    //oscMapper.setup(MILLUMIN_PORT);

    
    //-------------------------------------------
    //--SETUP Syphon--------------------------------
    //-------------------------------------------
    
    syphonlive.setName("0");
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0, 0, 0);
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        // ofLog() << "something!" << ofToString(m.getAddress());
        oscMapper.process(m);
        
//        // check for mouse moved message
//        if(m.getAddress().compare(0, 10, "/record/1/") == 0) {
//            // ofLog() << m.getAddress().erase(0,10);
//            recordLayerNum = ofToInt(m.getAddress().erase(0,10)) - 1;
//            // convert 0 / 1 to 2 / 1
//            recordLayerState = !m.getArgAsInt(0) + 1;
//            vidLayers[recordLayerNum].setState(recordLayerState);
//        } else  if(m.getAddress().compare(0, 10, "/Position/x") == 0){
//            ofLog() << "Position: " << m.getAddress();
//        }

    }
    
    // gate latest camera frame, assign it to a texture
    videoGrabber.update();

    
    // If new video frame is available, put it into a texture
     if(videoGrabber.isFrameNew()){
         nowTexture = videoGrabber.getTexture();
     }
    
    // update video layers' textures
    for (int k=0;k<LAYER_COUNT;k++){
        vidLayers[k].update(nowTexture);
        }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);

    //nowTexture.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);
    nowTexture.draw(0,0,THUMB_W, float(CAM_H) / float(CAM_W) * float(THUMB_W));
    
    //publish texture to syphon
    syphonlive.publishTexture(&nowTexture);
    
    //itereate through all recording buffers
    
    if (LAYER_DRAW == 1) {
        for (int k=0;k<LAYER_COUNT;k++){
            //ofLog(OF_LOG_NOTICE,"...drawing layer "+ ofToString(k));
            vidLayers[k].draw(nowTexture);
        }
    }
    

    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '1'){
        vidLayers[0].setState(1);
    }
    if(key == '2'){
        vidLayers[1].setState(1);
    }
    if(key == '3'){
        vidLayers[2].setState(1);
    }
    if(key == '4'){
        vidLayers[3].setState(1);
    }
    if(key == '5'){
        vidLayers[4].setState(1);
    }
    if(key == '6'){
        vidLayers[5].setState(1);
    }
    if(key == '7'){
        vidLayers[6].setState(1);
    }
    if(key == '8'){
        vidLayers[7].setState(1);
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
    if(key == '3'){
        vidLayers[2].setState(2);
    }
    if(key == '4'){
        vidLayers[3].setState(2);
    }
    if(key == '5'){
        vidLayers[4].setState(2);
    }
    if(key == '6'){
        vidLayers[5].setState(2);
    }
    if(key == '7'){
        vidLayers[6].setState(2);
    }
    if(key == '8'){
        vidLayers[7].setState(2);
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // clean up
//    midiIn.closePort();
//    midiIn.removeListener(this);
}

//--------------------------------------------------------------



