#pragma once

#include "ofMain.h"
//#include "ofxMidi.h"
#include "vidLayer.h"
#include "Utils.h"
#include "ofxXmlSettings.h"
#include "ConsoleListener.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"



//camera
extern int CAM_DEV_ID;
extern int CAM_W;
extern int CAM_H;
extern float VID_SCALE;
extern int THUMB_W;
extern int THUMB_H;
extern int CAM_FPS;

//recording layers
extern int LAYER_COUNT;
extern int FRAME_COUNT;

//render window
extern int WINDOW_W;
extern int WINDOW_H;

//OSC
extern int OSC_PORT;
extern int OSC_NUM_MSG_STRINGS;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
//        void drawMIDI();
        void exit();
    
		void keyPressed(int key);
		void keyReleased(int key);
//		void mouseMoved(int x, int y );
//		void mouseDragged(int x, int y, int button);
//		void mousePressed(int x, int y, int button);
//		void mouseReleased(int x, int y, int button);
//		void mouseEntered(int x, int y);
//		void mouseExited(int x, int y);
//		void windowResized(int w, int h);
//		void dragEvent(ofDragInfo dragInfo);
//		void gotMessage(ofMessage msg);
    
    ofTexture nowTexture;
    ofPixels nowPixels;

    int counter;

    // for GStreamer video capture
    // ofGstVideoUtils gst;
    ofTexture gsttex;
    
    
    // Video layers
    vector < vidLayer > vidLayers;
    ofVideoGrabber videoGrabber;
    
    //XML settings related
    ofxXmlSettings XML;
    
    //OSC
    ofxOscReceiver receiver;
    int recordLayerNum;
    int recordLayerState;
    
    //Syphon
    ofTexture syphonTex;
    
    ofxSyphonServer syphon1;
    ofxSyphonServer syphon2;
    ofxSyphonServer syphon3;
    ofxSyphonServer syphon4;
    ofxSyphonServer syphon5;
    ofxSyphonServer syphon6;
    ofxSyphonServer syphon7;

    
    ofxSyphonClient mClient;

};
