#ifndef _WAVE_H
#define _WAVE_H

#include "ofMain.h"

class wave : public ofPoint{
    
public:
	
    wave();
    
    void setup(float _x, float _y);
    void update(float _x, float _y, float _r);
    bool getKillMe();
    
    void draw();
    
private:
	
    bool kill;
    int r;
    int duration;
    int time;
    float alphaValue;
    float expandValue;
    int cYr, cYg, cYb;
    ofPoint ringPos;	
};

#endif