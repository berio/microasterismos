//
//  maFindArea.cpp
//  fireflyMvFindObjects
//
//  Created by Berio Molina on 27/04/14.
//
//

#include "maFindArea.h"

maFindArea::maFindArea(){
    
}

void maFindArea::setup(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4, ofColor _color, bool _full){
    findAreaPoints  = new ofVec2f[4];
    findAreaPoints[0].set(p1);
    findAreaPoints[1].set(p2);
    findAreaPoints[2].set(p3);
    findAreaPoints[3].set(p4);
    
    insideRect  = new ofVec2f[4];
    insideRect[0].set(findAreaPoints[0].x + 96, findAreaPoints[0].y + 71);
    insideRect[1].set(findAreaPoints[1].x - 96, findAreaPoints[1].y + 71);
    insideRect[2].set(findAreaPoints[2].x - 96, findAreaPoints[2].y - 71);
    insideRect[3].set(findAreaPoints[3].x + 96, findAreaPoints[3].y - 71);
    
    color = _color;
    full = _full;
    isDraggingAreaPoint = false;
    isDraggingInsidePoint = false;
}

void maFindArea::draw(){
    ofFill();
	ofSetColor(color);
    
    for (int i=0; i<4; i++) {
        if (full){
            ofCircle(findAreaPoints[i].x, findAreaPoints[i].y, 8);
            ofCircle(insideRect[i].x, insideRect[i].y, 8);
        }else{
            if(i ==0 || i == 2){
                ofCircle(findAreaPoints[i].x, findAreaPoints[i].y, 8);
            }
        }
        
        if(i==3){
            ofLine(findAreaPoints[i].x, findAreaPoints[i].y, findAreaPoints[0].x, findAreaPoints[0].y);
        }else{
            ofLine(findAreaPoints[i].x, findAreaPoints[i].y, findAreaPoints[i+1].x, findAreaPoints[i+1].y);
        }
    }
    
    for (int i=0; i<4; i++) {
        if(full){
            if(i==3){
                ofLine(insideRect[i].x, insideRect[i].y, insideRect[0].x, insideRect[0].y);
            }else{
                ofLine(insideRect[i].x, insideRect[i].y, insideRect[i+1].x, insideRect[i+1].y);
            }
        }
    }
    
    ofVec2f * insideRect2  = new ofVec2f[4];
    insideRect2[0].set(findAreaPoints[0].x + 192, findAreaPoints[0].y + 145);
    insideRect2[1].set(findAreaPoints[1].x - 192, findAreaPoints[1].y + 145);
    insideRect2[2].set(findAreaPoints[2].x - 192, findAreaPoints[2].y - 145);
    insideRect2[3].set(findAreaPoints[3].x + 192, findAreaPoints[3].y - 145);
    
    for (int i=0; i<4; i++) {
        if(full){
            if(i==3){
                ofLine(insideRect2[i].x, insideRect2[i].y, insideRect2[0].x, insideRect2[0].y);
            }else{
                ofLine(insideRect2[i].x, insideRect2[i].y, insideRect2[i+1].x, insideRect2[i+1].y);
            }
        }
    }
    
    ofNoFill();
}

ofVec2f * maFindArea::getFindAreaPoints(){
    return findAreaPoints;
}

int maFindArea::getFindAreaDragedPoint(){
    return findAreaDragedPoint;
}

bool maFindArea::getIsDraggingAreaPoint(){
    return isDraggingAreaPoint;
}

void maFindArea::setFindAreaPoint(int idPoint, int x, int y){
    findAreaPoints[idPoint].x = x;
    findAreaPoints[idPoint].y = y;
}

void maFindArea::setPreset(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4){
    findAreaPoints[0].set(p1.x, p1.y);
    findAreaPoints[1].set(p2.x, p2.y);
    findAreaPoints[2].set(p3.x, p3.y);
    findAreaPoints[3].set(p4.x, p4.y);
    
    insideRect[0].set(findAreaPoints[0].x + 96, findAreaPoints[0].y + 71);
    insideRect[1].set(findAreaPoints[1].x - 96, findAreaPoints[1].y + 71);
    insideRect[2].set(findAreaPoints[2].x - 96, findAreaPoints[2].y - 71);
    insideRect[3].set(findAreaPoints[3].x + 96, findAreaPoints[3].y - 71);
    
}

