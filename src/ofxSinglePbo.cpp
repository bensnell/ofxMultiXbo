//
//  ofxSinglePbo.cpp
//
//  Created by Ben Snell on 12/2/20.
//

#include "ofxSinglePbo.hpp"

// --------------------------------------------------
ofxSinglePbo::ofxSinglePbo()
{
}

// --------------------------------------------------
ofxSinglePbo::~ofxSinglePbo()
{
}

// --------------------------------------------------
bool ofxSinglePbo::setup(ofTexture *_texture)
{
    bValid = false;
    if (_texture == NULL)
    {
        ofLogError("ofxSinglePbo") << "Cannot pass null texture";
        return bValid;
    }
    texture = _texture;

    // Store the GL format
    glFormat = texture->getTextureData().glInternalFormat;

    // Determine number of channels and whether format uses floating point values
    nChannels = -1;
    bIsFloat = false;

    // Single channel formats
    if (glFormat == GL_R || glFormat == GL_LUMINANCE || glFormat == GL_INTENSITY)
    {
        nChannels = 1;
        bIsFloat = false;
    }
    else if (glFormat == GL_R32F || glFormat == GL_R16F)
    {
        nChannels = 1;
        bIsFloat = true;
    }
    // Two channel formats
    else if (glFormat == GL_RG)
    {
        nChannels = 2;
        bIsFloat = false;
    }
    else if (glFormat == GL_RG32F || glFormat == GL_RG16F)
    {
        nChannels = 2;
        bIsFloat = true;
    }
    // Three channel formats
    else if (glFormat == GL_RGB)
    {
        nChannels = 3;
        bIsFloat = false;
    }
    else if (glFormat == GL_RGB32F || glFormat == GL_RGB16F)
    {
        nChannels = 3;
        bIsFloat = true;
    }
    // Four channel formats
    else if (glFormat == GL_RGBA)
    {
        nChannels = 4;
        bIsFloat = false;
    }
    else if (glFormat == GL_RGBA32F || glFormat == GL_RGBA16F)
    {
        nChannels = 4;
        bIsFloat = true;
    }

    if (nChannels <= 0)
    {
        ofLogError("ofxSinglePbo") << "GL Format is not recognized: " << glFormat;
        return bValid;
    }

    // validate the size
    width = texture->getWidth();
    height = texture->getHeight();
    if (width <= 0 || height <= 0)
    {
        ofLogError("ofxSinglePbo") << "Texture dimensions invalid";
        return bValid;
    }

    bValid = true;

    ofLogVerbose("ofxSinglePbo") << "Allocating PBO " << width << " x " << height << " x " << nChannels
                                 << (bIsFloat ? " (float)" : " (byte)");

    // Calculate buffer size based on data type (float or byte)
    size_t bufferSize = width * height * nChannels * (bIsFloat ? sizeof(float) : sizeof(unsigned char));

    // Allocate the buffers
    pixelBufferBack.allocate(bufferSize, GL_DYNAMIC_READ);
    pixelBufferFront.allocate(bufferSize, GL_DYNAMIC_READ);

    // Allocate the appropriate pixels object
    if (bIsFloat)
    {
        floatPixels.allocate(width, height, nChannels);
    }
    else
    {
        pixels.allocate(width, height, nChannels);
    }

    return bValid;
}

// --------------------------------------------------
bool ofxSinglePbo::isValid()
{
    return bValid;
}

// --------------------------------------------------
void ofxSinglePbo::update()
{
    update(texture);
}

// --------------------------------------------------
void ofxSinglePbo::update(ofTexture *_tex)
{
    if (!isValid())
    {
        ofLogWarning("ofxSinglePbo") << "Invalid pbo";
        return;
    }
    if (_tex == NULL)
    {
        ofLogWarning("ofxSinglePbo") << "Texture passed is null.";
        return;
    }
    if (!_tex->isAllocated())
    {
        ofLogWarning("ofxSinglePbo") << "Texture is not allocated.";
    }

    // Unmap the back buffer
    pixelBufferBack.unmap();

    // Copy the texture to a buffer
    _tex->copyTo(pixelBufferBack);

    // bind and map the buffer as PIXEL_UNPACK so it can be
    // accessed from a different thread from the cpu
    // and send the memory address to the saver thread
    pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
    p = pixelBufferFront.map<void>(GL_READ_ONLY);

    // Copy to the appropriate pixels object
    mtx.lock();
    if (bIsFloat)
    {
        floatPixels.setFromPixels(static_cast<float *>(p), width, height, nChannels);
    }
    else
    {
        pixels.setFromPixels(static_cast<unsigned char *>(p), width, height, nChannels);
    }
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
ofPixels &ofxSinglePbo::getPixels()
{
    if (bIsFloat)
    {
        ofLogWarning("ofxSinglePbo") << "Attempting to get byte pixels from a float texture. Use getFloatPixels() instead.";
    }
    return pixels;
}

// --------------------------------------------------
ofFloatPixels &ofxSinglePbo::getFloatPixels()
{
    if (!bIsFloat)
    {
        ofLogWarning("ofxSinglePbo") << "Attempting to get float pixels from a byte texture. Use getPixels() instead.";
    }
    return floatPixels;
}

// --------------------------------------------------
void ofxSinglePbo::update(ofBufferObject *_buf)
{
    if (!isValid())
    {
        ofLogWarning("ofxSinglePbo") << "Invalid pbo";
        return;
    }
    if (_buf == NULL)
    {
        ofLogWarning("ofxSinglePbo") << "Buffer passed is null";
        return;
    }

    // Unmap the back buffer
    pixelBufferBack.unmap();

    // Copy the texture to a buffer
    _buf->copyTo(pixelBufferBack);

    // bind and map the buffer as PIXEL_UNPACK so it can be
    // accessed from a different thread from the cpu
    pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
    p = pixelBufferFront.map<void>(GL_READ_ONLY);

    // Copy to the appropriate pixels object
    mtx.lock();
    if (bIsFloat)
    {
        floatPixels.setFromPixels(static_cast<float *>(p), width, height, nChannels);
    }
    else
    {
        pixels.setFromPixels(static_cast<unsigned char *>(p), width, height, nChannels);
    }
    mtx.unlock();

    // Unbind the buffer
    pixelBufferFront.unbind(GL_PIXEL_UNPACK_BUFFER);

    // swap the front and back buffer so we are always
    // copying the texture to one buffer and reading
    // back from another to avoid stalls
    swap(pixelBufferBack, pixelBufferFront);
}