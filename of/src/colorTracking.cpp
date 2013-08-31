//
//  colorTracking.cpp
//  colorTrackingGuiXml
//
//  Created by berio molina on 06/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "colorTracking.h"

//------------------------------------------------------------------
colorTracking::colorTracking(){
    
}

void colorTracking::setup(){

    w = 320;
    h = 240;

    verHsb = verSat = verHue = false;
    verInput = true;
    isDraggingWarperPoint = false;

#ifdef _USE_LIVE_VIDEO
	movie.setVerbose(true);
	movie.setDeviceID(1);
	movie.setDesiredFrameRate(60);
    movie.initGrabber(w, h);
#else
    movie.loadMovie("fingers.mov");
    movie.play();
#endif

    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    grayImg.allocate(w,h);
    warpedGrayImg.allocate(w,h);
    filtered.allocate(w, h);
    warpedFiltered.allocate(w,h);

    IP.setup();
}

void colorTracking::setupHsbValues(int _h, int _s, int _b, int _ah, int _as, int _ab){
    findHue = _h;
    findSat = _s; 
    findBri = _b;
    anchoHue = _ah;
    anchoSat = _as;
    anchoBri = _ab;
}

void colorTracking::setupConfigValues(int _radioCircle, int _perimetroBlobMin, int _perimetroBlobMax){
    marginCircles = _radioCircle;
    perimetroBlobMin = _perimetroBlobMin;
    perimetroBlobMax = _perimetroBlobMax;
}

void colorTracking::setupWarpValues(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int _x4, int _y4){
    srcWarperPositions  = new ofPoint[4];
	srcWarperPositions[0].set(_x1, _y1);
	srcWarperPositions[1].set(_x2, _y2);
	srcWarperPositions[2].set(_x3, _y3);
	srcWarperPositions[3].set(_x4, _y4);
    
	dstWarperPositions  = new ofPoint[4];
	dstWarperPositions[0].set(0, 0);
	dstWarperPositions[1].set(w, 0);
	dstWarperPositions[2].set(w, h);
	dstWarperPositions[3].set(0, h);    
}

int * colorTracking::getHsbValues(){
    int * hsbValues  = new int[6];
    hsbValues[0] = findHue;
    hsbValues[1] = findSat;
    hsbValues[2] = findBri;
    hsbValues[3] = anchoHue;
    hsbValues[4] = anchoSat;
    hsbValues[5] = anchoBri;
    return hsbValues;
}

int * colorTracking::getConfigValues(){
    int * configValues  = new int[3];
    configValues[0] = marginCircles;
    configValues[1] = perimetroBlobMin;
    configValues[2] = perimetroBlobMax;
    return configValues;
}

int * colorTracking::getWarpValues(){
    int * warpValues  = new int[8];
    warpValues[0] = srcWarperPositions[0].x;
    warpValues[1] = srcWarperPositions[0].y;
    warpValues[2] = srcWarperPositions[1].x;
    warpValues[3] = srcWarperPositions[1].y;
    warpValues[4] = srcWarperPositions[2].x;
    warpValues[5] = srcWarperPositions[2].y;
    warpValues[6] = srcWarperPositions[3].x;
    warpValues[7] = srcWarperPositions[3].y;
    return warpValues;
}

