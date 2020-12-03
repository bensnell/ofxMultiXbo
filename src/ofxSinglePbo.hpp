//
//  ofxSinglePbo.hpp
//
//  Created by Ben Snell on 12/2/20.
//

#ifndef ofxSinglePbo_hpp
#define ofxSinglePbo_hpp

#include "ofMain.h"

// Pixel buffer objects allow information to be read from the GPU to the CPU.
// This object performs the same function as myFbo.readToPixels(myPixels),
// but without performance issues.
class ofxSinglePbo {
public:
    
    ofxSinglePbo();
    ~ofxSinglePbo();

	// Setup and allocate this object using the settings from this texture
	bool setup(ofTexture* _texture);
	
	// Is this PBO valid?
	bool isValid();

	// Call update outside of the GL loop.
	// Use the existing texture to update this PBO
	void update();
	void update(ofTexture* _tex); // update with a specific texture (advanced usage)
	void update(ofBufferObject* _buf);

	// Get the pixels
	ofPixels& getPixels();

private:

	// Is this PBO valid?
	bool bValid = false;

	// This is the original (GL) object we want to get data from
	ofTexture* texture;
	int width = -1;
	int height = -1;
	int nChannels = -1;

	// These buffers are used to async read the texture
	ofBufferObject pixelBufferBack, pixelBufferFront;

	// This pixels object contains the CPU-accessible data
	ofPixels pixels; // needed?
	unsigned char* p;

	ofMutex mtx;
};

#endif /* ofxSinglePbo_hpp */
