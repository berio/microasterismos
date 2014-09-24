//
//  maProjection.cpp
//  fireflyMvFindObjects
//
//  Created by Berio Molina on 07/05/14.
//
//

#include "maProjection.h"

//------------------------------------------------------------------
maProjection::maProjection(){
}

void maProjection::setup(){
    xProjection = 1478; // 1448
    contadorMaBlobs = 0;
    samplesLibrary = "un";
    blobPanned = false;
	currTime = 0;
    
    // OSC
    sender.setup(HOST, SEND_PORT);
    receiver.setup(RECEIVING_PORT);
}

void maProjection::reset(){
    
    // Check if there are messages from OSCreceiver
    receiveOscMsg();
    
    vector <int>  maBlobsToKill;
    list<maBlob>::iterator iMaBlob;
    
    if(maBlobs.size() > 0){
        for(iMaBlob = maBlobs.begin(); iMaBlob != maBlobs.end(); iMaBlob++) {
            if(!iMaBlob->getUpdated()){

                ofLog(OF_LOG_NOTICE, "matar blob");
                
                // PRESET #5 Creamos un freeze
                if(samplesLibrary == "cinco"){
                    
                    bool canCreateFreeze = true;
                    int radio = 4;
                    
                    if(freezeList.size()>0){
                        list<freeze>::iterator mFreeze;
                        for(mFreeze = freezeList.begin(); mFreeze != freezeList.end(); mFreeze++) {
                            
                            if(mFreeze->x < (iMaBlob->getCentro().x + radio)
                               && mFreeze->x > (iMaBlob->getCentro().x - radio)
                               && mFreeze->y < (iMaBlob->getCentro().y + radio)
                               && mFreeze->y > (iMaBlob->getCentro().y - radio)
                               ){
                                canCreateFreeze = false;
                            }
                        }
                    }else{
                        canCreateFreeze = true;
                    }
                    
                    if(canCreateFreeze){
                        ofPoint freezeCentro = iMaBlob->getCentro();
                        freeze freezeCreado = freeze(freezeCentro.x, freezeCentro.y);
                        freezeList.push_back(freezeCreado);
                        ofLog(OF_LOG_NOTICE, "creado un freeze");
                    }
                }

                sendKillOscMsg(iMaBlob->getMaBlobId(), iMaBlob->getSoundId());
                maBlobs.erase(iMaBlob);

            }
        }
    }
    
    list<maBlob>::iterator jMaBlob;
    for(jMaBlob = maBlobs.begin(); jMaBlob != maBlobs.end(); jMaBlob++) {
        jMaBlob->setUpdated(false);
    }

    // update bigWaves if PRESET #1
    if(bigWaves.size()>0){
        
        list<bigWave>::iterator iBigWave;
        
        for(iBigWave = bigWaves.begin(); iBigWave != bigWaves.end(); iBigWave++) {
            iBigWave->update();
            
            if(iBigWave->getKillMe()){
                bigWaves.erase(iBigWave);
            }
        }
    }
    
    // Delete freezes if not in PRESET #5
    if(samplesLibrary != "cinco"){
        freezeList.clear();
    }
}

// This update function is called on every blob
void maProjection::update(ofPoint _centro, ofRectangle _arredorRect){
    
    
    int nMaBlobs = maBlobs.size();
    list<maBlob>::iterator iMaBlob;
    
    if(nMaBlobs > 0){
        bool maBlobExists = false;
        
        for(iMaBlob = maBlobs.begin(); iMaBlob != maBlobs.end(); iMaBlob++) {
            
            //Chekeamos si o blob esta dentro dun maBlob
            if(iMaBlob->isInsideMe(_centro)){
                
                // Boolean for saying that the blob is inside a line.
                maBlobExists = true;
                
                // If the line doesn't have a blob yet
                if(!iMaBlob->getUpdated()){
                    
                    // if there is a blob inside this line, its boundingRect is updated
                    iMaBlob->setBlobOrigen(_centro, _arredorRect);
                    
                    // Miramos cal e o blob que esta mais perto a este.
                    float distanciaMinima = 9999;
                    
                    list<maBlob>::iterator jMaBlob;
                    
                    if(nMaBlobs>1){
                        for(jMaBlob = maBlobs.begin(); jMaBlob != maBlobs.end(); jMaBlob++) {
                            if(iMaBlob!=jMaBlob){
                                float distanciaEntreBlobs = ofDist(iMaBlob->getCentro().x, iMaBlob->getCentro().y, jMaBlob->getCentro().x, jMaBlob->getCentro().y);
                                
                                if(distanciaEntreBlobs < distanciaMinima){
                                    distanciaMinima = distanciaEntreBlobs;
                                }
                            }
                        }
                    }
                    
                    // pan entre -1 e 1
                    float pan;
                    //float pan = ofRandomf();
                    
                    if(nMaBlobs == 1){
                        pan = 0.0f;
                        blobPanned = false;
                    }else if (nMaBlobs == 2){
                        if(!blobPanned){
                            pan = 0.4f;
                            blobPanned = true;
                        }else{
                            pan = -0.4f;
                            blobPanned = false;
                        }
                    }else{
                        pan = 0.0f;
                    }
                    
                    iMaBlob->update(reubicarPunto(_centro), reubicarRectangulo(_arredorRect), distanciaMinima, pan);
                    iMaBlob->setUpdated(true);
                    
                    // send the warp sound message for the maBlob[i]
                    sendOscMsg(iMaBlob->getOscMsg());
                    
                    // Receive OSC
                    //receiveOscMsg();
                }
            }
        }
        
        // creamos un debuxo si non hai ningún que se corresponda con este blob
        if(!maBlobExists){
            creaMaBlob(_centro, _arredorRect);
        }
        
        // Si todavia non se creou ningun maBlob, creamos un.
    }else{
        creaMaBlob(_centro, _arredorRect);
    }
}

