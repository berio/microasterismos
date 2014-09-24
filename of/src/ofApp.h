#pragma once

#include "ofMain.h"
#include "ofxLibdc.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "maFindArea.h"
#include "maWarpArea.h"
#include "maGui.h"
#include "maProjection.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    // ofxLibdc is a namespace, and Camera is the object
    ofxLibdc::Camera camera;
	
    // ofImage is where we store the current frame we grabbed from the Camera
    ofImage curFrame;
	
    // include these just to make sure they compile
    ofxLibdc::Grabber grabber;
    ofxLibdc::PointGrey pointGrey;
    
    // GUI
    ofxFloatSlider shutter, exposure, brightness, gamma, gain;
	ofxToggle autoSettings;
    ofxPanel gui;
	ofxLabel autoExposure, autoShutter, autoBrightness, autoGamma, autoGain;
    bool isCameraSettingsAuto;
    
    maGui mainGui;
    
    // OpenCv
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage warpImage;
    ofxCvContourFinder contourFinder;
    vector<ofVec2f> selectedBlobs;
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    void setupWarpAreas();
    void saveArea(int a);
    void loadPresetArea(int a);
    vector<ofVec2f> trackedBlobs;
    vector<maWarpArea> warpAreas;
    ofPoint * dstWarperPositions;
    
    int w,h;
    bool showWarpImage, trackearBlobs;
    maProjection projection;

    ofxXmlSettings XML;
    int numOfMicroasterismos;

};
