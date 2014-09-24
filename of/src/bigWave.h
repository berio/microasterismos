//
//  bigWave.h
//  microasterismos
//
//  Created by Berio Molina on 29/08/14.
//
//

#ifndef __microasterismos__bigWave__
#define __microasterismos__bigWave__

#include "ofMain.h"

class bigWave : public ofPoint{

public:
    bigWave(float _x, float _y);
    bool getKillMe();
    void update();
    void draw();
    
private:
    bool kill;
    int duration, time, r;
    float alphaValue, expandValue, lineWidthValue;
};

#endif /* defined(__microasterismos__bigWave__) */
