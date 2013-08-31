#ifndef _LINEA_H
#define _LINEA_H

#include "ofMain.h"
#include "ofxOsc.h"
#include "timeBall.h"
#include "lunar.h"


/*
Esta clase en realidade non representa a unha linha, senon todo o debuxo dun blob,
 que pode ser un lunar ou un elemento de devuison
*/
class linea{

public:
    linea();
    ~linea();
    void setup(ofPoint _centro, ofRectangle _arredorRect, int _id, string _samplesLibrary, float radioOrixinal);
    void setBlobOrigen(ofPoint _centro, ofRectangle _arredorRect);
    void setSoundId(int _soundId);
    void update(ofPoint _centro, ofRectangle _arredorRect, float _distanciaMinima);
    void draw();
    bool checkTimeline(float _tLineYPos);
    
    ofxOscMessage getOscMsg();
    
    bool isInsideMe(ofPoint _centroBlob);
    void setUpdated(bool _updated);
    bool getUpdated();
    int getLineId();
    int getSoundId();
    float getRadio();
    float getLimitTop();
    float getLimitBottom();
    ofPoint getCentro();
    void killLine();
    
    ofPoint centroBlobOrigen;
    ofRectangle arredorRectBlobOrigen;

    ofPoint centro;
    ofRectangle arredorRect;
    
    bool updated;
    
    ofxOscMessage oscMsg;
    int         soundId;
    
    
private:
    void snapSoundMsg();
    void setWarpedSoundMsg(string _action);
    void setSamplePosition(float _y, float _yTop, float _yBottom);
    void setOscMsg(string _type, string _action="none");
    
    int         distanceLineDrawY;
    string      samplesLibrary;
    bool        timeLineSoundPlaying;
    timeBall    tBall;
    lunar       lunar;
    int         lineId;
    float       position, pitch, pan;
    float       r, rOrixinal;
    int         sampleContinuo;
    int         sampleGolpe;

};


#endif
