//
//  maProjection.h
//  microasterismos
//
//  Created by Berio Molina on 09/06/14.
//
//

#ifndef __microasterismos__maProjection__
#define __microasterismos__maProjection__

#include "ofMain.h"
#include "maBlob.h"
#include "ofxOsc.h"
#include "bigWave.h"
#include "freeze.h"

#define HOST "localhost"
#define SEND_PORT 57120
#define	RECEIVING_PORT 12345

class maProjection{
    
public:
    maProjection();
    
    void setup();
    void reset();
    void update(ofPoint _centro, ofRectangle _arredorRect);
    void draw();
    void drawConstellation();
    void drawWavyLine(ofPoint a, ofPoint b, float radio);
    void borrarDebuxos();
    void borrarFreezes();
    void setSamplesLibrary(string _samples);
    void sendKillAllSoundsOscMsg();
    
private:
    void sendOscMsg(ofxOscMessage _m);
    void receiveOscMsg();
    void sendKillOscMsg(int _lId, int _sId);
    
    void creaMaBlob(ofPoint _centro, ofRectangle _arredorRect);
    ofPoint reubicarPunto(ofPoint _punto);
    ofRectangle reubicarRectangulo(ofRectangle _rect);
    bool blobPanned;
    
    int xProjection;
    list <maBlob>  maBlobs;
    list <bigWave> bigWaves;
    list <freeze> freezeList;
    int contadorMaBlobs;
    string      samplesLibrary;
    
    ofxOscSender sender;
    ofxOscReceiver	receiver;
    float currTime;

};


#endif /* defined(__microasterismos__maProjection__) */
