//
//  lunar.h
//  microasterismos
//
//  Created by Berio Molina on 09/06/14.
//
//

#ifndef __microasterismos__lunar__
#define __microasterismos__lunar__

#include "ofMain.h"
#include "wave.h"

class lunar : public ofPoint {
    
public:
    lunar();
    ~lunar();
    void setup(float _x, float _y, float _r, int _d, string _tipo = "");
    void update(float _x, float _y, float _distanciaMinima, float _rOrixinal);
    void draw();
    
private:
    float r;
    float rOrixinal;
    int duracionOnda;
    string tipoOnda;
    
protected:
    vector <wave>	waveList;
    
};

#endif /* defined(__microasterismos__lunar__) */
