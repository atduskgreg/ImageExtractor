#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	
    
   /* imageDir = ofDirectory();
    imageDir.open("source/Brachman");
    currentImage = 0;
    
    cout << imageDir.listDir() << endl;    
    original.loadImage(imageDir.getPath(currentImage));

*/
    
    imageLoaded = false;
    currentMouse = ofPoint(0,0);

    loadNewThesis();
    
    
    imageCellSize = 70;
    hitWindowThreshold = 20;
    
    windowSize = 25;
    crossingThreshold = 0;
    windowOffset = 35;
    
    scoreThreshold = 280;
    ofEnableAlphaBlending(); 
    
    numImages = 2;
    
    padding = 10;
    
    /*
    colorImg.allocate(original.width, original.height);
    colorImg.setFromPixels(original.getPixels(), original.width, original.height);
    
	grayImage = colorImg;
    grayImage.updateTexture();
    cout << "about to get pixels" << endl;
    currentPixels = grayImage.getPixels();
    */
     imageLoaded = true;

    blobSizeThreshold = 25000;
    needsSave = false;
    
    cout << "finish setup" << endl;

}

void testApp::loadNewThesis(){
    ofFileDialogResult result = ofSystemLoadDialog("Please select a folder of images", true);
    imageDir = ofDirectory();
    imageDir.open(result.getPath());
    currentImage = 0;
    numPages = imageDir.listDir();
    
    cout << "list directory and load image" << endl;
    
    cout << imageDir.listDir() << endl;    
    original.loadImage(imageDir.getPath(currentImage));
    original.setImageType(OF_IMAGE_GRAYSCALE);

    cout << "original w: " << original.width << " h: " << original.height << endl;
    
    
    stringstream saveDirName;
    saveDirName << result.getPath() << "/" << "results";
    saveTarget = ofDirectory(saveDirName.str());
    if(!saveTarget.exists()){
        cout << saveDirName.str() << " doesn't exist. Creating." << endl;
        saveTarget.create();
    } else {
        cout << saveDirName.str() << " exists." << endl;
    }
        
}

//--------------------------------------------------------------
void testApp::update(){

    
    windows.clear();
    
    for(int y = 0; y < original.height; y+= windowOffset ){
    
        for(int x = 0; x < original.width; x+= windowOffset ){
            windows.push_back( scoreWindow(x, y) );

        }
    }
}

