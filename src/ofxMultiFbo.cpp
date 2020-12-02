//
//  ofxMultiFbo.cpp
//  g5_002
//
//  Created by Ben Snell on 1/17/19.
//

#include "ofxMultiFbo.hpp"

// -------------------------------------------------
ofxMultiFbo::ofxMultiFbo() {

}

// -------------------------------------------------
ofxMultiFbo::~ofxMultiFbo() {
    
}

// -------------------------------------------------
void ofxMultiFbo::setup(int _width, int _height, vector<GLenum> _glFormats, 
	vector<ofColor> _initColors, bool _bPboSupport) {
	if (_glFormats.size() >= 8 || _initColors.size() >= 8) {
		ofLogError("ofxMultiFbo") << "Cannot create a ofxMultiFbo with this many buffers";
		return;
	}
	if (_initColors.size() < _glFormats.size()) {
		ofLogNotice("ofxMultiFbo") << "Resizing the color buffers";
		_initColors.resize(_glFormats.size(), _initColors.back());
	}
	if (_glFormats.size() < _initColors.size()) {
		_glFormats.resize(_initColors.size(), _glFormats.back());
	}

	glFormats = _glFormats;
	width = _width;
	height = _height;
	bPboSupport = _bPboSupport;

	// Create this many textures
	ofFboSettings settings;
	settings.width = width;
	settings.height = height;

	// Allocate A
	A.allocate(settings);
	for (int i = 0; i < glFormats.size(); i++) {
		A.createAndAttachTexture(glFormats[i], i);
	}
	// Init A
	for (int i = 0; i < _initColors.size(); i++) {
		A.begin();
		A.setActiveDrawBuffer(i);
		ofClear(_initColors[i]);
		A.end();
	}
	
	// Allocate B
	B.allocate(settings);
	for (int i = 0; i < glFormats.size(); i++) {
		B.createAndAttachTexture(glFormats[i], i);
	}
	// Init B
	for (int i = 0; i < _initColors.size(); i++) {
		B.begin();
		B.setActiveDrawBuffer(i);
		ofClear(_initColors[i]);
		B.end();
	}

	if (bPboSupport) {
		// Also allocate a vector of textures for the pbo to copy (without locking onto thr fbo renderer)
		for (int i = 0; i < glFormats.size(); i++) {
			int glFormat = glFormats[i];
			int glType = ofGetGLTypeFromInternal(glFormat);
			int numChannels = ofGetNumChannelsFromGLFormat(glFormat);
			int numBytesPerChannel = ofGetBytesPerChannelFromGLType(glType);
			int numBytes = numChannels * numBytesPerChannel;

			ofBufferObject* buf = new ofBufferObject();
			buf->allocate(width * height * numBytes, GL_DYNAMIC_COPY);
			pboBuffers.push_back(buf);

			ofTexture* tex = new ofTexture();
			tex->allocate(A.getTexture(i).getWidth(), A.getTexture(i).getHeight(), glFormat);
			pboTextures.push_back(tex);
		}
	}

	data = &A;
	utility = &B;
	updatePboTextures();

	bAllocated = true;
}

// -------------------------------------------------
void ofxMultiFbo::setup(int _width, int _height, GLenum _glFormat, int _numBuffers, ofColor _initColor, bool _bPboSupport) {

	vector<GLenum> _glFormats;
	_glFormats.resize(_numBuffers, _glFormat);
	vector<ofColor> _initColors;
	_initColors.resize(_numBuffers, _initColor);
	setup(_width, _height, _glFormats, _initColors, _bPboSupport);
}

// -------------------------------------------------
void ofxMultiFbo::updatePboTextures() {
	if (!bPboSupport) return;

	for (int i = 0; i < pboTextures.size(); i++) {

		// copy the data
		getTex(i).copyTo(*pboBuffers[i]);

		// load into new texture
		pboTextures[i]->loadData(*pboBuffers[i], glFormats[i], ofGetGLTypeFromInternal(glFormats[i]));
	}
}

// -------------------------------------------------

// -------------------------------------------------

// -------------------------------------------------

// -------------------------------------------------

// -------------------------------------------------

// -------------------------------------------------



