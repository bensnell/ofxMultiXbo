//
//  ofxMultiPbo.hpp
//
//  Created by Ben Snell on 1/17/19.
//

#ifndef ofxMultiPbo_hpp
#define ofxMultiPbo_hpp

#include "ofMain.h"
#include "ofxMultiBufferObjectsUtils.hpp"
#include "ofxSinglePbo.hpp"
#include "ofxMultiFbo.hpp"
// #include "ofPBO.h" // ?

// A third option would be to use ofxFastFboReader, but it has the same performance
// as option 1
// Option 2 does not perform better than option 2

// Helper for mulitple pbo (used in tandem with MultiFbo)
// Only to be used with all GL_RGBA MultiFbo's
class ofxMultiPbo {
public:

	ofxMultiPbo();
	~ofxMultiPbo();

	void setup(MultiFbo* _mf);

	bool isValid() { return bValid; }

	//ofxSinglePbo& getPbo(int index);

	void update();

	ofPixels& getPixels(int index);

private:

	bool bValid = false;

	MultiFbo* mf = NULL;

	// Option 1
	vector<ofxSinglePbo*> pbos;
	// Option 2
	//vector<ofPBO*> pbos;
	
	vector<ofPixels> pixels;

};


#endif /* ofxMultiPbo_hpp */