void maFindArea::setFindAreaPointX(int idPoint, int x){
    findAreaPoints[idPoint].x = x;
}

void maFindArea::setFindAreaPointY(int idPoint, int y){
    findAreaPoints[idPoint].y = y;
}

//--------------------------------------------------------------
void maFindArea::mouseDragged(int x, int y){
    if(isDraggingAreaPoint){
        if(full){
            if(isDraggingInsidePoint){
                
                insideRect[findAreaDragedPoint].set(x, y);
                
                findAreaPoints[0].set(insideRect[0].x - 96, insideRect[0].y - 71);
                findAreaPoints[1].set(insideRect[1].x + 96, insideRect[1].y - 71);
                findAreaPoints[2].set(insideRect[2].x + 96, insideRect[2].y + 71);
                findAreaPoints[3].set(insideRect[3].x - 96, insideRect[3].y + 71);
            }else{
                
                findAreaPoints[findAreaDragedPoint].set(x, y);
                
                insideRect[0].set(findAreaPoints[0].x + 96, findAreaPoints[0].y + 71);
                insideRect[1].set(findAreaPoints[1].x - 96, findAreaPoints[1].y + 71);
                insideRect[2].set(findAreaPoints[2].x - 96, findAreaPoints[2].y - 71);
                insideRect[3].set(findAreaPoints[3].x + 96, findAreaPoints[3].y - 71);
            }
        }else{
            if(findAreaDragedPoint == 0){
                findAreaPoints[findAreaDragedPoint].x = x;
                findAreaPoints[findAreaDragedPoint].y = y;
                findAreaPoints[1].y = y;
                findAreaPoints[3].x = x;
                
            }else if(findAreaDragedPoint == 2){
                findAreaPoints[findAreaDragedPoint].x = x;
                findAreaPoints[findAreaDragedPoint].y = y;
                findAreaPoints[1].x = x;
                findAreaPoints[3].y = y;
            }
        }
    }
}

//--------------------------------------------------------------
void maFindArea::mousePressed(int x, int y){
    for(int i=0; i<4; i++){
        if(full){
            if(x<findAreaPoints[i].x + 10 &&
               x>findAreaPoints[i].x - 10 &&
               y<findAreaPoints[i].y + 10 &&
               y>findAreaPoints[i].y - 10){
                findAreaDragedPoint = i;
                isDraggingAreaPoint = true;
            }
            
            if(x<insideRect[i].x + 10 &&
               x>insideRect[i].x - 10 &&
               y<insideRect[i].y + 10 &&
               y>insideRect[i].y - 10){
                findAreaDragedPoint = i;
                isDraggingAreaPoint = true;
                isDraggingInsidePoint = true;
            }
        }else{
            if(i == 0 || i == 2){
                if(x<findAreaPoints[i].x + 10 &&
                   x>findAreaPoints[i].x - 10 &&
                   y<findAreaPoints[i].y + 10 &&
                   y>findAreaPoints[i].y - 10){
                    findAreaDragedPoint = i;
                    isDraggingAreaPoint = true;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void maFindArea::mouseReleased(int x, int y){
    if(isDraggingAreaPoint){
        
        if(isDraggingInsidePoint){
            
            insideRect[findAreaDragedPoint].set(x, y);
            
            findAreaPoints[0].set(insideRect[0].x - 96, insideRect[0].y - 71);
            findAreaPoints[1].set(insideRect[1].x + 96, insideRect[1].y - 71);
            findAreaPoints[2].set(insideRect[2].x + 96, insideRect[2].y + 71);
            findAreaPoints[3].set(insideRect[3].x - 96, insideRect[3].y + 71);
            
        }else{
            findAreaPoints[findAreaDragedPoint].x = x;
            findAreaPoints[findAreaDragedPoint].y = y;
            
            insideRect[0].set(findAreaPoints[0].x + 96, findAreaPoints[0].y + 71);
            insideRect[1].set(findAreaPoints[1].x - 96, findAreaPoints[1].y + 71);
            insideRect[2].set(findAreaPoints[2].x - 96, findAreaPoints[2].y - 71);
            insideRect[3].set(findAreaPoints[3].x + 96, findAreaPoints[3].y - 71);
        }
        isDraggingAreaPoint = false;
        isDraggingInsidePoint = false;
    }
}