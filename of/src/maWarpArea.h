//
//  maWarpArea.h
//  microasterismos
//
//  Created by Berio Molina on 20/06/14.
//
//

#ifndef __microasterismos__maWarpArea__
#define __microasterismos__maWarpArea__

#include "ofMain.h"

class maWarpArea : public ofBaseApp{

public:

    maWarpArea();

    void setup(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f p4);
    ofVec2f getPoints();
    
private:
  
    ofVec2f * points;

};

#endif /* defined(__microasterismos__maWarpArea__) */