DocumentWindow testApp::scoreWindow(int x, int y){
    
    int score = 0;
    
    currentPixels = original.getPixels();
    
    int windowCrossingThreshold = 0;
        
    for(int i = 0; i < windowSize; i++){
        int j = (x + i) + (y + i)*original.width;
            windowCrossingThreshold += currentPixels[j];
    }
    
    windowCrossingThreshold /= windowSize;
    
    windowCrossingThreshold += crossingThreshold;
    
    
    // horizontal pass
    for(int row = 0; row< windowSize; row++){
        for(int col = 0; col< windowSize-1; col++){

            //int i = p + col + (windowSize * row);
            int i = (x + col) + (y + row)*original.width;
            
                // crossing downwards
                if((currentPixels[i] > windowCrossingThreshold) && 
                   (currentPixels[i+1] <= windowCrossingThreshold)){
                    
                    score++;
                }
                
                // crossing upwards
                if((currentPixels[i] < windowCrossingThreshold) && 
                   (currentPixels[i+1] >= windowCrossingThreshold)){
                    
                    score++;
                }
        }
    }
    
    // vertical pass
    for(int row = 0; row< windowSize-1; row++){
        for(int col = 0; col< windowSize; col++){
            
            int j = (x + col) + (y + row)*original.width;
                            
                // crossing downwards
                if((currentPixels[j] > windowCrossingThreshold) && 
                   (currentPixels[j+windowSize] <= windowCrossingThreshold)){
                    score++;
                }
                
                // crossing upwards
                if((currentPixels[j] < windowCrossingThreshold) && 
                   (currentPixels[j+windowSize] >= windowCrossingThreshold)){
                    score++;
                }
            
            
        }
    }
    
    return DocumentWindow(x - windowSize/2,y -windowSize/2,score);
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    
    ofSetHexColor(0xffffff);
    ofFill();
    stringstream out;
    out << "scoreThreshold: " << scoreThreshold << endl;
    out << "crossingThreshold: " << crossingThreshold << endl;

    out << "next page: 'n'" << endl;
    out << "previous page: 'p'" << endl;

    ofDrawBitmapString(out.str(), 520, 680);
    
    ofSetHexColor(0x00ff00);
 
    ofDrawBitmapString("Found Images (hit 's' to save them)", 520, 20);

    ofSetHexColor(0xffffff);

    ofPushMatrix();
    ofScale(0.4, 0.4);

	//original.draw(0,0);
    if(imageLoaded){
    //    grayImage.draw(0, 0);
        
        original.draw(0,0);
    }
    int currentHeight = 100;
    
    minBlobSize = 500000;
    
    vector<int> imagesToSave;
    

    

    vector<DocumentWindow> hitWindows;
    
    for(int i = 0; i < windows.size(); i++ ){
        
            DocumentWindow window = windows.at(i);
        
                    
            if(window.score > scoreThreshold){
                float a = ofMap(window.score, 0, scoreThreshold * 2, 0, 255);

                ofSetColor(255,0,0, a);
                ofRect(window.x,window.y,windowSize, windowSize);
                
                hitWindows.push_back(window);
            }
    }
    
    /*int lowX = 0;
    int lowY = 0;
    int highX = original.width;
    int highY = original.height;

    for(int i = 0; i < hitWindows.size(); i++){
        DocumentWindow w = hitWindows.at(i);
        
        if(w.x > lowX){
            lowX = w.x;
        }
        
        if(w.y > lowY){
            lowY = w.y;
        }
        
        if(w.x < highX){
            highX = w.x;
        }
        
        if(w.y < highY){
            highY = w.y;
        }
    }
    
    ofSetColor(0, 255, 0);
    ofNoFill();
    ofRect(lowX, lowY, highX, highY);
     */
    
    
    for(int y = 0; y < original.width; y += imageCellSize){
        
        for(int x = 0;x < original.width; x += imageCellSize){
            
            int cellCount = 0;
            for(int i = 0; i < hitWindows.size(); i++){
                DocumentWindow w = hitWindows.at(i);
                if(w.x > x && w.y > y && w.x < x + imageCellSize && w.y < y + imageCellSize ){
                    
                    cellCount++;
                }
                if(cellCount > hitWindowThreshold){
                    ofSetColor(0,255,0, 50);
                    ofRect(x, y, imageCellSize, imageCellSize);
                }
                
                
            }
        }
        
    }
    
    
    if(imageLoaded){
    
        DocumentWindow w = scoreWindow(currentMouse.x * 1/0.4, currentMouse.y * 1/0.4);

                
        ofPopMatrix();

        ofSetColor(255, 0, 0);
        stringstream s;
        s << "Score under mouse " << w.score << endl;
        s << "hit window threshold " << hitWindowThreshold << endl;

        cout << s.str();
        
        ofDrawBitmapString(s.str(), 520, 50);

    }
    
  
    
    
       
    //cout << " highest score: " << highestScore << " lowestScore: " << lowestScore << endl;

    
    if(needsSave){
        
        for(int j = 0; j < imagesToSave.size(); j++){
            int i = imagesToSave.at(j);
            
            ofImage temp;  
            temp.allocate(contourFinder.blobs[i].boundingRect.width,contourFinder.blobs[i].boundingRect.height, OF_IMAGE_COLOR);  
        
            temp.cropFrom(original, contourFinder.blobs[i].boundingRect.x - padding/2, contourFinder.blobs[i].boundingRect.y - padding/2, contourFinder.blobs[i].boundingRect.width + padding,contourFinder.blobs[i].boundingRect.height + padding);
        
            
            // TODO:
            // -fix this to make and save in a results folder in the current image dir
            stringstream name;
            name << saveTarget.path() << "/" << currentImage << "_" << i;
            name << ".jpg";
            cout << "saving to: " << name.str() << endl;
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
    if(currentImage < 0){
        currentImage = numPages - 1;
    }
    
    if(currentImage >= numPages){
        currentImage = 0;
    }
    
    cout << "currentImage: " << currentImage << endl;
    
    
    original.loadImage(imageDir.getPath(currentImage));
    original.setImageType(OF_IMAGE_GRAYSCALE);
    //grayImage.setFromPixels(original.getPixels(), original.width, original.height);
   // grayImage.updateTexture();
    //currentPixels = grayImage.getPixels();
    cout << "reloaded pixels" << endl;

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){

		case '=':
            //numImages++;
			//minBlobSize += 1000;
            scoreThreshold += 10;
            cout << "score threshold " << scoreThreshold << endl;
			break;
		case '-':
            //numImages--;
            //minBlobSize -= 1000;
            scoreThreshold -= 10;
            cout << "score threshold " << scoreThreshold << endl;

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
            crossingThreshold += 5;
            cout << "crossingThreshold " << crossingThreshold << endl;
            break;
        case '<':
            crossingThreshold -= 5;
            cout << "crossingThreshold " << crossingThreshold << endl;

            break;
        case 'l':
            loadNewThesis();
            break;
        case '1':
            hitWindowThreshold -= 1;
            break;
        case '2':
            hitWindowThreshold += 1;
            break;
	}
 
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    currentMouse.x = x;
    currentMouse.y = y;
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
