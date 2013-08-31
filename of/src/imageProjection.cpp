#include "imageProjection.h"

//------------------------------------------------------------------
imageProjection::imageProjection(){
}

void imageProjection::setup(){
    xProxeccion = 1024;
    nLineas = 0;
    contadorLineIds = 0;
    samplesLibrary = "frase";
    
    sender.setup(HOST, SEND_PORT);
    receiver.setup(RECEIVING_PORT);
    // timeline setup
    tLine.setup(1024, 2048, 0, 768);
	currTime = 0;
    
    // Samples stup
    // samplesFrases [] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
}

void imageProjection::reset(){
    printf("Reset \n");

    // Check if there are messages from OSCreceiver
    receiveOscMsg();

    if(nLineas > 0){
        for(int i=0; i<nLineas; i++){
            if(!lineas[i].getUpdated()){
                //lineas[i].killLine();
                // Mandamos unha mensaxe para que mate o son
                sendKillOscMsg(lineas[i].getLineId(), lineas[i].getSoundId());
                lineas.erase(lineas.begin()+i);
                nLineas = lineas.size();
            }else{
                lineas[i].setUpdated(false);
            }
        }
    }
}

// This is update is called on each blob
void imageProjection::update(ofPoint _centro, ofRectangle _arredorRect){
    //chekeamos si o blob está dentro da linea
    // primeiro chekeamos si hai algunha linea creada
    if(nLineas > 0){
        bool checkDrawnAtBlob = false;
        for(int i = 0; i < nLineas; i++){
            printf("linea n %i, id %i, soundid %i \n", i, lineas[i].getLineId(), lineas[i].getSoundId() );

                //Chekeamos si o blob esta dentro dunha linea
                if(lineas[i].isInsideMe(_centro)){
                    
                    // Boolean for saying that the blob is inside a line.
                    checkDrawnAtBlob = true;
                    
                    // If the line doesn't have a blob yet
                    if(!lineas[i].getUpdated()){

                        // if there is a blob inside this line, its boundingRect is updated
                        lineas[i].setBlobOrigen(_centro, _arredorRect);
                        
                        // Miramos cal e o blob que esta mais perto a este.
                        float distanciaMinima = 9999;
                        
                        if(nLineas>1){

                            int blobMaisPerto;
                            for(int j=0; j < nLineas; j++){
                                if(i!=j){
                                    float distanciaEntreBlobs = ofDist(lineas[i].getCentro().x, lineas[i].getCentro().y, lineas[j].getCentro().x, lineas[j].getCentro().y);
                                    
                                    if(distanciaEntreBlobs < distanciaMinima){
                                        blobMaisPerto = j;
                                        distanciaMinima = distanciaEntreBlobs;
                                    }
                                }
                            }
                        }

                        printf("distancia minima %f \n", distanciaMinima );

                        lineas[i].update(reubicarPunto(_centro), reubicarRectangulo(_arredorRect), distanciaMinima);
                        lineas[i].setUpdated(true);

                        // send the warp sound message for the line[i]
                        sendOscMsg(lineas[i].getOscMsg());
                
                        // Receive OSC
                        //receiveOscMsg();
                    }
                }    
        }
        
        // creamos un debuxo si non hai ningún que se corresponda con este blob
        if(!checkDrawnAtBlob){
            creaDebuxo(_centro, _arredorRect);
        }
        
    // creamos un debuxo si todavía non hai ningún creado
    }else{
        creaDebuxo(_centro, _arredorRect);
    }
}

// Despite this function is called updateLines, 
void imageProjection::updateLines(){
    printf("Update Line \n");
    float lTop = 768;
    float lBottom = 0;
    
    for(int i=0; i<nLineas; i++){
        // Check if the general timeline snaps a line
        if(lineas[i].checkTimeline(tLine.getYPos())){
            // send the snap sound message for the line[i]
            sendOscMsg(lineas[i].getOscMsg());
        }
        // Update limits of the general TimeLine.
        if(lineas[i].getLimitTop()<lTop){
            lTop = lineas[i].getLimitTop();
        }
        if(lineas[i].getLimitBottom()>lBottom){
            lBottom = lineas[i].getLimitBottom();
        }
    }
    
    //timeline update
    tLine.update(lTop-10, lBottom+10);    
}

void imageProjection::draw(){
    
    // Draw net: lines between the blobs
    for (int j=0; j<lineas.size(); j++) {
        for (int k=0; k<lineas.size(); k++) {
            if(k != j){
                ofLine(lineas[j].getCentro().x, lineas[j].getCentro().y, lineas[k].getCentro().x, lineas[k].getCentro().y);
                //drawWavyLine(lineas[j].getCentro(), lineas[k].getCentro(), lineas[k].getRadio());
            }
        }
    }
        
    // Blobs draw
    for (int i=0; i<lineas.size(); i++) {
        lineas[i].draw();
    }
}

