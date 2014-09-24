//
//  maWarpArea.cpp
//  microasterismos
//
//  Created by Berio Molina on 20/06/14.
//
//

#include "maWarpArea.h"

maWarpArea::maWarpArea(){
}

void maWarpArea::setup(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4){
    
    points  = new ofVec2f[4];
    points[0].set(p1);
    points[1].set(p2);
    points[2].set(p3);
    points[3].set(p4);

}

ofVec2f maWarpArea::getPoints(){
    return *points;
}
