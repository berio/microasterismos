//
//  timeLine.h
//  drawingOpenCvOsc
//
//  Created by berio molina on 28/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _TIME_LINE_H
#define _TIME_LINE_H

#include "ofMain.h"

class timeLine : public ofPoint {

public:
    timeLine();
    void setup(float _ax, float _bx, float _limitTop, float _limitBottom);
    void update();
    void update(float _limitTop, float _limitBottom);
    void update(float _ax, float _bx, float _limitTop, float _limitBottom);
    void draw();
    float getYPos();
    
protected:
    float aPointX;
    float bPointX;
    float limitBottom;
    float limitTop;
    
};

#endif
