#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "DocumentWindow.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
        void drawCropped(ofImage& img, int x, int y, int w, int h);
    void refreshPage();
    void loadNewThesis();

    DocumentWindow scoreWindow(int x, int y);
    
    unsigned char * currentPixels;
    
    int windowSize;
    int crossingThreshold;
    int windowOffset;
    
    int scoreThreshold;
    
    bool imageLoaded;
    
    vector<DocumentWindow> windows;
    
    int currentImage;
    
    int imageCellSize;
    
    int hitWindowThreshold;
    
    int numPages;
    
    ofPoint currentMouse;

    int numImages;
    int padding;
    
    int blobSizeThreshold;
        ofDirectory imageDir;

        ofxCvColorImage			colorImg;

        ofxCvGrayscaleImage 	grayImage;

    int minBlobSize;

        ofxCvContourFinder 	contourFinder;
        ofImage original;

    bool needsSave;
    
    ofDirectory saveTarget;


};

