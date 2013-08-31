//
//  timeLine.h
//  drawingOpenCvOsc
//
//  Created by berio molina on 28/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _TIME_BALL_H
#define _TIME_BALL_H

#include "ofMain.h"
#include "wave.h"

class timeBall : public ofPoint {
    
public:
    timeBall();
    ~timeBall();
    void setup(float _x, float _limitTop, float _limitBottom);
    void update(float _x, float _limitTop, float _limitBottom);
    void draw();
    float getYPos();
    
protected:
    float aPointX;
    float bPointX;
    float limitBottom;
    float limitTop;
    vector <wave>	waveList;
    
};

#endif
