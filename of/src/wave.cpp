//
//  wave.cpp
//  microasterimos
//
//  Created by Berio Molina on 11/05/14.
//
//

#include "wave.h"

wave::wave(){
	kill = false;
}

void wave::setup(float _x, float _y, int _duration){
	set(_x, _y);
	r = 5;
	duration = _duration;
	time = ofGetElapsedTimeMillis();
}

void wave::update(float _x, float _y, float _r){
	ringPos.set(_x, _y);
    r = _r;
	int countUp = ofGetElapsedTimeMillis() - time;
	if(countUp > duration){
		kill = true;
	}else{
        ofLog(OF_LOG_NOTICE, "tipo de wave: " + tipo);
        if(tipo == "cometa"){
            alphaValue = ofMap(countUp, 0, duration, 100, 0);
            expandValue = ofMap(countUp, 0, duration, r, r+50);
        }else{
            alphaValue = ofMap(countUp, 0, duration, 200, 0);
            expandValue = ofMap(countUp, 0, duration, r, r+10);
        }
	}
}

void wave::setTipo(string _tipo){
    tipo = _tipo;
}

bool wave::getKillMe(){
	return kill;
}

void wave::draw(){
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, alphaValue);
	ofNoFill();
	ofCircle(x, y, expandValue);
	ofSetColor(255, 255, 255, alphaValue);
	ofFill();
	ofCircle(ringPos.x, ringPos.y, r+1);
	ofDisableAlphaBlending();
}