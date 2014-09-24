#include "ofApp.h"
#include "ofxGui.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
    ofBackground(0,0,0);
    
    mainGui.setup();
    showWarpImage = trackearBlobs = false;
    
	// If you want to set any non-default parameters like size, format7, blocking
	// capture, etc., you can do it here before setup. They'll be applied to the
	// camera during setup().
	
	// setup() will pick the first camera. Or if there are multiple cameras you
	// can use the number, or the GUID of the camera.
	camera.setup();
    camera.setExposureAutoOnFlag(false);
    camera.setBrightnessAutoOnFlag(false);
    camera.setGammaAutoOnFlag(false);
    camera.setGainAutoOnFlag(false);
    camera.setShutterAutoOnFlag(false);
    isCameraSettingsAuto = true;
    
    w = 640;
    h = 480;
    
    setupWarpAreas();
	dstWarperPositions  = new ofPoint[4];
	dstWarperPositions[0].set(0, 0);
	dstWarperPositions[1].set(w, 0);
	dstWarperPositions[2].set(w, h);
	dstWarperPositions[3].set(0, h);
    warpImage.allocate(w, h);
    
    projection.setup();
    
	// After setup it's still possible to change a lot of parameters. If you want
	// to change a pre-setup parameter, the camera will auto-restart
	
	// Set the window size to the camera size.
	//ofSetWindowShape(camera.getWidth(), camera.getHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    //ofLog(OF_LOG_NOTICE, "exposure GUI " + ofToString(mainGui.getShutter()));
	// grabVideo() will place the most recent frame into curFrame. If it's a new
	// frame, grabFrame returns true. If there are multiple frames available, it
	// will drop old frames and only give you the newest. This guarantees the
	// lowest latency. If you prefer to not drop frames, set the second argument
	// (dropFrames) to false. By default, capture is non-blocking.
	if(camera.grabVideo(curFrame)) {
		curFrame.update();
        
        grayImage.setFromPixels( curFrame.getPixelsRef() );
        warpImage.warpIntoMe(grayImage, mainGui.getWarpPoints(), dstWarperPositions);
        grayImage = warpImage;
        warpImage.blurGaussian( mainGui.getBlur() );
        //
        warpImage.threshold( mainGui.getThreshold() );
        warpImage.invert();
        contourFinder.findContours( warpImage, mainGui.getMinArea(), mainGui.getMaxArea(), 20, true );
        
        // BLOBS !!
        vector<ofxCvBlob>  &blobs = contourFinder.blobs;
        int n = blobs.size();     //Get number of blobs
        
        // Reset all maBlobs of Porjection class
        projection.reset();
        
        selectedBlobs.clear();
        ofVec2f * findAreaPoints = mainGui.getFindAreaPoints();
        
        for (int i=0; i<n; i++) {
            
            // 1. First check if blobs are inside of Find Area
            if(blobs[i].centroid.x > findAreaPoints[0].x &&
               blobs[i].centroid.y > findAreaPoints[0].y &&
               blobs[i].centroid.x < findAreaPoints[2].x &&
               blobs[i].centroid.y < findAreaPoints[2].y){
                
                /* 2. Comprobamos si o blob e un CIRCULO
                 Recorremos todos os puntos do contorno do blob, para saber a que distancia estan
                 do centro do blob. Si todos os puntos estan mais ou menos a mesma distancia, enton
                 estamos ante un circulo */
                vector <ofPoint> blobPts = blobs[i].pts;
                
                float margenMinima = 10.0;
                float minimo = 9000.0;
                float maximo = 0.0;
                for(int j=0; j<blobs[i].nPts; j++){
                    float distanciaAlCentro = ofDist(blobPts[j].x, blobPts[j].y, blobs[i].centroid.x, blobs[i].centroid.y);
                    minimo = fmin(minimo, distanciaAlCentro);
                    maximo = fmax(maximo, distanciaAlCentro);
                }
                float margen = maximo - minimo;
                
                if(margen < margenMinima){
                    
                    /* 3. Checkeamos si o blob Ž un dos que estamos trackeando*/
                    if(trackearBlobs){
                        trackedBlobs.push_back(blobs[i].centroid);
                    }else{
                        float margenCheckTrackedBlobs = 15.0;
                        
                        for(int k=0; k<trackedBlobs.size(); k++){
                            float distanciaTrackedAoCentroBlob = ofDist(trackedBlobs[k].x, trackedBlobs[k].y, blobs[i].centroid.x, blobs[i].centroid.y);
                            
                            if(distanciaTrackedAoCentroBlob < margenCheckTrackedBlobs){
                                selectedBlobs.push_back(blobs[i].centroid);
                                trackedBlobs[k] = blobs[i].centroid;
                                
                                // Send the blob to the projection
                                projection.update(blobs[i].centroid, blobs[i].boundingRect);
                            }
                        }
                    }
                }
            }
        }
        trackearBlobs = false;
        
	}
    
    if(mainGui.getCameraAuto()){
        if(isCameraSettingsAuto){
            isCameraSettingsAuto = false;
            camera.setExposureAutoOnFlag(true);
            camera.setBrightnessAutoOnFlag(true);
            camera.setGammaAutoOnFlag(true);
            camera.setGainAutoOnFlag(true);
        }
        mainGui.setCameraAuto(camera.getShutter(), camera.getExposure(), camera.getBrightness(), camera.getGamma(), camera.getGain());
    }else{
        if(!isCameraSettingsAuto){
            isCameraSettingsAuto = true;
            camera.setExposureAutoOnFlag(false);
            camera.setBrightnessAutoOnFlag(false);
            camera.setGammaAutoOnFlag(false);
            camera.setGainAutoOnFlag(false);
            camera.setShutterAutoOnFlag(false);
        }
        camera.setShutter(mainGui.getShutter());
        camera.setExposure(mainGui.getExposure());
        camera.setBrightness(mainGui.getBrightness());
        camera.setGamma(mainGui.getGamma());
        camera.setGain(mainGui.getGain());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255, 255, 255);
	// If the camera isn't ready, the curFrame will be empty.
	if(camera.isReady()) {
		// Camera doesn't draw itself, curFrame does.
        if(showWarpImage){
            grayImage.draw(0, 0);
        }else{
            curFrame.draw(0, 0);
        }
        
        grayImage.draw( w + 1, 0, w*0.5, h*0.5 );
        contourFinder.draw( w + 1, 0, w*0.5, h*0.5 );
        warpImage.draw(w+1, h*0.5, w*0.5, h*0.5);
        
        // draw selected blobs
        ofSetColor(59, 224, 9);
        for(int i=0; i<selectedBlobs.size(); i++){
            ofCircle(selectedBlobs[i].x, selectedBlobs[i].y, 10);
        }
	}
    
    //GUI
    mainGui.draw();
    
    // Chekear a onde chega o primeiro screen en relacion co segundo
    ofCircle(1448 + 2, 100, 8);
    
    projection.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key) {
        // salvar warp poits 1
        case 'q':
            saveArea(0);
            break;
            
        // salvar warp poits 2
        case 'w':
            saveArea(1);
            break;

        // salvar warp poits 3
        case 'e':
            saveArea(2);
            break;

        // salvar warp poits 4
        case 'r':
            saveArea(3);
            break;

        // salvar warp poits 5
        case 't':
            saveArea(4);
            break;
            
        case 'a':
            showWarpImage = !showWarpImage;
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'b':
            projection.borrarDebuxos();
            break;
        case 'v':
            projection.borrarFreezes();
            break;
        case 'c': // reseteamos os tracked blobs
            trackedBlobs.clear();
            trackearBlobs = true;
            break;
        case '1':
            projection.setSamplesLibrary("un");
            loadPresetArea(0);
            //ofPoint *puntos = new ofPoint[4];
            //puntos = mainGui.getWarpPoints();
            //warpAreas.setPuntos();
            //mainGui.setWarpPoints(puntos[0], puntos[0], puntos[0], puntos[0]);
            break;
        case '2':
            projection.setSamplesLibrary("dous");
            loadPresetArea(1);
            break;
        case '3':
            projection.setSamplesLibrary("tres");
            loadPresetArea(2);
            break;
        case '4':
            projection.setSamplesLibrary("catro");
            loadPresetArea(3);
            break;
        case '5':
            projection.setSamplesLibrary("cinco");
            loadPresetArea(4);
            break;
        case 'k':
            projection.sendKillAllSoundsOscMsg();
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    mainGui.mouseDragged(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mainGui.mousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mainGui.mouseReleased(x, y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

void ofApp::saveArea(int a){
    ofPoint *warpPuntos = mainGui.getWarpPoints();

    XML.pushTag("AREA", a);
    for(int i=0; i<4; i++){
        XML.pushTag("PT", i);
        XML.setValue("X", warpPuntos[i].x);
        XML.setValue("Y", warpPuntos[i].y);
        XML.popTag();
    }
    XML.popTag();
    if(a == 0){
        XML.setValue("THRESHOLD1", mainGui.getThreshold());
    }else if(a==1){
        XML.setValue("THRESHOLD2", mainGui.getThreshold());
    }else if(a==2){
        XML.setValue("THRESHOLD3", mainGui.getThreshold());
    }else if(a==3){
        XML.setValue("THRESHOLD4", mainGui.getThreshold());
    }else if(a==4){
        XML.setValue("THRESHOLD5", mainGui.getThreshold());
    }
    XML.saveFile("config.xml");
}

void ofApp::loadPresetArea(int a){
    ofVec2f *puntos = new ofVec2f[4];

    XML.pushTag("AREA", a);
    for(int j=0; j<4; j++){
        
        XML.pushTag("PT", j);
        puntos[j].set(XML.getValue("X", 0, 0), XML.getValue("Y", 0, 0));
        XML.popTag();
        
    }
    XML.popTag();
    /*
    if(a == 0){
        XML.getValue("THRESHOLD1", mainGui.getThreshold());
    }else if(a==1){
        XML.setValue("THRESHOLD2", mainGui.getThreshold());
    }else if(a==2){
        XML.setValue("THRESHOLD3", mainGui.getThreshold());
    }else if(a==3){
        XML.setValue("THRESHOLD4", mainGui.getThreshold());
    }else if(a==4){
        XML.setValue("THRESHOLD5", mainGui.getThreshold());
    }
*/
    mainGui.setWarpPoints(puntos[0], puntos[1], puntos[2], puntos[3]);

}

void ofApp::setupWarpAreas(){
    XML.loadFile("config.xml");

    XML.pushTag("AREAS");
    
//    numOfMicroasterismos = (int)XML.getNumTags("AREA");
    
    for(int i=0; i<numOfMicroasterismos; i++){

  //      maWarpArea area;
  //      ofVec2f *puntos = new ofVec2f[4];

        XML.pushTag("AREA", i);
        int numPuntos = XML.getNumTags("PT");
        
        for(int j=0; j<numPuntos; j++){
            
            XML.pushTag("PT", j);
    //        puntos[j].set(XML.getValue("X", 0, 0), XML.getValue("Y", 0, 0));
            XML.popTag();

        }
        
      //  area.setup(puntos[0], puntos[1], puntos[2], puntos[3]);
      //  warpAreas.push_back(area);
        
        XML.popTag();
    }
 
 
}
