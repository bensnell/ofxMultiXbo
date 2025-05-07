#include "ofApp.h"

void ofApp::setup()
{

    // Setup the fbo shader (renderer)
    renderer.setup(100, 100, USE_FLOATS ? GL_RGBA32F : GL_RGBA, NUM_BUFFERS, ofColor(255), "shaders/myShader", "inTex");

    pbo.setup(&renderer);

    // Allocate images for display
    for (int i = 0; i < NUM_BUFFERS; i++)
    {
        displayImages[i].allocate(100, 100, OF_IMAGE_COLOR_ALPHA);
    }

    // Initialize fbos with whatever information you need
    ofSetCircleResolution(32);
    // Here, we'll draw a blue circle on a white background at buffer index 2
    renderer.beginFbo(2);
    ofSetColor(0, 0, 255, 255);
    ofDrawCircle(50, 50, 50);
    renderer.endFbo(2);
    // ... And a black circle on a transparent background at buffer index 3
    renderer.beginFbo(3);
    ofClear(0, 0, 0, 0);
    ofSetColor(255, 255, 255, 255);
    ofDrawCircle(50, 50, 50);
    renderer.endFbo(3);
    // When individually writing to buffers, remember to end all at once or call swap
    renderer.endFbo();
}

void ofApp::update()
{

    // Begin rendering
    renderer.begin(true, ofColor(0, 0, 0, 0));

    // Pass any uniforms you need in the shader
    renderer.setUniform1f("time", ofGetElapsedTimef());
    renderer.setUniform1f("width", renderer.width());
    renderer.setUniform1f("height", renderer.height());

    // End rendering
    renderer.end();

    pbo.update();

    // Get pixel data from each buffer and update the display images
    for (int i = 0; i < NUM_BUFFERS; i++)
    {
        if (USE_FLOATS)
        {
            displayImages[i].setFromPixels(pbo.getFloatPixels(i));
        }
        else
        {
            displayImages[i].setFromPixels(pbo.getPixels(i));
        }
        displayImages[i].update();
    }
}

void ofApp::draw()
{
    // Increase y-position to add space for text
    const int yOffset = 120;
    const int xOffset = 120;
    const int imgSize = 100;

    for (int i = 0; i < NUM_BUFFERS; i++)
    {
        ofSetColor(255);
        renderer.getTex(i).draw(xOffset * i, 0, imgSize, imgSize);
        displayImages[i].draw(xOffset * i, yOffset, imgSize, imgSize);
        ofDrawBitmapString("FBO " + ofToString(i), xOffset * i, yOffset - 5);
        ofDrawBitmapString("PBO " + ofToString(i), xOffset * i, yOffset + imgSize + 15);
    }
}

void ofApp::keyPressed(int key)
{
}

void ofApp::keyReleased(int key)
{
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
}

void ofApp::mousePressed(int x, int y, int button)
{
}

void ofApp::mouseReleased(int x, int y, int button)
{
}

void ofApp::windowResized(int w, int h)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
