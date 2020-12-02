# ofxMultiXbo
Utilities for Manipulating Multiple <u>X</u> Buffer Objects (<u>F</u>BOs and <u>P</u>BOs)

## Table of Contents

* [Project Details](#Project-Details)
* [Installation](#installation)
* [Usage](#usage)
* [Troubleshooting](#usage)
* [Roadmap](#roadmap)
* [License](#license)
* [Notes](#notes)

## Project Details

There is no object called `ofxMultiXbo`. Rather, it represents the collection of other buffer objects, which are individually described in detail below:

### ofxMultiFbo

This is a helper class for handling frame buffer objects (FBOs) with multiple texture targets. It is useful in combination with shaders that output multiple textures. 

Internally, it contains two FBOs for every one allocated. These two FBOs are used to ping pong textures back and forth, so that one can be written to (the utility FBO) while the other is read from (the data FBO). Between `fbo.begin()` and `fbo.end()` calls, the texture `fbo.getTex(i)` contains the last rendered texture (this is analogous to `fbo.getDataFbo()`). After the call to `fbo.end()`, textures are swapped and `fbo.getTex(i)` contains the just-rendered texture (this is analogous to `fbo.getDataFbo()`). 

*Note: This object is limited to 8 attached buffer objects on a single ofxMultiFbo instance.*

An example of usage includes:

***main.cpp*** 

*Note: Your main.cpp file needs to be set up like this, enabling support for GL version 3.2*

```c++
int main() {

	ofGLFWWindowSettings winSettings;
	winSettings.setGLVersion(3, 2);
	winSettings.numSamples = 8;
	winSettings.setSize(1920, 1080);

	shared_ptr<ofAppBaseWindow> win = ofCreateWindow(winSettings);	// sets up the opengl context!
	((ofAppGLFWWindow*)win.get())->setMultiDisplayFullscreen(true);

	ofRunApp(win, shared_ptr<ofBaseApp>(new ofApp()));
	ofRunMainLoop();
}
```

***ofApp.h***

```c++
class ofApp : public ofBaseApp {
    ofxMultiFbo fbo;
	ofShader shader;
}
```

***ofApp.cpp***

```c++
ofApp::Setup() {
	
    // Allocate a frame buffer consisting of two RGBA textures 
	// at 100 x 100 pixels initialized with the color white
	// and without PBO support.
	fbo.allocate(100, 100, GL_RGBA, 2, ofColor(255), false);

	// Load Shader
	shader.load("shaders/myShader");
}

ofApp::Update() {
    
    // Run the shader and save its two outputs to the two texture
    // targets in fbo
	fbo.begin();
	ofClear(0,0,0,0); // Clear the textures
	shader.begin();
	shader.setUniformTexture("tex0", fbo.getTex(0), 0);
	shader.setUniformTexture("tex1", fbo.getTex(1), 1);
	fbo.getTex(0).draw(0,0); // Call the shader to run
	shader.end();
	fbo.end();
}

ofApp::Draw() {
    
    // Draw the two just-rendered textures.
    fbo.getTex(0).draw(0,0,100,100);
    fbo.getTex(1).draw(100,0,100,100);
}
```

***myShader.frag*** 

*Note: Your fragment shader must have this header to specify its version and a numberof extensions required for operation. Every texture generated within ofxMultiFbo will be accessible in order as an output. The name `outTex0` can be user-specified within the shader.* 

```c
#version 150
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

in vec2 texCoordVarying;

layout (location = 0) out vec4 outTex0;	// First output texture
layout (location = 1) out vec4 outTex1;	// Second output texture

void main() {
    
    outTex0 = vec4(1,0,0,1);
    outTex1 = vecr(texCoordVarying.x/100.0,texCoordVarying.y/100.0,0,1);
}
```

***myShader.vert***

*Note: You must also include the header version in the vertex shader.*

```c
#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;

void main()
{
    texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}
```

### ofxMultiPbo

xxx

#### Resources

xxx

#### Dependencies & Frameworks

- OpenGL Version 3.2

## Installation

xxx

## Usage

xxx

## Troubleshooting

xxx

## Roadmap

### TODO:

- [x] How to load data into fbo

- [x] How to draw into fbo

- [x] How to use with shaders -- what shader settings to use, etc.

- [x] How to retrieve data from fbo (pbo)

- [ ] Package a shader into the fbo (multiShader?)?

## License

xxx

---

### Notes

(1): Any additional footnotes or [links](https://example.com) can be added here.