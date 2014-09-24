//
//  maGui.cpp
//  fireflyMvFindObjects
//
//  Created by Berio Molina on 27/04/14.
//
//

#include "maGui.h"

maGui::maGui(){
    
}

void maGui::setup(){
    cameraParameters.setName("camera");
	cameraParameters.add(guiAutoCamera.set("guiAutoCamera",false));
    cameraParameters.add(guiShutter.set( "guiShutter", 0.3, 0, 1.0 ));
    cameraParameters.add(guiExposure.set( "guiExposure", 0.3, 0, 1.0 ));
    cameraParameters.add(guiBrightness.set( "guiBrightness", 0.3, 0, 1.0 ));
    cameraParameters.add(guiGamma.set( "guiGamma", 0.3, 0, 1.0 ));
    cameraParameters.add(guiGain.set( "guiGain", 0.3, 0, 1.0 ));
    cameraParameters.add(labelShutter.set( "shutter", "" ));
    cameraParameters.add(labelExposure.set( "exposure", "" ));
    cameraParameters.add(labelBrightness.set( "brightness", "" ));
    cameraParameters.add(labelGamma.set( "gamma", "" ));
    cameraParameters.add(labelGain.set( "gain", "" ));
    
    findAreaParameters.setName("find area");
    findAreaParameters.add(findAreaPointA.set("findAreaPointA", ofVec2f(10, 10),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    findAreaParameters.add(findAreaPointB.set("findAreaPointB", ofVec2f(20, 10),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    findAreaParameters.add(findAreaPointC.set("findAreaPointC", ofVec2f(20, 20),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    findAreaParameters.add(findAreaPointD.set("findAreaPointD", ofVec2f(10, 20),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    
    openCvParameters.setName("openCv");
    openCvParameters.add(guiBlur.set("guiBlur", 0, 0, 20));
    openCvParameters.add(guiThreshold.set("guiThreshold", 0, 0, 255));
    openCvParameters.add(guiMinArea.set("guiMinArea", 0, 0, 1000));
    openCvParameters.add(guiMaxArea.set("guiMaxArea", 0, 0, 10000));
    
    warpParameters.setName("warp");
    warpParameters.add(warpAreaPointA.set("warpAreaPointA", ofVec2f(10, 10),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    warpParameters.add(warpAreaPointB.set("warpAreaPointB", ofVec2f(20, 10),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    warpParameters.add(warpAreaPointC.set("warpAreaPointC", ofVec2f(20, 20),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    warpParameters.add(warpAreaPointD.set("warpAreaPointD", ofVec2f(10, 20),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    
    mainParameters.setName("microasterimos");
    mainParameters.add(cameraParameters);
    mainParameters.add(openCvParameters);
    mainParameters.add(findAreaParameters);
    mainParameters.add(warpParameters);
    
    gui.setup(mainParameters);
    gui.loadFromFile("settings.xml");
    
    isDraggingAreaPoint = false;
    
    ofColor findAreaColor;
    findAreaColor.r = 59;
    findAreaColor.g = 224;
    findAreaColor.b = 9;
    findArea.setup(findAreaPointA.get(), findAreaPointB.get(), findAreaPointC.get(), findAreaPointD.get(), findAreaColor, false);
    
    ofColor warpColor;
    warpColor.r = 71;
    warpColor.g = 227;
    warpColor.b = 255;
    warpArea.setup(warpAreaPointA.get(), warpAreaPointB.get(), warpAreaPointC.get(), warpAreaPointD.get(), warpColor, true);
}

float maGui::getShutter(){
    return guiShutter.get();
}

float maGui::getExposure(){
    return guiExposure.get();
}

float maGui::getBrightness(){
    return guiBrightness.get();
}

float maGui::getGamma(){
    return guiGamma.get();
}

float maGui::getGain(){
    return guiGain.get();
}

bool maGui::getCameraAuto(){
    return guiAutoCamera;
}

void maGui::setCameraAuto(float _shutter, float _exposure, float _brightness, float _gamma, float _gain){
    labelShutter.set( "shutter", ofToString(_shutter) );
    labelExposure.set( "exposure", ofToString(_exposure) );
    labelBrightness.set( "brightness", ofToString(_brightness) );
    labelGamma.set( "gamma", ofToString(_gamma) );
    labelGain.set( "gain", ofToString(_gain) );
    
}

ofVec2f * maGui::getFindAreaPoints(){
    return findArea.getFindAreaPoints();
}

ofPoint * maGui::getWarpPoints(){
    ofPoint * warpedPoints;
    ofVec2f * oldPoints;
    oldPoints  = new ofVec2f[4];
    warpedPoints  = new ofPoint[4];
    
    oldPoints = warpArea.getFindAreaPoints();
    for (int i=0; i<4; i++) {
        warpedPoints[i].set(oldPoints[i].x, oldPoints[i].y);
    }
    return warpedPoints;
}

void maGui::setWarpPoints(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4){
    warpArea.setPreset(p1, p2, p3, p4);
    //    warpAreaPointA.set("warpAreaPointA", ofVec2f(warpAreaPoints[0].x, warpAreaPoints[0].y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));

    
}

int maGui::getBlur(){
    return guiBlur.get();
}

int maGui::getThreshold(){
    return guiThreshold.get();
}

int maGui::getMinArea(){
    return guiMinArea.get();
}

int maGui::getMaxArea(){
    return guiMaxArea.get();
}

void maGui::draw(){
    findArea.draw();
    warpArea.draw();
	gui.draw();
}

//--------------------------------------------------------------
void maGui::mouseDragged(int x, int y){
    findArea.mouseDragged(x, y);
    warpArea.mouseDragged(x, y);
}

//--------------------------------------------------------------
void maGui::mousePressed(int x, int y){
    findArea.mousePressed(x, y);
    warpArea.mousePressed(x, y);
}

//--------------------------------------------------------------
void maGui::mouseReleased(int x, int y){
    if(findArea.getIsDraggingAreaPoint()){
        
        ofVec2f * findAreaPoints = findArea.getFindAreaPoints();
        switch (findArea.getFindAreaDragedPoint()) {
            case 0:
                findAreaPointA.set("findAreaPointA", ofVec2f(x, y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                findAreaPointB.set("findAreaPointB", ofVec2f(findAreaPoints[1].x, y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                findAreaPointD.set("findAreaPointD", ofVec2f(x, findAreaPoints[3].y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                break;
                
            case 2:
                findAreaPointC.set("findAreaPointC", ofVec2f(x, y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                findAreaPointB.set("findAreaPointB", ofVec2f(x, findAreaPoints[1].y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                findAreaPointD.set("findAreaPointD", ofVec2f(findAreaPoints[3].x, y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                break;
        }
        findArea.mouseReleased(x, y);
    }
    
    if(warpArea.getIsDraggingAreaPoint()){
        
        warpArea.mouseReleased(x, y);
        
        ofVec2f * warpAreaPoints = warpArea.getFindAreaPoints();
        
        switch (warpArea.getFindAreaDragedPoint()) {
            case 0:
                warpAreaPointA.set("warpAreaPointA", ofVec2f(warpAreaPoints[0].x, warpAreaPoints[0].y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                break;
                
            case 1:
                warpAreaPointB.set("warpAreaPointB", ofVec2f(warpAreaPoints[1].x, warpAreaPoints[1].y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                break;
                
            case 2:
                warpAreaPointC.set("warpAreaPointC", ofVec2f(warpAreaPoints[2].x, warpAreaPoints[2].y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                break;
                
            case 3:
                warpAreaPointD.set("warpAreaPointD", ofVec2f(warpAreaPoints[3].x, warpAreaPoints[3].y), ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
                break;
        }
    }
}