void colorTracking::update(){

    bool bNewFrame = false;

    movie.update();
    bNewFrame = movie.isFrameNew();

	if (bNewFrame){
    
    //copy webcam pixels to rgb image and greyImage
    rgb.setFromPixels(movie.getPixels(), w, h);
    grayImg.setFromColorImage(rgb);
    warpedGrayImg.warpIntoMe(grayImg, srcWarperPositions, dstWarperPositions);
        
    //duplicate rgb
    hsb = rgb;
        
    //convert to hsb
    hsb.convertRgbToHsv();
        
    //store the three channels as grayscale images
    hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
    //filter image based on the hue value were looking for
    for (int i=0; i<w*h; i++) {
        if(ofInRange(hue.getPixels()[i],findHue-anchoHue,findHue+anchoHue) && 
           ofInRange(sat.getPixels()[i],findSat-anchoSat,findSat+anchoSat) && 
           ofInRange(bri.getPixels()[i],findBri-anchoBri,findBri+anchoBri)){
            filtered.getPixels()[i] =255;
        }else{
            filtered.getPixels()[i] =0;
        }
    }

    filtered.dilate();
    filtered.dilate();
    filtered.dilate();
    filtered.flagImageChanged();
    warpedFiltered.warpIntoMe(filtered, srcWarperPositions, dstWarperPositions);
        
    //run the contour finder on the filtered image to find blobs with a certain hue
    contours.findContours(warpedFiltered, 50, w*h/2, 10, false);
        
    IP.reset();
    printf("Comezan os blobs \n");
    for (int i=0; i<contours.nBlobs; i++) {
        
        int perimetro = contours.blobs[i].length;
        printf("length %i \n", perimetro);

        if(perimetro > perimetroBlobMin && perimetro < perimetroBlobMax){
        }
        vector <ofPoint> blobPts = contours.blobs[i].pts;
        /* Saber si o blob e un CIRCULO
        Recorremos todos os puntos do contorno do blob, para saber a que distancia estan
        do centro do blob. Si todos os puntos estan mais ou menos a mesma distancia, enton
        estamos ante un circulo */
        float margenMinima = 10.0;
        float minimo = 9000.0;
        float maximo = 0.0;
        for(int j=0; j<contours.blobs[i].nPts; j++){
            float distanciaAlCentro = ofDist(blobPts[j].x, blobPts[j].y, contours.blobs[i].centroid.x, contours.blobs[i].centroid.y);
            //printf("punto %i, distancia al centro %f \n", j, distanciaAlCentro);
            minimo = fmin(minimo, distanciaAlCentro);
            maximo = fmax(maximo, distanciaAlCentro);
            //printf("minimo %f \n", minimo);
            //printf("maximo %f \n", maximo);
        }
        float margen = maximo - minimo;
        
        if(margen < margenMinima){
            //printf("blob n %i \n", i);
            IP.update(contours.blobs[i].centroid, contours.blobs[i].boundingRect);
            //printf("numero de puntos do contorno do blob %i \n", contours.blobs[i].nPts);
        }
    }
    IP.updateLines();
    }
}

void colorTracking::draw(){

    ofSetColor(255,255,255);
    
    //draw all cv images
    if(verInput) rgb.draw(0,0);
    if(verHsb) hsb.draw(0,0);
    if(verHue) hue.draw(0,0);
    if(verSat) sat.draw(0,0);
    warpedGrayImg.draw(w,0);
    warpedFiltered.draw(w*2,0);
    contours.draw(w*2,0);
    
    ofSetColor(255, 0, 0);
    ofFill();
    
    //draw red circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        //ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
    }    
    
    //draw gui
	ofSetHexColor(0xBBBBBB);	
    int yOffset = h + 80;
    int margen = 20;
	ofDrawBitmapString("HSV VALUES: ", 40, yOffset+margen);
	ofSetHexColor(0x777777);	
	ofDrawBitmapString("Hue: " + ofToString(findHue) + " (a, z)", 40, yOffset+margen*2);
	ofDrawBitmapString("Sat: " + ofToString(findSat) + " (s, x)", 40, yOffset+margen*3);
	ofDrawBitmapString("Bri: " + ofToString(findBri) + " (d, c)", 40, yOffset+margen*4);
	ofDrawBitmapString("Hue Ancho: " + ofToString(anchoHue) + " (f, v)", 40, yOffset+margen*5);
	ofDrawBitmapString("Sat Ancho: " + ofToString(anchoSat) + " (g, b)", 40, yOffset+margen*6);
	ofDrawBitmapString("Bri Ancho: " + ofToString(anchoBri) + " (h, n)", 40, yOffset+margen*7);

    ofSetHexColor(0xBBBBBB);
	ofDrawBitmapString("CONFIG VALUES: ", 40, yOffset+margen*9);
	ofSetHexColor(0x777777);
	ofDrawBitmapString("Margin detect circles: " + ofToString(marginCircles) + " (j, m)", 40, yOffset+margen*10);
	ofDrawBitmapString("Minimum blob length: " + ofToString(perimetroBlobMin) + " (k, ,)", 40, yOffset+margen*11);
	ofDrawBitmapString("Margin detect circles: " + ofToString(perimetroBlobMax) + " (l, .)", 40, yOffset+margen*12);

    ofSetHexColor(0xBBBBBB);
	ofDrawBitmapString("KEYS: ", 40, yOffset+margen*14);
	ofSetHexColor(0x777777);	
	ofDrawBitmapString("\"i\" -> Fullscreen ", 40, yOffset+margen*15);
	ofDrawBitmapString("\"1\" / \"2\" / \"3\" -> Switch samples library", 40, yOffset+margen*16);
	ofDrawBitmapString("\"l\" -> Save warp values ", 40, yOffset+margen*17);
	ofDrawBitmapString("\"o\" -> Save HSV values ", 40, yOffset+margen*18);
	ofDrawBitmapString("\"k\" -> Save Config values ", 40, yOffset+margen*19);

    IP.draw();
}