void maProjection::draw(){
    
    int nMaBlobs = maBlobs.size();
    
    // Draw net: lines between the blobs
    list<maBlob>::iterator jMaBlob;
    list<maBlob>::iterator kMaBlob;
    
    for(jMaBlob = maBlobs.begin(); jMaBlob != maBlobs.end(); jMaBlob++) {
        for(kMaBlob = maBlobs.begin(); kMaBlob != maBlobs.end(); kMaBlob++) {
            if(kMaBlob != jMaBlob){
                ofLine(jMaBlob->getCentro().x, jMaBlob->getCentro().y, kMaBlob->getCentro().x, kMaBlob->getCentro().y);
                
                // PRESET #3. Foot. Draw Wavy lines
                if(samplesLibrary == "tres"){
                    drawWavyLine(jMaBlob->getCentro(), kMaBlob->getCentro(), 10);
                }
            }
        }
    }
    
    // PRESET #1 draw bigWaves
    if(bigWaves.size()>0){
        list<bigWave>::iterator iBigWave;
        for(iBigWave = bigWaves.begin(); iBigWave != bigWaves.end(); iBigWave++) {
            iBigWave->draw();
        }
    }
    
    // PRESET #5 draw freezes
    drawConstellation();
    
    // Draw Blobs
    list<maBlob>::iterator iMaBlob;
    for(iMaBlob = maBlobs.begin(); iMaBlob != maBlobs.end(); iMaBlob++) {
        iMaBlob->draw();
    }
}

void maProjection::drawConstellation(){
    
    if(freezeList.size()>0){
        list<freeze>::iterator iFreeze;
        
        freeze freezeDestino = freeze(0,0);

        for(iFreeze = freezeList.begin(); iFreeze != freezeList.end(); iFreeze++) {
            ofLog(OF_LOG_NOTICE, "iFreeze->x: "+ ofToString(iFreeze->x));
            if(freezeList.size()>1){
                list<freeze>::iterator jFreeze;
                float distanciaTotal = 9999999;
                
                for(jFreeze = freezeList.begin(); jFreeze != freezeList.end(); jFreeze++) {
                    
                    if(jFreeze->x != iFreeze->x){
                        float distancia =  ofDist(iFreeze->x, iFreeze->y, jFreeze->x, jFreeze->y);
                        if(distancia < distanciaTotal){
                            distanciaTotal = distancia;
                            freezeDestino = *jFreeze;
                        }
                    }
                }
            }
            
            if(freezeList.size()>1){
                ofSetColor(255, 255, 255, 100);
                ofLine(iFreeze->x, iFreeze->y, freezeDestino.x, freezeDestino.y);
            }
            iFreeze->draw();
        }
    }

    
}

void maProjection::drawWavyLine(ofPoint a, ofPoint b, float _radio) {
    float radioMapped = ofMap(_radio, 10.0, 50.0, 0.0, 20.0);
    //r += ofMap(_distanciaMinima, 0.0, 350.0, 30.0, 5.0);
    currTime = ofGetElapsedTimef();
    
	ofPoint diff = (b-a);
	float length = diff.length();
    
	// now calculate the normal, normalized
	ofPoint n = diff/length;
    
	// turn (x, y) into (y, -x) - this flips the vector 90 degrees
	float ny = n.y;
	n.y = -n.x;
	n.x = ny;
    
	glBegin(GL_LINE_STRIP);
    
	for(float f = 0; f < PI*2; f+=PI/5.f) {
		float d = f/TWO_PI;
        
		float window = 1-4*(d-0.5)*(d-0.5);
        // para cambiar a velocidade da linha: 'sin(f+currTime*20)'
        printf("Radio do blob: %f \n", _radio);
        printf("Radio do blob mapped: %f \n", radioMapped);
		ofPoint sine = n * sin(f+currTime*20)*ofMap(length, 0, 200, 0, 8)*window;
		ofPoint p = sine + a + diff * d;
        
		glVertex2f(p.x, p.y);
	}
	glVertex2f(b.x, b.y);
    
	glEnd();
}

