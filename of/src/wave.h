//
//  wave.h
//  microasterismos
//
//  Created by Berio Molina on 09/06/14.
//
//

#ifndef __microasterismos__wave__
#define __microasterismos__wave__

#include "ofMain.h"

class wave : public ofPoint{
    
public:
	
    wave();
    
    void setup(float _x, float _y, int _duration);
    void update(float _x, float _y, float _r);
    bool getKillMe();
    
    void draw();
    void setTipo(string _tipo);
    
private:
	
    bool kill;
    int r;
    int duration;
    int time;
    float alphaValue;
    float expandValue;
    int cYr, cYg, cYb;
    ofPoint ringPos;
    string tipo;
};

#endif /* defined(__microasterismos__wave__) */
