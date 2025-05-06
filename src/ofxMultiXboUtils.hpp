//
//  AppUtils.hpp
//
//  Created by Ben Snell on 12/2/20.
//

#ifndef AppUtils_hpp
#define AppUtils_hpp

#include "ofMain.h"

// Returns glFormat
int getFboGLFormat(ofFbo &fbo);

void fillTextureRGBA(ofTexture &tex, ofColor fillColor, ofFbo &utilityFbo, ofBufferObject &utilityBuffer);

// Apply the modulo operator
// returns a % b where a can be a wrapped negative value
int modNeg(int a, int b);

#endif /* AppUtils_hpp */
