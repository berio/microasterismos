//
//  bigWave.cpp
//  microasterismos
//
//  Created by Berio Molina on 29/08/14.
//
//

#include "bigWave.h"

bigWave::bigWave(float _x, float _y){
    x = _x;
    y = _y;
	r = 15;
	duration = 1500;
	time = ofGetElapsedTimeMillis();
	kill = false;
}

void bigWave::update(){
	int countUp = ofGetElapsedTimeMillis() - time;
	if(countUp > duration){
		kill = true;
	}else{
		alphaValue = ofMap(countUp, 0, duration, 200, 0);
		expandValue = ofMap(countUp, 0, duration, r, r+100);
        lineWidthValue = ofMap(countUp, 0, duration, 1, 20);
	}
}

void bigWave::draw(){
	ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofSetLineWidth(lineWidthValue);
	ofSetColor(255, 255, 255, alphaValue);
	ofNoFill();
	ofCircle(x, y, expandValue);
	ofDisableAlphaBlending();
    ofSetLineWidth(1);
}

bool bigWave::getKillMe(){
	return kill;
}
