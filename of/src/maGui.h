//
//  maGui.h
//  microasterismos
//
//  Created by Berio Molina on 09/06/14.
//
//

#ifndef __microasterismos__maGui__
#define __microasterismos__maGui__

#include "ofMain.h"
#include "ofxGui.h"
#include "maFindArea.h"

class maGui{
    
public:
    
    maGui();
    
    void setup();
    void draw();
    
    float getShutter();
    float getExposure();
    float getBrightness();
    float getGamma();
    float getGain();
    int getBlur();
    int getThreshold();
    int getMinArea();
    int getMaxArea();
    void setCameraAuto(float _shutter, float _exposure, float _brightness, float _gamma, float _gain);
    bool getCameraAuto();
    ofVec2f * getFindAreaPoints();
    ofPoint * getWarpPoints();
    void setWarpPoints(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4);
    
    void mouseDragged(int x, int y);
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y);
    
private:
    
    maFindArea findArea;
    maFindArea warpArea;
    ofxPanel gui;
    ofParameterGroup mainParameters;
    ofParameterGroup cameraParameters;
    ofParameterGroup findAreaParameters;
    ofParameterGroup openCvParameters;
    ofParameterGroup warpParameters;
    
    ofParameter<float> guiShutter;
    ofParameter<float> guiExposure;
    ofParameter<float> guiBrightness;
    ofParameter<float> guiGamma;
    ofParameter<float> guiGain;
    ofParameter<bool> guiAutoCamera;
    ofParameter<string> labelShutter, labelExposure, labelBrightness, labelGamma, labelGain;
    
    ofParameter<ofVec2f> findAreaPointA, findAreaPointB, findAreaPointC, findAreaPointD;
    
    ofParameter<int> guiBlur, guiThreshold, guiMinArea, guiMaxArea;
    
    ofParameter<ofVec2f> warpAreaPointA, warpAreaPointB, warpAreaPointC, warpAreaPointD;
    
    bool isDraggingAreaPoint;
    int findAreaDragedPoint;
};

#endif /* defined(__microasterismos__maGui__) */
