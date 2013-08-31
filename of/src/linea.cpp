#include "linea.h"

//------------------------------------------------------------------
linea::linea(){
}

linea::~linea(){
    oscMsg.clear();
}

void linea::setup(ofPoint _centro, ofRectangle _arredorRect, int _id, string _sl, float _radioOrixinal){
    centro = _centro;
    arredorRect = _arredorRect;
    lineId = _id;
    samplesLibrary = _sl;
    distanceLineDrawY = 0;
    ofSeedRandom();
    pan = ofRandomf();
    timeLineSoundPlaying = false;
    pitch = 1;
    r = _radioOrixinal;
    rOrixinal = _radioOrixinal;
    
    float yTop = arredorRect.y;
    float yBottom = arredorRect.y + arredorRect.height;

    tBall.setup(centro.x - (arredorRect.width/2)-distanceLineDrawY, yTop, yBottom);
//    tBall.setup(centro.x, yTop, yBottom);
    lunar.setup(centro.x, centro.y, r);

    setSamplePosition(tBall.getYPos(), yTop, yBottom);
    
    ofSeedRandom();
    float sc = ofRandom(0,9);
    sampleContinuo = int(sc + 0.5);
    float sg = ofRandom(0,9);
    sampleGolpe = int(sg + 0.5);
    
    //printf("sampleId: %i", sampleContinuo);
    
    setWarpedSoundMsg("play");
}

void linea::update(ofPoint _centro, ofRectangle _arredorRect, float _distanciaMinima){
    centro = _centro;
    arredorRect = _arredorRect;
    r = rOrixinal;
    
    float yTop = arredorRect.y;
    float yBottom = arredorRect.y + arredorRect.height;
    
    tBall.update(centro.x - (arredorRect.width/2)-distanceLineDrawY, yTop, yBottom);
//    tBall.update(centro.x, yTop, yBottom);
    
    if(_distanciaMinima<350.0){
        r += ofMap(_distanciaMinima, 0.0, 350.0, 30.0, 5.0);
        pitch = ofMap(_distanciaMinima, 0.0, 350.0, 0.5, 2.0);
    }else{
        pitch = 2.0;
    }
    printf("pitch: %f", pitch);

    lunar.update(centro.x, centro.y, r, rOrixinal);

    setSamplePosition(tBall.getYPos(), yTop, yBottom);
    
    setWarpedSoundMsg("update");
}

bool linea::checkTimeline(float _timeLineYPos){
    bool returnValue = false;
    if(_timeLineYPos > arredorRect.y && _timeLineYPos < arredorRect.y + arredorRect.height){
        if(timeLineSoundPlaying == false){
            //we put "play snap sound messagge" in OSC
            snapSoundMsg();
            timeLineSoundPlaying = true;
            returnValue = true;
        }else{
            returnValue = false;
        }
    }else{
        timeLineSoundPlaying = false;
        returnValue = false;
    }
    return returnValue;
}

void linea::setBlobOrigen(ofPoint _centro, ofRectangle _arredorRect){
    centroBlobOrigen = _centro;
    arredorRectBlobOrigen = _arredorRect;
}

void linea::setSoundId(int _soundId){
    soundId = _soundId;
}

void linea::setUpdated(bool _updated){
    updated = _updated;
}

bool linea::getUpdated(){
    return updated;
}

int linea::getLineId(){
    return lineId;
}

int linea::getSoundId(){
    return soundId;
}

float linea::getRadio(){
    return r;
}

float linea::getLimitTop(){
    return arredorRect.y;
}

float linea::getLimitBottom(){
    return arredorRect.y + arredorRect.height;
}

ofPoint linea::getCentro(){
    return centro;
}

void linea::draw(){
	//ofEnableSmoothing();
	//ofEnableAlphaBlending();
	
    //tBall.draw();
    lunar.draw();
    
    ofSetColor(255, 255, 255, 255);

    // linea do centro
    //ofCircle(centro.x, centro.y, 5);
    //ofLine(centro.x, centro.y - (arredorRect.height/2), centro.x, centro.y + (arredorRect.height/2));

    // linea do wave
    //ofCircle(centro.x - arredorRect.width, centro.y, 5);
    //ofLine(centro.x - (arredorRect.width/2)-distanceLineDrawY, centro.y - (arredorRect.height/2), centro.x - (arredorRect.width/2)-distanceLineDrawY, centro.y + (arredorRect.height/2));
    
	//ofDisableAlphaBlending();
    
    //ofDrawBitmapString(ofToString(lineId), centro.x - arredorRect.width-15, centro.y);

	//ofDisableSmoothing();
}

bool linea::isInsideMe(ofPoint _centroBlob){
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

void linea::killLine(){
    //setOscMsg("kill");
}

//_____________________________________ SOUND AND OSC.
void linea::setSamplePosition(float _y, float _yTop, float _yBottom){
	position = ofMap(_y, _yTop, _yBottom, 0, 1);
}

ofxOscMessage linea::getOscMsg(){
    return oscMsg;
}

void linea::setOscMsg(string _type, string _action){
    oscMsg.clear();

}

void linea::snapSoundMsg(){
    oscMsg.clear();
    oscMsg.setAddress("/collision");    // type of sound
    oscMsg.addStringArg("true");        // 1. (play / noplay)
    oscMsg.addIntArg(sampleGolpe);      // 2. sample
    oscMsg.addFloatArg(1);              // 3. volume
    oscMsg.addFloatArg(pan);            // 4. pan
}

void linea::setWarpedSoundMsg(string _action){
    oscMsg.clear();
    oscMsg.setAddress("/friction");         // type of sound
    oscMsg.addStringArg(_action);           // 1. Action: (play / stop / update)
    oscMsg.addIntArg(lineId);               // 2. Line Id
    oscMsg.addIntArg(soundId);              // 3. Sound Id
    oscMsg.addIntArg(sampleContinuo);       // 4. Sample
    oscMsg.addFloatArg(0.6);                // 5. Volume
    oscMsg.addFloatArg(pan);                // 6. Pan
    oscMsg.addFloatArg(position);           // 7. Pointer (Position)
    oscMsg.addFloatArg(pitch);              // 8. Pitch
    oscMsg.addStringArg(samplesLibrary);    // 9. Sample Lybrary
}