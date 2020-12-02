//
//  AppUtils.hpp
//  g5_002
//
//  Created by Ben Snell on 1/17/19.
//

#ifndef AppUtils_hpp
#define AppUtils_hpp

#include "ofMain.h"


// Get a color for a string
ofColor getColor(string a);

// this clamps by default
float mapPower(float val, float inMin, float inMax, float power, float outMin, float outMax);

// Tab a string to the right or left a given number of tabs
// TODO
string getTabbed(string s, int nTabs);

// Returns glFormat
int getFboGLFormat(ofFbo& fbo);

void fillTextureRGBA(ofTexture& tex, ofColor fillColor, ofFbo& utilityFbo, ofBufferObject& utilityBuffer);

enum FontCaseStyle {
	FONT_CASE_LOWER = 0,
	FONT_CASE_UPPER,
	FONT_CASE_CAMEL
};
vector<string> getFontCaseSyleNames();
string convertCase(string in, FontCaseStyle style);

// Get the average color in an image (does not sample alpha)
ofColor getAvgColor(ofImage& img);

// Apply the modulo operator
// returns a % b where a can be a wrapped negative value
int modNeg(int a, int b);

string getStringSlider(float lo, float min, float avg, float max, float hi, int length);
string getStringSlider(float lo, float mid, float hi, int length);

// Pad a string with a number of spaces after the end up to a point.
// This will clip the string to the size given
string pad(string input, int length, char padding = ' ');

#endif /* AppUtils_hpp */