void imageProjection::drawWavyLine(ofPoint a, ofPoint b, float _radio) {
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
    
	for(float f = 0; f < PI*2; f+=PI/28.f) {
		float d = f/TWO_PI;
        
		float window = 1-4*(d-0.5)*(d-0.5);
        // para cambiar a velocidade da linha: 'sin(f+currTime*20)'
        printf("Radio do blob: %f \n", _radio);
        printf("Radio do blob mapped: %f \n", radioMapped);
		ofPoint sine = n * sin(f+currTime*20)*ofMap(length, 0, 200, 0, radioMapped)*window;
		ofPoint p = sine + a + diff * d;
        
		glVertex2f(p.x, p.y);
	}
	glVertex2f(b.x, b.y);
    
	glEnd();
}

void imageProjection::creaDebuxo(ofPoint _blobPunto, ofRectangle _blobRect){
    linea lineaCreada = linea();
    
    float radioOrixinal = _blobRect.width;
    
    // relocalizamos as coordenadas do blob para a proxeccion
    lineaCreada.setup(reubicarPunto(_blobPunto), reubicarRectangulo(_blobRect), contadorLineIds, samplesLibrary, radioOrixinal);
    lineaCreada.setBlobOrigen(_blobPunto, _blobRect);
    lineaCreada.setUpdated(true);
    sendOscMsg(lineaCreada.getOscMsg());
    
    // engadimos a lina ao array
    lineas.push_back(lineaCreada);
    // send the warp sound message for the line[i]
    //sendCreationOscMsg(contadorLineIds);
        
    int lId = lineas[nLineas].getLineId();
    int sId = lineas[nLineas].getSoundId();
//    printf("linea creada. SoundId: %i, lineId: %i \n", sId, lId);
    nLineas = lineas.size();   
    contadorLineIds++;
}

ofPoint imageProjection::reubicarPunto(ofPoint _punto){
    ofPoint punto;
    float lineaIPX = ofMap(_punto.x, 0, 320, 0, 1024);
    float lineaIPY = ofMap(_punto.y, 0, 240, 0, 768);
    punto.set(lineaIPX + xProxeccion, lineaIPY);

    return punto;
}

ofRectangle imageProjection::reubicarRectangulo(ofRectangle _rect){
    ofRectangle rectangulo;
    rectangulo.x = ofMap(_rect.x, 0, 320, 0, 1024);
    rectangulo.y = ofMap(_rect.y, 0, 240, 0, 768);
    rectangulo.width = ofMap(_rect.width, 0, 320, 0, 1024);
    rectangulo.height = ofMap(_rect.height, 0, 240, 0, 768);
    
    return rectangulo;
}

void imageProjection::borrarDebuxos(){
    lineas.clear();    
    nLineas = 0;
}

//--------------------------------------------------------------
void imageProjection::keyPressed(int key){
	switch(key){
        case 'u':
            borrarDebuxos();
            break;
        case '1':
            samplesLibrary = "frase";
            break;
        case '2':
            samplesLibrary = "ritmos";
            break;
        case '3':
            samplesLibrary = "friccion";
            break;
	}
}

//------------- block of OSC messages --------------------------
void imageProjection::sendOscMsg(ofxOscMessage _m){
    sender.sendMessage(_m);    
}

void imageProjection::sendKillOscMsg(int _lId, int _sId){
    ofxOscMessage oscMsg;
    oscMsg.setAddress("/kill");
    oscMsg.addStringArg("true");    // 1
    oscMsg.addIntArg(_lId);         // 2. lineId
    oscMsg.addIntArg(_sId);         // 3. soundId
    sender.sendMessage(oscMsg);    
}

void imageProjection::sendCreationOscMsg(int _lId){
    printf("enviando funcion...\n");
    ofxOscMessage oscMsg;
    oscMsg.setAddress("/friction");     // type of sound
    oscMsg.addStringArg("play");       // 1. Action: (play / stop / update)
    oscMsg.addIntArg(_lId);           // 2. Line Id
    oscMsg.addIntArg(0);          // 3. Sound Id
    oscMsg.addIntArg(3);                // 4. Sample
    oscMsg.addFloatArg(0.8);			// 5. Volume
    oscMsg.addFloatArg(0.5);			// 6. Pan
    oscMsg.addFloatArg(0.0);       // 7. Pointer (Position)
    oscMsg.addFloatArg(1.0);          // 8. Pitch
    sender.sendMessage(oscMsg);    
}

void imageProjection::receiveOscMsg(){
    printf("recibindo funcion...\n");
	while(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		if(m.getAddress()=="/soundId"){
			int lineId = m.getArgAsInt32(0);
			int soundId = m.getArgAsInt32(1);
            for(int i=0; i<lineas.size(); i++){
                if(lineas[i].getLineId()==lineId){
                    lineas[i].setSoundId(soundId);
                }
            }
            printf("recibido soundId: %i, lineId %i \n", soundId, lineId);
		}
	}
}

