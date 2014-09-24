//
//  maBlob.h
//  microasterismos
//
//  Created by Berio Molina on 09/06/14.
//
//

#ifndef __microasterismos__maBlob__
#define __microasterismos__maBlob__

#include "ofMain.h"
#include "ofxOsc.h"
#include "lunar.h"

class maBlob{
    
public:
    maBlob();
    ~maBlob();
    void setup(ofPoint _centro, ofRectangle _arredorRect, int _id, string _sl, float _radioOrixinal, string _tipo, int _duration, float _pan = ofRandomf());
    void setBlobOrigen(ofPoint _centro, ofRectangle _arredorRect);
    void setSoundId(int _soundId);
    void update(ofPoint _centro, ofRectangle _arredorRect, float _distanciaMinima, float pan = 0.0);
    void draw();
    
    bool isInsideMe(ofPoint _centroBlob);
    void setUpdated(bool _updated);
    bool getUpdated();
    int getMaBlobId();
    int getSoundId();
    ofPoint getCentro();
    
    ofxOscMessage getOscMsg();
    ofxOscMessage getSnapMsg();
    
private:
    void snapSoundMsg();
    void setWarpedSoundMsg(string _action);
    void setSamplePosition(float _y, float _yTop, float _yBottom);
    void setOscMsg(string _type, string _action="none");
    
    ofPoint centroBlobOrigen;
    ofRectangle arredorRectBlobOrigen;
    
    ofPoint centro;
    ofRectangle arredorRect;
    
    bool updated;
    
    float       r, rOrixinal;
    float       position, pitch, pan;
    int         soundId, maBlobId;
    
    lunar       lunar;
    
    string      samplesLibrary;
    ofxOscMessage oscMsg;
    ofxOscMessage snapMsg;
    
    int         sampleContinuo;
    int         sampleGolpe;
    
};

#endif /* defined(__microasterismos__maBlob__) */
