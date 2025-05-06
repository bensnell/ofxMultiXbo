#pragma once

#include "ofMain.h"
#include "ofxMultiXbo.hpp"

class ofApp : public ofBaseApp
{

  public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // FBO with standard byte format (GL_RGBA)
    ofFbo standardFbo;
    // FBO with floating point format (GL_RGBA32F)
    ofFbo floatFbo;

    // PBO for standard texture
    ofxSinglePbo standardPbo;
    // PBO for float texture
    ofxSinglePbo floatPbo;

    // Shader for rendering to FBOs
    ofShader shader;

    ofImage standardImage;
    ofImage floatImage;
};