#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	
    original.loadImage("example_page1.jpg");
    
    colorImg.allocate(original.width, original.height);
    colorImg.setFromPixels(original.getPixels(), original.width, original.height);
    
	grayImage.allocate(original.width, original.height);
    grayImage.setFromPixels(original.getPixels(), original.width, original.height);
    grayImage.updateTexture();
    
    minBlobSize = 6000;
}

//--------------------------------------------------------------
void testApp::update(){

    

    /*
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
     */	
    
    grayImage.threshold(80);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 50, (1275 * 1650)/3, 10, true);	// find holes
    

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    
    ofSetHexColor(0xffffff);
    ofFill();
    stringstream out;
    out << "minimum image size: " << minBlobSize << " (+/- to change)" << endl;
    ofDrawBitmapString(out.str(), 20, 700);
    
    ofSetHexColor(0xffffff);
    ofScale(0.4, 0.4);

	original.draw(0,0);
    grayImage.draw(original.width, 0);
    ofSetHexColor(0x00ff00);
    ofNoFill();
    ofSetLineWidth(3.0);
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        stringstream out;
        out << i;
        ofDrawBitmapString(out.str(), contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y);
        
        if(contourFinder.blobs[i].boundingRect.width * contourFinder.blobs[i].boundingRect.height > minBlobSize){
            cout << "blob " << i << " x/y: " << contourFinder.blobs[i].boundingRect.x  << "/" << contourFinder.blobs[i].boundingRect.y << " size: " << contourFinder.blobs[i].boundingRect.width * contourFinder.blobs[i].boundingRect.height << endl;
        
            ofRect(contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y, contourFinder.blobs[i].boundingRect.width, contourFinder.blobs[i].boundingRect.height);
        }
    }
 

    
    
/*
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(1275,20);
	grayBg.draw(20,1650);
	grayDiff.draw(1275, 1650);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    

	// finally, a report:

	
 */

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){

		case '+':
			minBlobSize += 1000;
			break;
		case '-':
            minBlobSize -= 1000;
			break;
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
