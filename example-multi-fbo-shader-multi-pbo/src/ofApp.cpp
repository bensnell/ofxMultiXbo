#include "ofApp.h"

void ofApp::setup()
{

    // Setup the fbo shader (renderer)
    renderer.setup(100, 100, GL_RGBA, 5, ofColor(255), "shaders/myShader", "inTex");

    pbo.setup(&renderer);

    // Allocate images for display
    for (int i = 0; i < 5; i++)
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
    for (int i = 0; i < 5; i++)
    {
        displayImages[i].setFromPixels(pbo.getPixels(i));
        displayImages[i].update();
    }
}

void ofApp::draw()
{
    // Increase y-position to add space for text
    const int yOffset = 120;
    const int xOffset = 120;
    const int imgSize = 100;

    // First texture should be all red
    renderer.getTex(0).draw(0, 0, imgSize, imgSize);
    displayImages[0].draw(0, yOffset, imgSize, imgSize);
    ofSetColor(255);
    ofDrawBitmapString("FBO 0", 0, yOffset - 5);
    ofDrawBitmapString("PBO 0", 0, yOffset + imgSize + 15);

    // Second texture should be a red/yellow/black gradient
    renderer.getTex(1).draw(xOffset, 0, imgSize, imgSize);
    displayImages[1].draw(xOffset, yOffset, imgSize, imgSize);
    ofSetColor(255);
    ofDrawBitmapString("FBO 1", xOffset, yOffset - 5);
    ofDrawBitmapString("PBO 1", xOffset, yOffset + imgSize + 15);

    // Third texture is a blue circle
    renderer.getTex(2).draw(xOffset * 2, 0, imgSize, imgSize);
    displayImages[2].draw(xOffset * 2, yOffset, imgSize, imgSize);
    ofSetColor(255);
    ofDrawBitmapString("FBO 2", xOffset * 2, yOffset - 5);
    ofDrawBitmapString("PBO 2", xOffset * 2, yOffset + imgSize + 15);

    // Fourth texture is a gradient circle on transparent background
    renderer.getTex(3).draw(xOffset * 3, 0, imgSize, imgSize);
    displayImages[3].draw(xOffset * 3, yOffset, imgSize, imgSize);
    ofSetColor(255);
    ofDrawBitmapString("FBO 3", xOffset * 3, yOffset - 5);
    ofDrawBitmapString("PBO 3", xOffset * 3, yOffset + imgSize + 15);

    // Fifth texture is an animated gradient circle on transparent background
    renderer.getTex(4).draw(xOffset * 4, 0, imgSize, imgSize);
    displayImages[4].draw(xOffset * 4, yOffset, imgSize, imgSize);
    ofSetColor(255);
    ofDrawBitmapString("FBO 4", xOffset * 4, yOffset - 5);
    ofDrawBitmapString("PBO 4", xOffset * 4, yOffset + imgSize + 15);
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
