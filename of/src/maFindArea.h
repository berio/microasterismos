//
//  maFindArea.h
//  microasterismos
//
//  Created by Berio Molina on 09/06/14.
//
//

#ifndef __microasterismos__maFindArea__
#define __microasterismos__maFindArea__

#include "ofMain.h"

class maFindArea : public ofBaseApp{
    
public:
    
    maFindArea();
    
    void setup(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4, ofColor _color, bool _full);
    void draw();
    
    ofVec2f * getFindAreaPoints();
    int getFindAreaDragedPoint();
    bool getIsDraggingAreaPoint();
    void setPreset(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4);
    void setFindAreaPoint(int idPoint, int x, int y);
    void setFindAreaPointX(int idPoint, int x);
    void setFindAreaPointY(int idPoint, int y);
    
    void mouseDragged(int x, int y);
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y);
    
    
private:
    
    ofVec2f * findAreaPoints;
    ofVec2f * insideRect;
    int findAreaDragedPoint;
    ofColor color;
    bool full;
    bool isDraggingAreaPoint;
    bool isDraggingInsidePoint;
    
};

#endif /* defined(__microasterismos__maFindArea__) */
