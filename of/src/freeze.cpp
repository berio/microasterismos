//
//  freeze.cpp
//  microasterismos
//
//  Created by Berio Molina on 04/09/14.
//
//

#include "freeze.h"

freeze::freeze(float _x, float _y){
    x = _x;
    y = _y;
    centerDislocation = 2;
}

ofPoint freeze::getCenter(){
    ofPoint center;
    center.set(x, y);
    
    return center;
}

void freeze::draw(){
    
    ofEnableAlphaBlending();
    ofSetCircleResolution(50);
	ofNoFill();
    
    for(int i=0; i<20; i++){
        float xDislocation = ofRandom(x-centerDislocation, x+centerDislocation);
        float yDislocation = ofRandom(y-centerDislocation, y+centerDislocation);
        float alpha = ofRandom(0, 40);
        ofSetColor(255, 255, 255, alpha);
        ofCircle(xDislocation, yDislocation, 10);
	}
	ofFill();
    ofCircle(x, y, 10);
    
    ofDisableAlphaBlending();
}

void freeze::snapSoundMsg(){
    
    snapMsg.clear();
    snapMsg.setAddress("/freeze");    // type of sound
    snapMsg.addStringArg("true");        // 1. (play / noplay)
    snapMsg.addIntArg(1);      // 2. sample
    snapMsg.addFloatArg(1);              // 3. volume
    snapMsg.addFloatArg(0.5);            // 4. pan
}
