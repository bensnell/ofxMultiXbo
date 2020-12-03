//
//  ofxSinglePbo.cpp
//
//  Created by Ben Snell on 12/2/20.
//

#include "ofxSinglePbo.hpp"

// --------------------------------------------------
ofxSinglePbo::ofxSinglePbo() {
    
}

// --------------------------------------------------
ofxSinglePbo::~ofxSinglePbo() {
    
}

// --------------------------------------------------
bool ofxSinglePbo::setup(ofTexture* _texture) {
	bValid = false;
	if (_texture == NULL) {
		ofLogError("ofxSinglePbo") << "Cannot pass null texture";
		return bValid;
	}
	texture = _texture;

	// validate the type
	int glFormat = texture->getTextureData().glInternalFormat;
	nChannels = (glFormat == GL_R || glFormat == GL_LUMINANCE || glFormat == GL_INTENSITY) ? 1 : (glFormat == GL_RG ? 2 : (glFormat == GL_RGB ? 3 : (glFormat == GL_RGBA ? 4 : -1)));
	if (nChannels <= 0) {
		ofLogError("ofxSinglePbo") << "GL Format is not recognized";
		return bValid;
	}

	// validate the size
	width = texture->getWidth();
	height = texture->getHeight();
	if (width <= 0 || height <= 0) {
		ofLogError("ofxSinglePbo") << "Texture dimensions invalid";
		return bValid;
	}

	bValid = true;

	ofLogVerbose("ofxSinglePbo") << "Allocating PBO " << width << " x " << height << " x " << nChannels;

	// Allocate the buffers
	pixelBufferBack.allocate(width * height * nChannels, GL_DYNAMIC_READ);
	pixelBufferFront.allocate(width * height * nChannels, GL_DYNAMIC_READ);

	// Allocate the pixels
	pixels.allocate(width, height, nChannels);
	return bValid;
}

// --------------------------------------------------
bool ofxSinglePbo::isValid() {
	return bValid;
}

// --------------------------------------------------
void ofxSinglePbo::update() {
	update(texture);
}

// --------------------------------------------------
void ofxSinglePbo::update(ofTexture* _tex) {
	if (!isValid()) {
		ofLogWarning("ofxSinglePbo") << "Invalid pbo";
		return;
	}
	if (_tex == NULL) {
		ofLogWarning("ofxSinglePbo") << "Texture passed is null.";
		return;
	}
	if (!_tex->isAllocated()) {
		ofLogWarning("ofxSinglePbo") << "Texture is not allocated.";
	}

	// Unmap the back buffer?
	pixelBufferBack.unmap();

	// Copy the texture to a buffer
	_tex->copyTo(pixelBufferBack);

	// bind and map the buffer as PIXEL_UNPACK so it can be
	// accessed from a different thread  from the cpu
	// and send the memory address to the saver thread
	pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
	p = pixelBufferFront.map<unsigned char>(GL_READ_ONLY);
	
	// Copy to a pixels object
	mtx.lock();
	pixels.setFromPixels(p, width, height, nChannels);
	mtx.unlock();

	// The pixel buffer must be unbound. If not, many GL operations will not work
	// as expected (for example, textures will turn black).
	// This is of particular concern to the following addons
	// - ofxDXT
	// - ofxImageSequenceVideo
	// - ofxAnimationAssetManager
	// since they depend on glCompressedTexImage2D, glTexImage2D, etc.
	// See this page for reference: 
	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
	// and the paragraph on this page that reads
	//     "While a non-zero buffer object is bound to the GL_PIXEL_UNPACK_BUFFER target, 
	//		the following commands are affected: glCompressedTexImage1D, glCompressedTexImage2D, 
	//		glCompressedTexImage3D, glCompressedTexSubImage1D, glCompressedTexSubImage2D, 
	//		glCompressedTexSubImage3D, glTexImage1D, glTexImage2D, glTexImage3D, glTexSubImage1D, 
	//		glTexSubImage2D, and glTexSubImage3D. The pointer parameter is interpreted as an 
	//		offset within the buffer object measured in basic machine units."
	pixelBufferFront.unbind(GL_PIXEL_UNPACK_BUFFER);

	// swap the front and back buffer so we are always
	// copying the texture to one buffer and reading
	// back from another to avoid stalls
	swap(pixelBufferBack, pixelBufferFront);
}

// --------------------------------------------------
ofPixels& ofxSinglePbo::getPixels() {
	return pixels;
}

// --------------------------------------------------
void ofxSinglePbo::update(ofBufferObject* _buf) {
	if (!isValid()) {
		ofLogWarning("ofxSinglePbo") << "Invalid pbo";
		return;
	}
	if (_buf == NULL) {
		ofLogWarning("ofxSinglePbo") << "Buffer passed is null";
		return;
	}

	// Unmap the back buffer?
	pixelBufferBack.unmap();

	// Copy the texture to a buffer
	_buf->copyTo(pixelBufferBack);

	// bind and map the buffer as PIXEL_UNPACK so it can be
	// accessed from a different thread  from the cpu
	// and send the memory address to the saver thread
	pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
	p = pixelBufferFront.map<unsigned char>(GL_READ_ONLY);

	// Copy to a pixels object
	mtx.lock();
	pixels.setFromPixels(p, width, height, nChannels);
	mtx.unlock();

	// swap the front and back buffer so we are always
	// copying the texture to one buffer and reading
	// back from another to avoid stalls
	swap(pixelBufferBack, pixelBufferFront);
}

// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------