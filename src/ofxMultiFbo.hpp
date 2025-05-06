//
//  ofxMultiFbo.hpp
//
//  Created by Ben Snell on 12/2/20.
//

#ifndef ofxMultiFbo_hpp
#define ofxMultiFbo_hpp

#include "ofMain.h"
#include "ofxMultiXboUtils.hpp"

// Reference:
// https://forum.openframeworks.cc/t/solved-custom-shader-and-multiple-render-targets/17930/2

// This fbo is an easy handler for multi-buffer fbos.
// It makes shader usage easier.
// Note: All buffers must be the same size, but they can be different types.
class ofxMultiFbo
{
  public:
    ofxMultiFbo();
    ~ofxMultiFbo();

    // Allocate the multitexture
    bool allocate(int __width, int __height, vector<GLenum> _glFormats,
                  vector<ofColor> _initColors);
    bool allocate(int __width, int __height, GLenum _glFormat,
                  int _numBuffers, ofColor _initColor);
    bool isAllocated() { return bAllocated; }

    // Helper methods
    int size() { return getNumBuffers(); }
    int width() { return _width; }
    int height() { return _height; }
    GLenum getGlFormat(int index) { return glFormats[index]; }

    // Begin utility fbo (bind to all buffers)
    bool begin();

    // End binding to all buffers within the fbo, then swap internal buffers (by default)
    bool end(bool swapBuffers = true);

    // Get data texture (pass this to shader)
    ofTexture &getTex(int index = 0);
    ofTexture &getTexture(int index = 0) { return getTex(index); };

    // Get vector of all textures from the data FBO (most recently rendered frame)
    vector<ofTexture *> getTextures();

    // Get vector of all textures from the data FBO (most recently rendered frame)
    vector<ofTexture *> getDataTextures();

    // Get vector of all textures from the utility FBO (currently being rendered to or previous frame)
    vector<ofTexture *> getUtilityTextures();

    // ===================================
    // ======== ADVANCED METHODS =========
    // ===================================

    // Begin drawing to a specific buffer within the fbo (advanced usage only)
    bool begin(int index);

    // End drawing to a specific buffer object.
    // Note: You need to manually swap buffers by caling swap()
    bool end(int index);

    // Manually swap buffers.
    void swap();

    // ===================================
    // ========= HELPER METHODS ==========
    // ===================================

    // Get the fbo for the most recent data
    ofFbo *getDataFbo()
    {
        return data;
    }
    // Get the utility fbo
    ofFbo *getUtilityFbo()
    {
        return utility;
    }
    int getNumBuffers()
    {
        return glFormats.size();
    }
    bool validBufferIndex(int index)
    {
        return index >= 0 && index < getNumBuffers();
    }

    // ===================================
    // ========== DRAW METHODS ===========
    // ===================================

    // Draw the fbo (at the given index) at the provided coords
    bool draw(float x, float y, float w, float h, int index = 0);
    bool draw(float x, float y, int index = 0);

  private:
    vector<GLenum> glFormats;

    ofFbo *data;
    ofFbo *utility;

    set<int> activeUtilityBuffers;
    bool isUtilityBufferActive(int index);
    bool isUtilityBufferActive();

    ofFbo A;
    ofFbo B;

    int _width;
    int _height;

    bool bAllocated = false;
};

#endif /* ofxMultiFbo_hpp */
