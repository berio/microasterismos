//
//  lunar.cpp
//  lunar
//
//  Created by Berio Molina on 8/20/13.
//

#include "lunar.h"

lunar::lunar(){
}

lunar::~lunar(){
	if(waveList.size()>0){
		waveList.clear();
	}
}

void lunar::setup(float _x, float _y, float _r){
	set(_x, _y);
    r = _r;

    waveList.push_back(wave());
	waveList[waveList.size()-1].setup(_x, y);
}

void lunar::update(float _x, float _y, float _distanciaMinima, float _rOrixinal){
	set(_x, _y);
    r = _distanciaMinima;
    rOrixinal = _rOrixinal;

    printf("radio: %f", r);
    
    waveList.push_back(wave());
	waveList[waveList.size()-1].setup(_x, y);

    // Wave Visualization Update
    if(waveList.size()>0){
        for(int i=0; i<waveList.size(); i++){
            waveList[i].update(_x, _y, r+10);
            if(waveList[i].getKillMe()){
                waveList.erase(waveList.begin()+i);
            }
        }
    }
}

void lunar::draw(){

    ofSetColor(250, 250, 250);

    // Wave visuazitation draw
	if(waveList.size()>0){
		for(int i=0; i<waveList.size(); i++){
			waveList[i].draw();
		}
	}

    //ofLine(aPointX, y, bPointX, y);
    ofCircle(x, y, r);
    ofSetColor(0, 0, 0);
    ofCircle(x, y, rOrixinal+15);
}