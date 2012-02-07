#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	
    
    imageDir = ofDirectory();
    imageDir.open("source/Brachman");
    currentImage = 0;
    
    cout << imageDir.listDir() << endl;    
    original.loadImage(imageDir.getPath(currentImage));


    numImages = 2;
    
    padding = 10;
    
    colorImg.allocate(original.width, original.height);
    colorImg.setFromPixels(original.getPixels(), original.width, original.height);
    
	grayImage.allocate(original.width, original.height);
    grayImage.setFromPixels(original.getPixels(), original.width, original.height);
    grayImage.updateTexture();
    
    blobSizeThreshold = 25000;
    needsSave = false;
}

//--------------------------------------------------------------
void testApp::update(){

    grayImage.threshold(80);
    contourFinder.findContours(grayImage, 50, (1275 * 1650)/3, 10, true);	// find holes
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    
    ofSetHexColor(0xffffff);
    ofFill();
    stringstream out;
    out << "max number of images: " << numImages << " (+/- to change)" << endl;
    out << "min size: " << minBlobSize << endl;
    out << "padding: " << padding << endl;

    out << "next page: 'n'" << endl;
    out << "previous page: 'p'" << endl;

    ofDrawBitmapString(out.str(), 20, 680);
    
    ofSetHexColor(0x00ff00);
 
    ofDrawBitmapString("Found Images (hit 's' to save them)", 520, 20);

    ofSetHexColor(0xffffff);

    ofScale(0.4, 0.4);

	original.draw(0,0);
    //grayImage.draw(original.width, 0);
    ofSetHexColor(0x00ff00);
    ofNoFill();
    ofSetLineWidth(3.0);
    
    int currentHeight = 100;
    
    minBlobSize = 500000;
    
    vector<int> imagesToSave;
    
    // check at most the number of
    // blobs that we actually found
    int numToCheck = numImages;
    if(contourFinder.nBlobs < numToCheck){
        numToCheck = contourFinder.nBlobs;
    }

    // blobs come in sorted by largest to smallest
    // so we look at the numToCheck largest
    for (int i = 0; i < numToCheck; i++){
        stringstream out;
        out << i;
        ofDrawBitmapString(out.str(), contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y);
        
        
        if(contourFinder.blobs[i].boundingRect.width * contourFinder.blobs[i].boundingRect.height < minBlobSize){
            minBlobSize = contourFinder.blobs[i].boundingRect.width * contourFinder.blobs[i].boundingRect.height;
        }
        
//        ofPushStyle();
//        ofSetColor(255,0,0);
//        ofFill();
//        ofEllipse(contourFinder.blobs[i].boundingRect.getCenter().x, contourFinder.blobs[i].boundingRect.getCenter().y, 10, 10);
//        ofPopStyle();
        
        if(contourFinder.blobs[i].boundingRect.width * contourFinder.blobs[i].boundingRect.height > blobSizeThreshold && !(i > 0 && contourFinder.blobs[0].boundingRect.inside(contourFinder.blobs[i].boundingRect.getCenter()))){
            imagesToSave.push_back(i);
                    
            ofPushMatrix();
            
                ofTranslate(original.width + 20, currentHeight);
                ofScale(1.0/0.4, 1/0.4);

                ofSetColor(255);
                drawCropped(original, contourFinder.blobs[i].boundingRect.x - padding/2, contourFinder.blobs[i].boundingRect.y - padding/2, contourFinder.blobs[i].boundingRect.width + padding, contourFinder.blobs[i].boundingRect.height + padding);
            
                         
            ofPopMatrix();
            
            currentHeight += contourFinder.blobs[i].boundingRect.height + 100;
                              
            ofSetColor(0, 255, 0);

            ofRect(contourFinder.blobs[i].boundingRect.x - padding/2, contourFinder.blobs[i].boundingRect.y - padding/2, contourFinder.blobs[i].boundingRect.width + padding, contourFinder.blobs[i].boundingRect.height + padding);
        }
    }
    
    
    if(needsSave){
        
        for(int j = 0; j < imagesToSave.size(); j++){
            int i = imagesToSave.at(j);
            
            ofImage temp;  
            temp.allocate(contourFinder.blobs[i].boundingRect.width,contourFinder.blobs[i].boundingRect.height, OF_IMAGE_COLOR);  
        
            temp.cropFrom(original, contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y, contourFinder.blobs[i].boundingRect.width,contourFinder.blobs[i].boundingRect.height);
        
            stringstream name;
            name << i;
            name << ".jpg";
            temp.saveImage(name.str());  
        }
        needsSave = false;

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


void testApp::refreshPage(){
    original.loadImage(imageDir.getPath(currentImage));
    grayImage.setFromPixels(original.getPixels(), original.width, original.height);
    grayImage.updateTexture();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){

		case '+':
            numImages++;
			//minBlobSize += 1000;
			break;
		case '-':
            numImages--;
            //minBlobSize -= 1000;
			break;
            
        case 's':
            needsSave = true;
            break;
        case 'n':
            currentImage++;
            refreshPage();
            break;
        case 'p':
            currentImage--;
            refreshPage();
            break;
        case '>':
            padding += 5;
            break;
        case '<':
            padding -= 5;
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
