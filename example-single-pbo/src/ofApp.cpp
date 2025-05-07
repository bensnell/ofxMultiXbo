#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // General app settings
    ofSetVerticalSync(false);
    ofSetFrameRate(1000);
    ofBackground(40);

    // Set up the FBOs
    ofFboSettings standardSettings;
    standardSettings.width = 512;
    standardSettings.height = 512;
    standardSettings.internalformat = GL_RGBA; // Standard byte-based format
    standardFbo.allocate(standardSettings);

    ofFboSettings floatSettings;
    floatSettings.width = 512;
    floatSettings.height = 512;
    floatSettings.internalformat = GL_RGBA32F; // Floating-point format
    floatFbo.allocate(floatSettings);

    // Load shader
    shader.load("shaders/pboExample");

    // Initialize both FBOs with some content
    standardFbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    ofDrawCircle(standardFbo.getWidth() / 2, standardFbo.getHeight() / 2, 100);
    standardFbo.end();

    floatFbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    ofDrawCircle(floatFbo.getWidth() / 2, floatFbo.getHeight() / 2, 100);
    floatFbo.end();

    // Set up the PBOs for reading back pixel data from each FBO
    standardPbo.setup(&standardFbo.getTexture());
    floatPbo.setup(&floatFbo.getTexture());

    standardImage.allocate(standardPbo.getPixels().getWidth(), standardPbo.getPixels().getHeight(), OF_IMAGE_COLOR);
    floatImage.allocate(floatPbo.getFloatPixels().getWidth(), floatPbo.getFloatPixels().getHeight(), OF_IMAGE_COLOR);

    // Print information about the initialized PBOs
    ofLogNotice() << "Standard PBO setup complete. Is valid: " << standardPbo.isValid();
    ofLogNotice() << "Float PBO setup complete. Is valid: " << floatPbo.isValid();

    ofSetWindowTitle("ofxSinglePbo Example - Press 'v' to toggle float data visualization");
}

//--------------------------------------------------------------
void ofApp::update()
{

    // Update both PBOs to get the latest pixel data from the FBOs
    standardPbo.update();
    ofPixels &standardPixels = standardPbo.getPixels();
    standardImage.setFromPixels(standardPixels);
    standardImage.update();

    floatPbo.update();
    ofFloatPixels &floatPixels = floatPbo.getFloatPixels();
    floatImage.setFromPixels(floatPixels);
    floatImage.update();

    // Debug: Print a pixel value from each PBO
    if (ofGetFrameNum() % 60 == 0)
    { // Once per second
        ofColor standardColor = standardPbo.getPixels().getColor(standardFbo.getWidth() / 2, standardFbo.getHeight() / 2);
        ofFloatColor floatColor = floatPbo.getFloatPixels().getColor(floatFbo.getWidth() / 2, floatFbo.getHeight() / 2);

        ofLogNotice() << "Center pixel from standard FBO: " << standardColor;
        ofLogNotice() << "Center pixel from float FBO: " << floatColor;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(255);

    // Update shader parameters for rendering
    float time = ofGetElapsedTimef();

    // Update standard FBO with animated content
    standardFbo.begin();
    ofClear(0, 0, 0, 255);
    shader.begin();
    shader.setUniform1f("timeSec", time);
    shader.setUniform2f("canvasDims", standardFbo.getWidth(), standardFbo.getHeight());
    ofDrawRectangle(0, 0, standardFbo.getWidth(), standardFbo.getHeight());
    shader.end();
    standardFbo.end();

    // Update float FBO with the same animation but in floating point (0-1 range)
    floatFbo.begin();
    ofClear(0, 0, 0, 255);
    shader.begin();
    shader.setUniform1f("timeSec", time);
    shader.setUniform2f("canvasDims", floatFbo.getWidth(), floatFbo.getHeight());
    ofDrawRectangle(0, 0, floatFbo.getWidth(), floatFbo.getHeight());
    shader.end();
    floatFbo.end();

    // Draw the standard FBO on the left
    standardFbo.draw(10, 10, 400, 400);
    ofDrawBitmapStringHighlight("Standard FBO (GL_RGBA)", 10, 430);

    // Draw the float FBO on the right
    floatFbo.draw(420, 10, 400, 400);
    ofDrawBitmapStringHighlight("Float FBO (GL_RGBA32F)", 420, 430);

    // Draw the standard PBO-copied pixel data below the standard FBO
    standardImage.draw(10, 450, 400, 400);
    ofDrawBitmapStringHighlight("Standard PBO Data (GL_RGBA)", 10, 870);

    // Draw the float PBO-copied pixel data below the float FBO
    floatImage.draw(420, 450, 400, 400);
    ofDrawBitmapStringHighlight("Float PBO Data (GL_RGBA32F)", 420, 870);

    // Display FPS
    ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate(), 1), ofGetWidth() - 120, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}