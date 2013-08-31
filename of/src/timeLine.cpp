#include "timeLine.h"

timeLine::timeLine(){
}

void timeLine::setup(float _ax, float _bx, float _limitTop, float _limitBottom){
    aPointX = _ax;
    bPointX = _bx;
    limitBottom = _limitBottom;
    y = limitTop = _limitTop;
}

void timeLine::update(){
    y ++;
    if(y>limitBottom) y=limitTop;
}

void timeLine::update(float _limitTop, float _limitBottom){
    limitBottom = _limitBottom;
    limitTop = _limitTop;
    y ++;
    if(y>limitBottom) y=limitTop;
}

void timeLine::update(float _ax, float _bx, float _limitTop, float _limitBottom){
    aPointX = _ax;
    bPointX = _bx;
    limitBottom = _limitBottom;
    limitTop = _limitTop;
    y ++;
    if(y>limitBottom) y=limitTop;
}

float timeLine::getYPos(){
    return y;
}

void timeLine::draw(){
    ofSetColor(250, 250, 250);
    ofLine(aPointX, y, bPointX, y);
}