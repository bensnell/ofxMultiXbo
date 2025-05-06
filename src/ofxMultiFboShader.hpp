//
//  ofxMultiFboShader.hpp
//
//  Created by Ben Snell on 12/2/20.
//

#ifndef ofxMultiFboShader_hpp
#define ofxMultiFboShader_hpp

#include "ofMain.h"
#include "ofxMultiFbo.hpp"
#include "ofxMultiXboUtils.hpp"

class ofxMultiFboShader : public ofxMultiFbo, public ofShader
{
  public:
    ofxMultiFboShader();
    ~ofxMultiFboShader();

    bool setup(int __width, int __height, vector<GLenum> _glFormats,
               vector<ofColor> _initColors,
               string _shaderPath, vector<string> _textureNames);

    bool setup(int __width, int __height, GLenum _glFormat,
               int _numBuffers, ofColor _initColor,
               string _shaderPath, string _texturePrefix = "tex");

    bool isSetup() { return bSetup; }

    // Begin the fbo and the shader and pass all fbo textures to the shader
    void begin(bool bClear = false, ofColor clearColor = ofColor(0, 0, 0, 0));
    int texLocationCounter = 0;

    // Call the shader to run, then end the shader and the fbo
    void end();

    // Begin and end specific FBOs
    bool beginFbo(int index) { return ofxMultiFbo::begin(index); }
    bool endFbo(int index) { return ofxMultiFbo::end(index); }
    bool endFbo(bool swapBuffers = true) { return ofxMultiFbo::end(swapBuffers); }

  private:
    // Is this setup?
    bool bSetup = false;

    vector<string> textureNames;
    bool setTextureNames(vector<string> _textureNames);

    // Call the shader to run
    void runShader();
};

#endif /* ofxMultiFboShader_hpp */
