#pragma once

#include "ofMain.h"
#include "ofxMultiFbo.hpp"
#include "ofxMultiPbo.hpp"

class ofApp : public ofBaseApp
{
  public:
    void setup();
    void update();
    void draw();

  private:
    // Multi-buffer FBO for rendering
    ofxMultiFbo fbo;

    // PBO for efficient GPU-to-CPU transfer
    ofxMultiPbo pbo;

    // Drawing variables
    float phase = 0;
    ofColor colors[3] = {
        ofColor(255, 0, 0), // Red
        ofColor(0, 255, 0), // Green
        ofColor(0, 0, 255)  // Blue
    };

    // Display images - one for each buffer
    ofImage displayImages[3];
};