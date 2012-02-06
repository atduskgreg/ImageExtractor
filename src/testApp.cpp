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
    
    ofSetHexColor(0x00ff00);
 
    ofDrawBitmapString("Found Images", 520, 20);

    ofSetHexColor(0xffffff);

    ofScale(0.4, 0.4);

	original.draw(0,0);
    //grayImage.draw(original.width, 0);
    ofSetHexColor(0x00ff00);
    ofNoFill();
    ofSetLineWidth(3.0);
    
    int currentHeight = 100;
    
    
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        stringstream out;
        out << i;
        ofDrawBitmapString(out.str(), contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y);
        
        if(contourFinder.blobs[i].boundingRect.width * contourFinder.blobs[i].boundingRect.height > minBlobSize){

                    
            ofPushMatrix();
            
                ofTranslate(original.width + 20, currentHeight);
                ofScale(1.0/0.4, 1/0.4);

                ofSetColor(255);
                drawCropped(original, contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y, contourFinder.blobs[i].boundingRect.width, contourFinder.blobs[i].boundingRect.height);
                
            ofPopMatrix();
            
            currentHeight += contourFinder.blobs[i].boundingRect.height + 100;
                              
            ofSetColor(0, 255, 0);

            ofRect(contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y, contourFinder.blobs[i].boundingRect.width, contourFinder.blobs[i].boundingRect.height);
        }
    }

}

    void testApp::drawCropped(ofImage& img, int x, int y, int w, int h){
        ofVec2f nw(x, y);
        ofVec2f ne(nw.x + w, nw.y);
        ofVec2f se(ne.x, ne.y + h);
        ofVec2f sw(nw.x, nw.y + h);
        
        ofMesh subImage;
        
        subImage.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        
        subImage.addVertex(ofVec2f(0, 0));
        subImage.addTexCoord(nw);
        subImage.addVertex(ofVec3f(w, 0));
        subImage.addTexCoord(ne);
        subImage.addVertex(ofVec3f(w, h));
        subImage.addTexCoord(se);
        subImage.addVertex(ofVec3f(0, h));
        subImage.addTexCoord(sw);
        
        img.getTextureReference().bind();
        subImage.draw();
        img.getTextureReference().unbind();
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
