#include "timeBall.h"

timeBall::timeBall(){
}

timeBall::~timeBall(){
	if(waveList.size()>0){
		waveList.clear();
	}
}

void timeBall::setup(float _x, float _limitTop, float _limitBottom){
    aPointX = _x;
    limitBottom = _limitBottom;
    y = limitTop = _limitTop;

    waveList.push_back(wave());
	waveList[waveList.size()-1].setup(_x, y);
}

void timeBall::update(float _x, float _limitTop, float _limitBottom){
    aPointX = _x;
    limitBottom = _limitBottom;
    limitTop = _limitTop;

    waveList.push_back(wave());
	waveList[waveList.size()-1].setup(_x, y);

    // Wave Visualization Update
    if(waveList.size()>0){
        for(int i=0; i<waveList.size(); i++){
            //waveList[i].update(aPointX, y);
            if(waveList[i].getKillMe()){
                waveList.erase(waveList.begin()+i);
            }	
        }
    }

    y ++;
    if(y>limitBottom) y=limitTop;
    
}

float timeBall::getYPos(){
    return y;
}

void timeBall::draw(){
    ofSetColor(250, 250, 250);
    //ofLine(aPointX, y, bPointX, y);
    ofCircle(aPointX, y, 5);

	// Wave visuazitation draw
	if(waveList.size()>0){
		for(int i=0; i<waveList.size(); i++){
			waveList[i].draw();
		}
	}		
    
}