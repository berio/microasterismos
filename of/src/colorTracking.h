#ifndef _COLOR_TRACKING_H
#define _COLOR_TRACKING_H

#include "ofxOpenCv.h"
#include "ofMain.h"
#include "imageProjection.h"
#include "atopaDebuxos.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera

class colorTracking : public ofBaseApp{
    
public:
    
    colorTracking();
    
    void setup();
    void setupHsbValues(int findHue, int findSat, int findBri, int anchoHue, int anchoSat, int anchoBri);
    void setupWarpValues(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int _x4, int _y4);
    void setupConfigValues(int _radioCircle, int _perimetroBlobMin, int _perimetroBlobMax);
    int * getHsbValues();
    int * getConfigValues();
    int * getWarpValues();
    void update();
    void draw();
    void drawWarpIn();
    
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    #ifdef _USE_LIVE_VIDEO
        ofVideoGrabber  movie;
    #else
        ofVideoPlayer   movie;
    #endif
    
    ofxCvColorImage rgb,hsb;
    ofxCvGrayscaleImage hue,sat,bri,filtered, grayImg, warpedGrayImg, warpedFiltered;    
    atopaDebuxos contours;
    
    imageProjection IP;
    
    ofPoint * srcWarperPositions;
    ofPoint * dstWarperPositions;
    int warperDraggingPoint;
    bool isDraggingWarperPoint;

    int w,h;
    int findHue, findSat, findBri, anchoHue, anchoSat, anchoBri;

    //GUI
    bool verHsb, verSat, verHue, verInput;

    int marginCircles, perimetroBlobMin, perimetroBlobMax;
};
#endif
