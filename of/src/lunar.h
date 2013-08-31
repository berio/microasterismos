//
//  lunar.h
//
//  Created by berio molina on 20/08/13.
//

#ifndef _LUNAR_H
#define _LUNAR_H

#include "ofMain.h"
#include "wave.h"

class lunar : public ofPoint {
    
public:
    lunar();
    ~lunar();
    void setup(float _x, float _y, float _r);
    void update(float _x, float _y, float _distanciaMinima, float _rOrixinal);
    void draw();
    
private:
    float r;
    float rOrixinal;

protected:
    vector <wave>	waveList;
    
};

#endif
