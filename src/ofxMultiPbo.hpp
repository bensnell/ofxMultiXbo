//
//  ofxMultiPbo.hpp
//
//  Created by Ben Snell on 12/2/20.
//

#ifndef ofxMultiPbo_hpp
#define ofxMultiPbo_hpp

#include "ofMain.h"
#include "ofxMultiFbo.hpp"
#include "ofxSinglePbo.hpp"

// A container for multiple Pixel Buffer Objects
// Allows efficient GPU-to-CPU data transfer for multiple textures
// Supports all texture formats that ofxSinglePbo supports, including float data
class ofxMultiPbo
{
  public:
    // Defines which FBO textures to use in ofxMultiFbo
    enum FboSource
    {
        DATA_FBO,   // Use textures from the data FBO (most recent complete frame)
        UTILITY_FBO // Use textures from the utility FBO (frame being rendered or previous frame)
    };

  public:
    ofxMultiPbo();
    ~ofxMultiPbo();

    // Setup with a vector of textures
    bool setup(vector<ofTexture *> _textures);

    // Setup with an ofxMultiFbo (specifying which set of textures to use)
    bool setup(ofxMultiFbo *multiFbo, FboSource source = DATA_FBO);

    // Clear all PBOs
    void clear();

    // Is this multi-PBO valid?
    bool isValid() const { return bValid; }

    // Number of PBOs
    size_t size() const { return pbos.size(); }

    // Get access to a specific PBO instance
    ofxSinglePbo &getPbo(int index);

    // Update all PBOs with the latest textures from the specified FBO source
    void update();

    // Update a specific PBO with its texture
    void update(int index);

    // Get byte pixels from a specific PBO
    ofPixels &getPixels(int index);

    // Get float pixels from a specific PBO (if texture format is float)
    ofFloatPixels &getFloatPixels(int index);

  private:
    bool bValid = false;
    vector<ofxSinglePbo *> pbos;

    // Reference to the MultiFbo for dynamic texture access
    ofxMultiFbo *multiFbo = nullptr;
    FboSource fboSource = DATA_FBO;
    bool usingDirectTextures = false;

    // Static dummy objects for error handling
    static ofxSinglePbo dummyPbo;
    static ofPixels dummyPixels;
    static ofFloatPixels dummyFloatPixels;
};

#endif /* ofxMultiPbo_hpp */
