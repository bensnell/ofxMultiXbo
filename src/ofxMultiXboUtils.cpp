//
//  AppUtils.cpp
//
//  Created by Ben Snell on 12/2/20.
//

#include "ofxMultiXboUtils.hpp"

// -------------------------------------------------
int getFboGLFormat(ofFbo &fbo)
{
    return fbo.getTexture().texData.glInternalFormat;
}

// -------------------------------------------------
void fillTextureRGBA(ofTexture &tex, ofColor fillColor, ofFbo &utilityFbo, ofBufferObject &utilityBuffer)
{
    utilityFbo.begin();
    ofClear(fillColor);
    utilityFbo.end();
    utilityFbo.copyTo(utilityBuffer);
    tex.loadData(utilityBuffer, GL_RGBA, GL_UNSIGNED_BYTE);
}

// -------------------------------------------------
int modNeg(int a, int b)
{
    if (a < 0)
        a = (a + (abs(a) / b + 1) * b);
    return a % b;
}

// -------------------------------------------------

// -------------------------------------------------