//
//  ofxMultiPbo.cpp
//
//  Created by Ben Snell on 1/17/19.
//

#include "ofxMultiPbo.hpp"

// -------------------------------------------------
ofxMultiPbo::ofxMultiPbo() {

}

// -------------------------------------------------
ofxMultiPbo::~ofxMultiPbo() {
    
}

// -------------------------------------------------
void ofxMultiPbo::setup(ofxMultiFbo* _mf) {
	if (!_mf->isAllocated()) {
		ofLogError("ofxMultiPbo") << "ofxMultiFbo is not allocated. Not allocating ofxMultiPbo";
		return;
	}
	bool allMfRGBA = true;
	for (int i = 0; i < _mf->getNumBuffers(); i++) {
		allMfRGBA &= (_mf->getGlFormat(i) == GL_RGBA);
	}
	if (!allMfRGBA) {
		ofLogError("ofxMultiPbo") << "All ofxMultiFbo glFormats must be GL_RGBA";
		return;
	}
	if (!_mf->hasPboSupport()) {
		ofLogError("ofxMultiPbo") << "ofxMultiFbo must have Pbo support";
		return;
	}

	mf = _mf;
	
	// Create a pixel buffer object for every fbo buffer
	for (int i = 0; i < mf->getNumBuffers(); i++) {
		
		// Option 1
		// allocate
		ofxSinglePbo* pbo = new ofxSinglePbo();
		pbo->setup(mf->getPboTexture(i));

		// set pixels
		mf->getDataFbo()->readToPixels(pbo->getPixels());

		// save 
		pbos.push_back(pbo);




		// Option 2
		//ofPBO* pbo = new ofPBO();
		//pbo->allocate(*(mf->getPboTexture(i)), 1);
		//pbos.push_back(pbo);

		//ofPixels pix;
		//pix.allocate(mf->getWidth(), mf->getHeight(), OF_IMAGE_COLOR_ALPHA);
		//pixels.push_back(pix);



	}

	bValid = true;
}

// -------------------------------------------------
//ofxSinglePbo& ofxMultiPbo::getPbo(int index) {
//	return *pbos[index];
//}

// -------------------------------------------------
ofPixels& ofxMultiPbo::getPixels(int index) {

	// Option 1
	return pbos[index]->getPixels();

	// Option 2
	//return pixels[index];
}

// -------------------------------------------------
void ofxMultiPbo::update() {
	for (int i = 0; i < pbos.size(); i++) {
		
		// Option 1
		pbos[i]->update(); // mf->getPboBuffer(i));
	

		// Option 2
		//pbos[i]->updateTexture();
		//pbos[i]->loadData(pixels[i], true);
	
	}
}

// -------------------------------------------------

// -------------------------------------------------

// -------------------------------------------------



