//
//  ofxMultiFboShader.cpp
//
//  Created by Ben Snell on 12/2/20.
//

#include "ofxMultiFboShader.hpp"

// -------------------------------------------------
ofxMultiFboShader::ofxMultiFboShader()
{
}

// -------------------------------------------------
ofxMultiFboShader::~ofxMultiFboShader()
{
}

// -------------------------------------------------
bool ofxMultiFboShader::setup(int __width, int __height, vector<GLenum> _glFormats,
                              vector<ofColor> _initColors,
                              string _shaderPath, vector<string> _textureNames)
{
    bSetup = false;

    if (!ofxMultiFbo::allocate(__width, __height, _glFormats, _initColors))
    {
        ofLogError("ofxMultiFboShader") << "Could not load ofxMultiFbo.";
        return false;
    }

    if (!ofShader::load(_shaderPath))
    {
        ofLogError("ofxMultiFboShader") << "Could not load ofShader.";
        return false;
    }

    if (!setTextureNames(_textureNames))
    {
        ofLogError("ofxMultiFboShader") << "Texture names must be unique.";
        return false;
    }

    bSetup = true;
    return true;
}

// -------------------------------------------------
bool ofxMultiFboShader::setup(int __width, int __height, GLenum _glFormat,
                              int _numBuffers, ofColor _initColor,
                              string _shaderPath, string _texturePrefix)
{
    bSetup = false;

    if (!ofxMultiFbo::allocate(__width, __height, _glFormat, _numBuffers, _initColor))
    {
        ofLogError("ofxMultiFboShader") << "Could not load ofxMultiFbo.";
        return false;
    }

    if (!ofShader::load(_shaderPath))
    {
        ofLogError("ofxMultiFboShader") << "Could not load ofShader.";
        return false;
    }

    vector<string> tmpNames;
    for (int i = 0; i < ofxMultiFbo::size(); i++)
        tmpNames.push_back(_texturePrefix + ofToString(i));
    if (!setTextureNames(tmpNames))
    {
        ofLogError("ofxMultiFboShader") << "Texture names must be unique.";
        return false;
    }

    bSetup = true;
    return true;
}

// -------------------------------------------------
bool ofxMultiFboShader::setTextureNames(vector<string> _textureNames)
{
    // Make sure all names are unique
    set<string> tmpNames;
    for (auto name : _textureNames)
        tmpNames.insert(name);
    if (tmpNames.size() != _textureNames.size())
        return false;
    textureNames = _textureNames;

    return true;
}

// -------------------------------------------------
void ofxMultiFboShader::begin(bool bClear, ofColor clearColor)
{
    if (!isSetup())
        return;

    // Reset texture location counter
    texLocationCounter = 0;

    // Begin the fbo
    ofxMultiFbo::begin();

    // Clear the shader, if instructed to
    if (bClear)
        ofClear(clearColor);

    // Begin the shader
    ofShader::begin();

    // Pass all textures to the shader
    for (int i = 0; i < ofxMultiFbo::size(); i++)
        ofShader::setUniformTexture(textureNames[i], ofxMultiFbo::getTex(i), texLocationCounter++);
}

// -------------------------------------------------
void ofxMultiFboShader::end()
{
    if (!isSetup())
        return;

    // Run the shader
    runShader();

    // End the shader
    ofShader::end();

    // End the fbo (this will also swap textures automatically)
    ofxMultiFbo::end();
}

// -------------------------------------------------
void ofxMultiFboShader::runShader()
{
    if (!isSetup())
        return;

    // Call the shader to run by drawing the first texture
    ofxMultiFbo::getTex(0).draw(0, 0);
}

// -------------------------------------------------

// -------------------------------------------------

// -------------------------------------------------
