#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofAppGLFWWindow win;
    win.setMultiDisplayFullscreen(true);    //this makes the fullscreen window span across all your monitors
    
	ofSetupOpenGL(&win,1024,768,OF_FULLSCREEN);		// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
    
}
