#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	ofSetVerticalSync(true);
    ofBackground(0,0,0);
            
	CT.setup();

    // XML THING
    XML.loadFile("config.xml");
    int findHue = XML.getValue("CONFIG:HUE", 5);
    int findSat = XML.getValue("CONFIG:SAT", 5);
    int findBri = XML.getValue("CONFIG:BRI", 5);
    int anchoHue = XML.getValue("CONFIG:ANCHO_HUE", 5);
    int anchoSat = XML.getValue("CONFIG:ANCHO_SAT", 5);
    int anchoBri = XML.getValue("CONFIG:ANCHO_BRI", 5);
    
    CT.setupHsbValues(findHue, findSat, findBri, anchoHue, anchoSat, anchoBri);

    int radioCircle = XML.getValue("CONFIG:MARGIN_CIRCLE", 10);
    int perimetroBlobMin = XML.getValue("CONFIG:PERIMETRO_BLOB_MIN", 50);
    int perimetroBlobMax = XML.getValue("CONFIG:PERIMETRO_BLOB_MAX", 200);
    
    CT.setupConfigValues(radioCircle, perimetroBlobMin, perimetroBlobMax);

    int x1 = XML.getValue("CONFIG:X1", 0);
    int y1 = XML.getValue("CONFIG:Y1", 0);
    int x2 = XML.getValue("CONFIG:X2", 320);
    int y2 = XML.getValue("CONFIG:Y2", 0);
    int x3 = XML.getValue("CONFIG:X3", 320);
    int y3 = XML.getValue("CONFIG:Y3", 240);
    int x4 = XML.getValue("CONFIG:X4", 0);
    int y4 = XML.getValue("CONFIG:Y4", 240);
    
    CT.setupWarpValues(x1, y1, x2, y2, x3, y3, x4, y4);

	//background.loadImage("devuison.jpg");
	fondo.loadImage("fondo.jpg");
}

//--------------------------------------------------------------
void testApp::update(){
    CT.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofEnableSmoothing();
	ofSetColor(215,215,215);
    fondo.draw(0,0);
    //background.draw(30,400);
    CT.draw();    
    CT.drawWarpIn();
    ofDisableSmoothing();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    int * warpVals;
    CT.keyPressed(key);
	switch(key){
        case 'i':{
			ofToggleFullscreen();
            break;
        }
        case 'l':{
            warpVals = CT.getWarpValues();
            XML.setValue("CONFIG:X1", warpVals[0]);
            XML.setValue("CONFIG:Y1", warpVals[1]);
            XML.setValue("CONFIG:X2", warpVals[2]);
            XML.setValue("CONFIG:Y2", warpVals[3]);
            XML.setValue("CONFIG:X3", warpVals[4]);
            XML.setValue("CONFIG:Y3", warpVals[5]);
            XML.setValue("CONFIG:X4", warpVals[6]);
            XML.setValue("CONFIG:Y4", warpVals[7]);
			XML.saveFile("config.xml");
            break;
        }
        case 'o':{
            int * hsbVals = CT.getHsbValues();
            XML.setValue("CONFIG:HUE", hsbVals[0]);
            XML.setValue("CONFIG:SAT", hsbVals[1]);
            XML.setValue("CONFIG:BRI", hsbVals[2]);
            XML.setValue("CONFIG:ANCHO_HUE", hsbVals[3]);
            XML.setValue("CONFIG:ANCHO_SAT", hsbVals[4]);
            XML.setValue("CONFIG:ANCHO_BRI", hsbVals[5]);
			XML.saveFile("config.xml");
            break;
        }
        case '+':{
            int * configVals = CT.getConfigValues();
            XML.setValue("CONFIG:MARGIN_CIRCLE", configVals[0]);
            XML.setValue("CONFIG:PERIMETRO_BLOB_MIN", configVals[1]);
            XML.setValue("CONFIG:PERIMETRO_BLOB_MAX", configVals[2]);
			XML.saveFile("config.xml");
            break;
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    CT.mouseDragged(x, y, button);   
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    CT.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    CT.mouseReleased(x, y, button);    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
