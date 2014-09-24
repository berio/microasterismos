//
//  freeze.h
//  microasterismos
//
//  Created by Berio Molina on 04/09/14.
//
//

#ifndef __microasterismos__freeze__
#define __microasterismos__freeze__

#include "ofMain.h"
#include "ofxOsc.h"

class freeze : public ofPoint{

public:
    freeze(float _x, float _y);
    ofPoint getCenter();
    void draw();

private:
    void snapSoundMsg();
    int centerDislocation;
    ofxOscMessage snapMsg;
    int soundId, freezeId;
};

#endif /* defined(__microasterismos__freeze__) */
