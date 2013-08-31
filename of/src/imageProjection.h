#ifndef _IMAGE_PROJECTION_H
#define _IMAGE_PROJECTION_H

#include "ofxOpenCv.h"
#include "ofMain.h"
#include "linea.h"
#include "timeLine.h"

#define HOST "localhost"
#define SEND_PORT 57120
#define	RECEIVING_PORT 12345

class imageProjection : public ofBaseApp{

public:
    
    imageProjection();
    
    void update(ofPoint _centro, ofRectangle _arredorRect);
    void updateLines();
    
    void setup();
    void draw();
    void drawWavyLine(ofPoint a, ofPoint b, float radio);
    void reset();
    
    void keyPressed(int key);

    vector <int>  lineasConBlob;
    vector <linea>  lineas;
    vector <int> probas;
    int xProxeccion;

    ofxOscSender sender;
    ofxOscReceiver	receiver;
    
    void sendOscMsg(ofxOscMessage _m);
    void sendCreationOscMsg(int _lId);
    void receiveOscMsg();

private:
    void sendKillOscMsg(int _lId, int _sId);
    void creaDebuxo(ofPoint _blobPunto, ofRectangle _blobRect);
    void borrarDebuxos();
    ofPoint reubicarPunto(ofPoint _punto);
    ofRectangle reubicarRectangulo(ofRectangle _rect);
    
    string      samplesLibrary;
    int         samplesFrases[10];
    int         contadorLineIds;
    int         nLineas;
    timeLine    tLine;
    float currTime;
};

#endif
