//
//  ofxMultiPbo.cpp
//
//  Created by Ben Snell on 12/2/20.
//

#include "ofxMultiPbo.hpp"

// Static dummy objects for error handling
ofxSinglePbo ofxMultiPbo::dummyPbo;
ofPixels ofxMultiPbo::dummyPixels;
ofFloatPixels ofxMultiPbo::dummyFloatPixels;

// -------------------------------------------------
ofxMultiPbo::ofxMultiPbo()
{
}

// -------------------------------------------------
ofxMultiPbo::~ofxMultiPbo()
{
    clear();
}

// -------------------------------------------------
bool ofxMultiPbo::setup(vector<ofTexture *> _textures)
{
    clear();

    if (_textures.empty())
    {
        ofLogError("ofxMultiPbo") << "Cannot setup with empty texture list";
        return false;
    }

    // Create PBOs for each texture
    for (int i = 0; i < _textures.size(); i++)
    {
        ofxSinglePbo *pbo = new ofxSinglePbo();

        if (!pbo->setup(_textures[i]))
        {
            ofLogError("ofxMultiPbo") << "Failed to setup PBO for texture " << i;
            delete pbo;
            continue;
        }

        pbos.push_back(pbo);
    }

    usingDirectTextures = true;
    multiFbo = nullptr;
    bValid = !pbos.empty();

    return bValid;
}

// -------------------------------------------------
bool ofxMultiPbo::setup(ofxMultiFbo *_multiFbo, FboSource source)
{
    clear();

    if (_multiFbo == nullptr || !_multiFbo->isAllocated())
    {
        ofLogError("ofxMultiPbo") << "Cannot setup with null or unallocated ofxMultiFbo";
        return false;
    }

    // Store reference to MultiFbo for dynamic access
    multiFbo = _multiFbo;
    fboSource = source;
    usingDirectTextures = false;

    // Create empty PBOs for each buffer in the MultiFbo
    int numBuffers = multiFbo->getNumBuffers();

    if (numBuffers <= 0)
    {
        ofLogError("ofxMultiPbo") << "MultiFbo has no buffers";
        return false;
    }

    // Get current textures based on source
    vector<ofTexture *> textures;
    if (source == DATA_FBO)
    {
        textures = multiFbo->getDataTextures();
    }
    else
    {
        textures = multiFbo->getUtilityTextures();
    }

    // Create PBOs for each texture
    for (int i = 0; i < textures.size(); i++)
    {
        ofxSinglePbo *pbo = new ofxSinglePbo();

        if (!pbo->setup(textures[i]))
        {
            ofLogError("ofxMultiPbo") << "Failed to setup PBO for texture " << i;
            delete pbo;
            continue;
        }

        pbos.push_back(pbo);
    }

    bValid = !pbos.empty();
    return bValid;
}

// -------------------------------------------------
void ofxMultiPbo::clear()
{
    for (auto &pbo : pbos)
    {
        if (pbo != nullptr)
        {
            delete pbo;
        }
    }
    pbos.clear();
    multiFbo = nullptr;
    usingDirectTextures = false;
    bValid = false;
}

// -------------------------------------------------
ofxSinglePbo &ofxMultiPbo::getPbo(int index)
{
    if (index < 0 || index >= pbos.size())
    {
        ofLogError("ofxMultiPbo") << "PBO index out of range: " << index;
        return dummyPbo;
    }
    return *pbos[index];
}

// -------------------------------------------------
void ofxMultiPbo::update()
{
    if (!bValid)
    {
        return;
    }

    // If using dynamic MultiFbo textures, update them with the current state
    if (!usingDirectTextures && multiFbo != nullptr)
    {
        vector<ofTexture *> currentTextures;

        if (fboSource == DATA_FBO)
        {
            currentTextures = multiFbo->getDataTextures();
        }
        else
        {
            currentTextures = multiFbo->getUtilityTextures();
        }

        // Update each PBO with the current texture
        for (int i = 0; i < pbos.size() && i < currentTextures.size(); i++)
        {
            pbos[i]->update(currentTextures[i]);
        }
    }
    else
    {
        // Using direct textures, just update normally
        for (auto &pbo : pbos)
        {
            if (pbo != nullptr && pbo->isValid())
            {
                pbo->update();
            }
        }
    }
}

// -------------------------------------------------
void ofxMultiPbo::update(int index)
{
    if (!bValid || index < 0 || index >= pbos.size())
    {
        ofLogError("ofxMultiPbo") << "PBO index out of range: " << index;
        return;
    }

    // If using dynamic MultiFbo textures, update with the current state
    if (!usingDirectTextures && multiFbo != nullptr)
    {
        vector<ofTexture *> currentTextures;

        if (fboSource == DATA_FBO)
        {
            currentTextures = multiFbo->getDataTextures();
        }
        else
        {
            currentTextures = multiFbo->getUtilityTextures();
        }

        if (index < currentTextures.size())
        {
            pbos[index]->update(currentTextures[index]);
        }
    }
    else
    {
        // Using direct textures, just update normally
        if (pbos[index] != nullptr && pbos[index]->isValid())
        {
            pbos[index]->update();
        }
    }
}

// -------------------------------------------------
ofPixels &ofxMultiPbo::getPixels(int index)
{
    if (index < 0 || index >= pbos.size())
    {
        ofLogError("ofxMultiPbo") << "PBO index out of range: " << index;
        return dummyPixels;
    }
    return pbos[index]->getPixels();
}

// -------------------------------------------------
ofFloatPixels &ofxMultiPbo::getFloatPixels(int index)
{
    if (index < 0 || index >= pbos.size())
    {
        ofLogError("ofxMultiPbo") << "PBO index out of range: " << index;
        return dummyFloatPixels;
    }
    return pbos[index]->getFloatPixels();
}
