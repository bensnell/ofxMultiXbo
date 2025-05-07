#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(60);

    // Set up FBO with 3 buffers (RGB format)
    fbo.allocate(512, 512, GL_RGB, 3, ofColor(0));

    // Set up PBO to read from the DATA_FBO (most recent rendered frame)
    pbo.setup(&fbo, ofxMultiPbo::DATA_FBO);

    // Allocate images for display
    for (int i = 0; i < 3; i++)
    {
        displayImages[i].allocate(512, 512, OF_IMAGE_COLOR);
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Animate phase
    phase += 0.01;

    // Draw different content to each buffer
    for (int i = 0; i < 3; i++)
    {
        // Begin drawing to this specific buffer
        fbo.begin(i);
        ofClear(0, 0, 0);

        // Draw some animated content
        ofSetColor(colors[i]);
        ofDrawCircle(256 + 100 * sin(phase + i * TWO_PI / 3),
                     256 + 100 * cos(phase + i * TWO_PI / 3),
                     50 + 20 * sin(phase * 2));

        // End drawing to this buffer (don't swap yet)
        fbo.end(i);
    }

    // Now swap the FBO buffers to make the new content available
    fbo.swap();

    // Update PBO - this automatically gets the current DATA_FBO textures
    pbo.update();

    // Get pixel data from each buffer and update the display images
    for (int i = 0; i < 3; i++)
    {
        displayImages[i].setFromPixels(pbo.getPixels(i));
        displayImages[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(40);

    // Display our content

    // Draw the FBO textures at the top
    for (int i = 0; i < 3; i++)
    {
        fbo.draw(20 + i * 170, 20, 160, 160, i);
        ofDrawBitmapStringHighlight("FBO " + ofToString(i), 20 + i * 170, 200);
    }

    // Draw the images created from PBO pixels directly below their respective FBO textures
    for (int i = 0; i < 3; i++)
    {
        displayImages[i].draw(20 + i * 170, 220, 160, 160);
        ofDrawBitmapStringHighlight("PBO " + ofToString(i), 20 + i * 170, 400);
    }

    // Draw instructions
    ofDrawBitmapStringHighlight("Example: Using ofxMultiPbo with ofxMultiFbo DATA_FBO", 20, ofGetHeight() - 40);
    ofDrawBitmapStringHighlight("The PBO automatically reads from the most recent FBO data", 20, ofGetHeight() - 20);
}