void colorTracking::drawWarpIn(){
    
	// Draw lines between points
	ofSetColor(223, 223, 223);
	ofNoFill();
	ofBeginShape();
	for (int j = 0; j < 4; j++){
		ofVertex(srcWarperPositions[j].x, srcWarperPositions[j].y);
	}
	ofEndShape(true);
    
    // Draw interior rectangle
    ofBeginShape();
    float ancho1 = srcWarperPositions[1].x - srcWarperPositions[0].x;
    float ancho2 = srcWarperPositions[2].x - srcWarperPositions[3].x;
    float alto1 = srcWarperPositions[3].y - srcWarperPositions[0].y;
    float alto2 = srcWarperPositions[2].y - srcWarperPositions[1].y;
    ofVertex(srcWarperPositions[0].x + (ancho1*0.25), srcWarperPositions[0].y + (alto1*0.25));
    ofVertex(srcWarperPositions[1].x - (ancho1*0.25), srcWarperPositions[1].y + (alto2*0.25));
    ofVertex(srcWarperPositions[2].x - (ancho2*0.25), srcWarperPositions[2].y - (alto2*0.25));
    ofVertex(srcWarperPositions[3].x + (ancho2*0.25), srcWarperPositions[3].y - (alto1*0.25));
    ofEndShape(true);
    
	// Draw points
	ofFill();
	ofSetColor(33, 216, 255);
	for (int j = 0; j < 4; j++){
		ofCircle(srcWarperPositions[j].x, srcWarperPositions[j].y, 5);
	}
	ofNoFill();
}

//--------------------------------------------------------------
void colorTracking::mouseDragged(int x, int y, int button){
    if(isDraggingWarperPoint){
        srcWarperPositions[warperDraggingPoint].x = x;
        srcWarperPositions[warperDraggingPoint].y = y;
    }
}

//--------------------------------------------------------------
void colorTracking::mousePressed(int x, int y, int button){
    for(int i=0; i<4; i++){
        if(x<srcWarperPositions[i].x + 10 && 
           x>srcWarperPositions[i].x - 10 &&
           y<srcWarperPositions[i].y + 10 &&
           y>srcWarperPositions[i].y - 10){
            warperDraggingPoint = i;
            isDraggingWarperPoint = true;
        }
    }
    if(x<w && y<h && isDraggingWarperPoint==false){
        //get hue value on mouse position
        findHue = hue.getPixels()[y*w+x];
        findSat = sat.getPixels()[y*w+x];
        findBri = bri.getPixels()[y*w+x];
    }    
}

//--------------------------------------------------------------
void colorTracking::mouseReleased(int x, int y, int button){
    if(isDraggingWarperPoint){
        isDraggingWarperPoint = false;
    }
}

//--------------------------------------------------------------
void colorTracking::keyPressed(int key){
    IP.keyPressed(key);
	switch(key){
		case 'a':
			findHue = min(findHue + 1, 180);
			break;
		case 'z':
			findHue = max(findHue - 1, 0);
			break;
		case 's':
			findSat = min(findSat + 1, 250);
			break;
		case 'x':
			findSat = max(findSat - 1, 0);
			break;
		case 'd':
			findBri = min(findBri + 1, 250);
			break;
		case 'c':
			findBri = max(findBri - 1, 0);
			break;
		case 'f':
			anchoHue = min(anchoHue + 1, 180);
			break;
		case 'v':
			anchoHue = max(anchoHue - 1, 0);
			break;
		case 'g':
			anchoSat = min(anchoSat + 1, 100);
			break;
		case 'b':
			anchoSat = max(anchoSat - 1, 0);
			break;
		case 'h':
			anchoBri = min(anchoBri + 1, 100);
			break;
		case 'n':
			anchoBri = max(anchoBri - 1, 0);
			break;
        // Margin Circles
		case 'j':
			marginCircles = min(marginCircles + 1, 500);
			break;
		case 'm':
			marginCircles = max(marginCircles - 1, 0);
			break;
        // Minimo del perimetro del blob
		case 'k':
			perimetroBlobMin = min(perimetroBlobMin + 1, 1000);
			break;
		case ',':
			perimetroBlobMin = max(perimetroBlobMin - 1, 0);
			break;
        // Maximo del perimetro del blob
		case 'l':
			perimetroBlobMax = min(perimetroBlobMax + 1, 1000);
			break;
		case '.':
			perimetroBlobMax = max(perimetroBlobMax - 1, 0);
			break;
        #ifdef _USE_LIVE_VIDEO
		case 'p':
			movie.videoSettings();
			break;
        #endif
	}    
}

