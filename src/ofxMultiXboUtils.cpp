//
//  AppUtils.cpp
//  g5_002
//
//  Created by Ben Snell on 1/17/19.
//

#include "ofxMultiXboUtils.hpp"

ofColor getColor(string a) {
	std::hash<string> hasher;
	size_t h = hasher(a);
	int hue = h % 255;
	int sat = 255;
	int brt = (h / 31) % 100 + 155;
	ofColor out = ofColor::fromHsb(hue, sat, brt);
	return out;
}

// -------------------------------------------------
float mapPower(float val, float inMin, float inMax, float power, float outMin, float outMax) {
	float tmp = ofMap(val, inMin, inMax, 0, 1, true);
	tmp = powf(tmp, power);
	return ofMap(tmp, 0, 1, outMin, outMax, true);
}

// -------------------------------------------------
string getTabbed(string s, int nTabs) {
	if (s.empty()) return "";
	nTabs = abs(nTabs);

	string out = s;
	size_t lastPos = 0;
	while (lastPos != string::npos) {

		// Insert a tab
		for (int i = 0; i < nTabs; i++) {
			out.insert(lastPos + (lastPos == 0 ? 0 : 1), "\t");
			lastPos++;
		}
		lastPos++;

		// Find the next position
		lastPos = out.find("\n", lastPos);
	}
	return out;
}

// -------------------------------------------------
int getFboGLFormat(ofFbo& fbo) {
	return fbo.getTexture().texData.glInternalFormat;
}

// -------------------------------------------------
void fillTextureRGBA(ofTexture& tex, ofColor fillColor, ofFbo& utilityFbo, ofBufferObject& utilityBuffer) {
	utilityFbo.begin();
	ofClear(fillColor);
	utilityFbo.end();
	utilityFbo.copyTo(utilityBuffer);
	tex.loadData(utilityBuffer, GL_RGBA, GL_UNSIGNED_BYTE);
}

// -------------------------------------------------
string convertCase(string in, FontCaseStyle style) {

	switch (style) {
	case FONT_CASE_LOWER: {
		return ofToLower(in);
	}; break;
	case FONT_CASE_UPPER: {
		return ofToUpper(in);
	}; break;
	case FONT_CASE_CAMEL: default: {
		vector<string> words = ofSplitString(in, " ");
		for (int i = 0; i < words.size(); i++) {
			words[i] = ofToLower(words[i]);
			words[i][0] = ofToUpper(words[i])[0];
		}
		return ofJoinString(words, " ");
	}; break;
	}
	return in;
}

// -------------------------------------------------
vector<string> getFontCaseSyleNames() {
	vector<string> out = { "Lowercase", "Uppercase", "Camelcase" };
	return out;
}

// -------------------------------------------------
ofColor getAvgColor(ofImage& img) {

	uint64_t sumR = 0;
	uint64_t sumG = 0;
	uint64_t sumB = 0;

	for (int x = 0; x < img.getWidth(); x++) {
		for (int y = 0; y < img.getHeight(); y++) {
			ofColor c = img.getColor(x, y);
			sumR += uint64_t(c.r);
			sumG += uint64_t(c.g);
			sumB += uint64_t(c.b);
		}
	}

	int nPixels = img.getWidth() * img.getHeight();
	return ofColor(
		CLAMP(int(round(float(sumR) / float(nPixels))), 0, 255),
		CLAMP(int(round(float(sumG) / float(nPixels))), 0, 255),
		CLAMP(int(round(float(sumB) / float(nPixels))), 0, 255),
		255
	);
}

// -------------------------------------------------
int modNeg(int a, int b) {
	if (a < 0) a = (a + (abs(a) / b + 1) * b);
	return a % b;
}

// -------------------------------------------------
string getStringSlider(float lo, float mid, float hi, int length) {
	return getStringSlider(lo, mid, mid, mid, hi, length);
}

// -------------------------------------------------
string getStringSlider(float _lo, float _min, float _avg, float _max, float _hi, int _length) {

	// Normalize and clamp everything
	_min = ofMap(_min, _lo, _hi, 0.0, 1.0, true);
	_max = max(_min, ofMap(_max, _lo, _hi, 0.0, 1.0, true));
	_avg = CLAMP(ofMap(_avg, _lo, _hi, 0.0, 1.0, true), _min, _max);
	_lo = 0.0;
	_hi = 1.0;

	// Generate the character locations
	int minIndex = floor(_min * float(_length));
	int maxIndex = floor(_max * float(_length));
	int avgIndex = floor(_avg * float(_length));

	// Create the stream
	stringstream ss;
	ss << "[";
	for (int i = 0; i < _length; i++) {
		string thisChar = "-";
		if (i > minIndex && i < maxIndex) thisChar = "=";
		if (i == maxIndex) thisChar = ")";
		if (i == minIndex) thisChar = "(";
		if (i == avgIndex) thisChar = "X";
		ss << thisChar;
	}
	ss << "]";
	return ss.str();
}

// -------------------------------------------------
string pad(string input, int length, char padding) {
	input.resize(length, padding);
	return input;
}

// -------------------------------------------------

// -------------------------------------------------