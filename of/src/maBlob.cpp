//
//  maBlob.cpp
//  fireflyMvFindObjects
//
//  Created by Berio Molina on 07/05/14.
//
//

#include "maBlob.h"

maBlob::maBlob(){
}

maBlob::~maBlob(){
}

void maBlob::setup(ofPoint _centro, ofRectangle _arredorRect, int _id, string _sl, float _radioOrixinal, string _tipo, int _duration, float _pan){
    centro = _centro;
    arredorRect = _arredorRect;
    maBlobId = _id;
    samplesLibrary = _sl;
    pan = _pan;
    pitch = 1;
    r = _radioOrixinal;
    rOrixinal = _radioOrixinal;
    
    lunar.setup(centro.x, centro.y, r, _duration, _tipo);
    
    // sound
    // ofSeedRandom();
    float sc = ofRandom(0,9);
    ofLog(OF_LOG_NOTICE, "Sample num: " + ofToString(sc));
    sampleContinuo = int(sc + 0.5);
    ofLog(OF_LOG_NOTICE, "Sample num: " + ofToString(sampleContinuo));
    
    float sg = ofRandom(0,9);
    sampleGolpe = int(sg + 0.5);
    
    setWarpedSoundMsg("play");
}

void maBlob::update(ofPoint _centro, ofRectangle _arredorRect, float _distanciaMinima, float _pan){
    centro = _centro;
    arredorRect = _arredorRect;
    r = rOrixinal;
    
    float yTop = arredorRect.y;
    float yBottom = arredorRect.y + arredorRect.height;
    
    
    if(_pan != 0.0){
        pan = _pan;
    }
    
    //    ofLog(OF_LOG_NOTICE, "blob real pan " + ofToString(pan));
    //MA    tBall.update(centro.x - (arredorRect.width/2)-distanceLineDrawY, yTop, yBottom);
    //    tBall.update(centro.x, yTop, yBottom);
    
    if(_distanciaMinima<350.0){
        r += ofMap(_distanciaMinima, 0.0, 350.0, 30.0, 5.0);
        pitch = ofMap(_distanciaMinima, 0.0, 350.0, 0.5, 2.0);
    }else{
        pitch = 2.0;
    }
    
    lunar.update(centro.x, centro.y, r, rOrixinal);
    
    //MA   setSamplePosition(tBall.getYPos(), yTop, yBottom);
    
    setWarpedSoundMsg("update");
}

bool maBlob::isInsideMe(ofPoint _centroBlob){
    if(_centroBlob.x > arredorRectBlobOrigen.x &&
       _centroBlob.x < arredorRectBlobOrigen.x + arredorRectBlobOrigen.width &&
       _centroBlob.y > arredorRectBlobOrigen.y &&
       _centroBlob.y < arredorRectBlobOrigen.y + arredorRectBlobOrigen.height
       ){
        return true;
    }else{
        return false;
    }
    
}

void maBlob::setBlobOrigen(ofPoint _centro, ofRectangle _arredorRect){
    centroBlobOrigen = _centro;
    arredorRectBlobOrigen = _arredorRect;
}

void maBlob::setUpdated(bool _updated){
    updated = _updated;
}

void maBlob::setSoundId(int _soundId){
    soundId = _soundId;
}

bool maBlob::getUpdated(){
    return updated;
}

ofPoint maBlob::getCentro(){
    return centro;
}

int maBlob::getMaBlobId(){
    return maBlobId;
}

int maBlob::getSoundId(){
    return soundId;
}

void maBlob::draw(){
    
    lunar.draw();
}

//_____________________________________ SOUND AND OSC.
ofxOscMessage maBlob::getOscMsg(){
    return oscMsg;
}

ofxOscMessage maBlob::getSnapMsg(){
    return snapMsg;
}

void maBlob::setSamplePosition(float _y, float _yTop, float _yBottom){
	position = ofMap(_y, _yTop, _yBottom, 0, 1);
}

void maBlob::setOscMsg(string _type, string _action){
    oscMsg.clear();
    
}

void maBlob::snapSoundMsg(){
    snapMsg.clear();
    snapMsg.setAddress("/freeze");    // type of sound
    snapMsg.addStringArg("true");        // 1. (play / noplay)
    snapMsg.addIntArg(sampleGolpe);      // 2. sample
    snapMsg.addFloatArg(1);              // 3. volume
    snapMsg.addFloatArg(pan);            // 4. pan
}

void maBlob::setWarpedSoundMsg(string _action){
    oscMsg.clear();
    oscMsg.setAddress("/friction");         // type of sound
    oscMsg.addStringArg(_action);           // 1. Action: (play / stop / update)
    oscMsg.addIntArg(maBlobId);             // 2. Line Id
    oscMsg.addIntArg(soundId);              // 3. Sound Id
    oscMsg.addIntArg(sampleContinuo);       // 4. Sample
    oscMsg.addFloatArg(0.6);                // 5. Volume
    oscMsg.addFloatArg(pan);                // 6. Pan
    oscMsg.addFloatArg(position);           // 7. Pointer (Position)
    oscMsg.addFloatArg(pitch);              // 8. Pitch
    oscMsg.addStringArg(samplesLibrary);    // 9. Sample Lybrary
}


