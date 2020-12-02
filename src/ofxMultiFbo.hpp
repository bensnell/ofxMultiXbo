//
//  ofxMultiFbo.hpp
//  g5_002
//
//  Created by Ben Snell on 1/17/19.
//

#ifndef ofxMultiFbo_hpp
#define ofxMultiFbo_hpp

#include "ofMain.h"
#include "ofxMultiXboUtils.hpp"

// Reference:
// https://forum.openframeworks.cc/t/solved-custom-shader-and-multiple-render-targets/17930/2

// This fbo is an easy handler for multi-buffer fbos 
// It makes shader usage easier
class ofxMultiFbo { 
public:

	ofxMultiFbo();
	~ofxMultiFbo();

	// Setup the multitexture
	void setup(int _width, int _height, vector<GLenum> _glFormats, 
		vector<ofColor> _initColors, bool _bPboSupport = false);
	void setup(int _width, int _height, GLenum _glFormat, 
		int _numBuffers, ofColor _initColor, bool _bPboSupport = false);

	bool isAllocated() { return bAllocated; }

	// Begin utility fbo
	void begin() {
		utility->begin();
		utility->activateAllDrawBuffers();
	}

	// Get data texture (pass this to shader)
	ofTexture& getTex(int index = 0) {
		return data->getTexture(index);
	}

	// End utility fbo
	void end() {
		utility->end();

		//ofFbo* tmp = data;
		//data = utility;
		//utility = tmp;
		swap(data, utility);
		updatePboTextures();
	}


	// Other methods (not as useful):

	// Get the fbo for the most recent data
	ofFbo* getDataFbo() {
		return data;
	}
	// Get the utility fbo
	ofFbo* getUtilityFbo() {
		return utility;
	}
	int getNumBuffers() {
		return glFormats.size();
	}
	GLenum getGlFormat(int index) {
		return glFormats[index];
	}
	int getWidth() { return width; }
	int getHeight() { return height; }

	ofTexture* getPboTexture(int index = 0) {
		return bPboSupport ? pboTextures[index] : NULL;
	}
	ofBufferObject* getPboBuffer(int index = 0) {
		return bPboSupport ? pboBuffers[index] : NULL;
	}
	bool hasPboSupport() { return bPboSupport; }

private:

	vector<GLenum> glFormats;

	ofFbo* data;
	ofFbo* utility;

	ofFbo A;
	ofFbo B;

	int width;
	int height;

	bool bAllocated = false;

	bool bPboSupport = false;
	vector<ofBufferObject*> pboBuffers; // utility
	vector<ofTexture*> pboTextures; // for pbo
	void updatePboTextures();
};


#endif /* ofxMultiFbo_hpp */
