#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // General app settings
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(40);
    
    // Initialize the visualization toggle
    visualizeFloatData = false;
    
    // Set up the FBOs
    ofFboSettings standardSettings;
    standardSettings.width = 512;
    standardSettings.height = 512;
    standardSettings.internalformat = GL_RGBA;  // Standard byte-based format
    standardFbo.allocate(standardSettings);
    
    ofFboSettings floatSettings;
    floatSettings.width = 512;
    floatSettings.height = 512;
    floatSettings.internalformat = GL_RGBA32F;  // Floating-point format
    floatFbo.allocate(floatSettings);
    
    // Load shader
    shader.load("shaders/pboExample");
    
    // Initialize both FBOs with some content
    standardFbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    ofDrawCircle(standardFbo.getWidth()/2, standardFbo.getHeight()/2, 100);
    standardFbo.end();
    
    floatFbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    ofDrawCircle(floatFbo.getWidth()/2, floatFbo.getHeight()/2, 100);
    floatFbo.end();
    
    // Set up the PBOs for reading back pixel data from each FBO
    standardPbo.setup(&standardFbo.getTexture());
    floatPbo.setup(&floatFbo.getTexture());
    
    // Print information about the initialized PBOs
    ofLogNotice() << "Standard PBO setup complete. Is valid: " << standardPbo.isValid();
    ofLogNotice() << "Float PBO setup complete. Is valid: " << floatPbo.isValid();
    
    ofSetWindowTitle("ofxSinglePbo Example - Press 'v' to toggle float data visualization");
}

//--------------------------------------------------------------
void ofApp::update() {
    
    // Update both PBOs to get the latest pixel data from the FBOs
    standardPbo.update();
    floatPbo.update();
    
    // Debug: Print a pixel value from each PBO
    if (ofGetFrameNum() % 60 == 0) { // Once per second
        ofColor standardColor = standardPbo.getPixels().getColor(standardFbo.getWidth()/2, standardFbo.getHeight()/2);
        ofFloatColor floatColor = floatPbo.getFloatPixels().getColor(floatFbo.getWidth()/2, floatFbo.getHeight()/2);
        
        ofLogNotice() << "Center pixel from standard FBO: " << standardColor;
        ofLogNotice() << "Center pixel from float FBO: " << floatColor;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);

    // Update shader parameters for rendering
    float time = ofGetElapsedTimef();
    
    // Update standard FBO with animated content
    standardFbo.begin();
    ofClear(0, 0, 0, 255);
    shader.begin();
    shader.setUniform1f("time", time);
    shader.setUniform1f("useFloats", 0.0); // Use 0-255 range values
    shader.setUniform2f("resolution", standardFbo.getWidth(), standardFbo.getHeight());
    ofDrawRectangle(0, 0, standardFbo.getWidth(), standardFbo.getHeight());
    shader.end();
    standardFbo.end();
    
    // Update float FBO with the same animation but in floating point (0-1 range)
    floatFbo.begin();
    ofClear(0, 0, 0, 255);
    shader.begin();
    shader.setUniform1f("time", time);
    shader.setUniform1f("useFloats", 1.0); // Use 0-1 range values
    shader.setUniform2f("resolution", floatFbo.getWidth(), floatFbo.getHeight());
    ofDrawRectangle(0, 0, floatFbo.getWidth(), floatFbo.getHeight());
    shader.end();
    floatFbo.end();
    
    
    // Draw the standard FBO on the left
    standardFbo.draw(10, 10, 400, 400);
    ofDrawBitmapStringHighlight("Standard FBO (GL_RGBA)", 10, 430);
    
    // Draw the float FBO on the right
    floatFbo.draw(420, 10, 400, 400);
    ofDrawBitmapStringHighlight("Float FBO (GL_RGBA32F)", 420, 430);
    
    // If enabled, visualize the float data more clearly
    if (visualizeFloatData) {
        ofFloatPixels& floatPixels = floatPbo.getFloatPixels();
        
        // Draw the red channel as a grayscale image
        ofImage redChannel;
        
        // Extract the red channel from float pixels
        ofFloatPixels redPixels;
        redPixels.allocate(floatPixels.getWidth(), floatPixels.getHeight(), OF_IMAGE_GRAYSCALE);
        
        for (int y = 0; y < floatPixels.getHeight(); y++) {
            for (int x = 0; x < floatPixels.getWidth(); x++) {
                float redValue = floatPixels.getColor(x, y).r;
                redPixels.setColor(x, y, ofFloatColor(redValue));
            }
        }
        
        redChannel.allocate(redPixels.getWidth(), redPixels.getHeight(), OF_IMAGE_GRAYSCALE);
        redChannel.setFromPixels(redPixels);
        redChannel.update();
        
        // Draw the red channel
        redChannel.draw(10, 450, 400, 400);
        ofDrawBitmapStringHighlight("Float FBO Red Channel Visualization", 10, 870);
        
        // Draw a gradient bar to show float range
        ofSetColor(255);
        ofDrawRectangle(420, 450, 400, 30);
        for (int x = 0; x < 400; x++) {
            float val = x / 400.0f;
            ofSetColor(val * 255);
            ofDrawRectangle(420 + x, 480, 1, 30);
        }
        ofSetColor(255);
        ofDrawBitmapStringHighlight("Float Value Range: 0.0", 420, 530);
        ofDrawBitmapStringHighlight("1.0", 800, 530);
    }
    
    // Show instructions
    ofSetColor(255);
    string instructions = "Press 'v' to toggle float data visualization\n"
                          "Using ofxSinglePbo to read back pixel data from standard and float FBOs";
    ofDrawBitmapStringHighlight(instructions, 10, ofGetHeight() - 40);
    
    // Display FPS
    ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate(), 1), ofGetWidth() - 120, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'v' || key == 'V') {
        visualizeFloatData = !visualizeFloatData;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}