void maProjection::creaMaBlob(ofPoint _centro, ofRectangle _arredorRect){
    
    ofPoint centroReubicado = reubicarPunto(_centro);
    
    maBlob maBlobCreado = maBlob();
    
    float radioOrixinal = _arredorRect.width;
    
    // PRESET #2 Cometas
    if(samplesLibrary == "dous"){
        maBlobCreado.setup(centroReubicado, reubicarRectangulo(_arredorRect), contadorMaBlobs, samplesLibrary, radioOrixinal, "cometa", 2000);
    }else{
        maBlobCreado.setup(centroReubicado, reubicarRectangulo(_arredorRect), contadorMaBlobs, samplesLibrary, radioOrixinal, "normal", 1000);
    }
    maBlobCreado.setBlobOrigen(_centro, _arredorRect);
    maBlobCreado.setUpdated(true);
    sendOscMsg(maBlobCreado.getOscMsg());
    ofLog(OF_LOG_NOTICE, "crea maBlobId: " + ofToString(contadorMaBlobs));
    
    // engadimos o maBlob ao array
    maBlobs.push_back(maBlobCreado);
    
    contadorMaBlobs++;
    
    // PRESET #1 Creamos unha onda grande
    if(samplesLibrary == "un"){
        bigWave bigWaveCreado = bigWave(centroReubicado.x, centroReubicado.y);
        bigWaves.push_back(bigWaveCreado);
    }

}

ofPoint maProjection::reubicarPunto(ofPoint _punto){
    ofPoint punto;
    //    float lineaIPX = ofMap(_punto.x, 0, 640, 0, 1024);
    //    float lineaIPY = ofMap(_punto.y, 0, 480, 0, 768);
    float lineaIPX = ofMap(_punto.x, 0, 640, 0, 1024);
    float lineaIPY = ofMap(_punto.y, 0, 480, 0, 768);
    punto.set(lineaIPX + xProjection, lineaIPY);
    
    return punto;
}

ofRectangle maProjection::reubicarRectangulo(ofRectangle _rect){
    ofRectangle rectangulo;
    rectangulo.x = ofMap(_rect.x, 0, 640, 0, 1024);
    rectangulo.y = ofMap(_rect.y, 0, 480, 0, 768);
    rectangulo.width = ofMap(_rect.width, 0, 640, 0, 1024);
    rectangulo.height = ofMap(_rect.height, 0, 480, 0, 768);
    
    return rectangulo;
}

void maProjection::borrarDebuxos(){
    maBlobs.clear();
}

void maProjection::borrarFreezes(){
    freezeList.clear();
}

void maProjection::setSamplesLibrary(string _samples){
    samplesLibrary = _samples;
}

//------------- block of OSC messages --------------------------
void maProjection::sendOscMsg(ofxOscMessage _m){
    sender.sendMessage(_m);
}

void maProjection::sendKillOscMsg(int _lId, int _sId){
    ofxOscMessage oscMsg;
    oscMsg.setAddress("/kill");
    oscMsg.addStringArg("true");    // 1
    oscMsg.addIntArg(_lId);         // 2. lineId
    oscMsg.addIntArg(_sId);         // 3. soundId
    sender.sendMessage(oscMsg);
    ofLog(OF_LOG_NOTICE, "matando OSC msg para o maBlobId: " + ofToString(_lId));
}

void maProjection::sendKillAllSoundsOscMsg(){
    ofxOscMessage oscMsg;
    oscMsg.setAddress("/killall");
    oscMsg.addStringArg("true");    // 1
    sender.sendMessage(oscMsg);
}

void maProjection::receiveOscMsg(){
	while(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		if(m.getAddress()=="/soundId"){
			int maBlobId = m.getArgAsInt32(0);
			int soundId = m.getArgAsInt32(1);
            
            list<maBlob>::iterator iMaBlob;
            for(iMaBlob = maBlobs.begin(); iMaBlob != maBlobs.end(); iMaBlob++) {
                if(iMaBlob->getMaBlobId()==maBlobId){
                    iMaBlob->setSoundId(soundId);
                }
            }
            //ofLog(OF_LOG_NOTICE, "recivido maBlobId: " + ofToString(maBlobId) + " soundid: " + ofToString(soundId));
		}
	}
}
