# ofxMultiXbo

Utilities for Manipulating Multiple <u>X</u> Buffer Objects (<u>F</u>BOs and <u>P</u>BOs)

## Table of Contents

- [Project Details](#Project-Details)
- [Installation](#installation)
- [Usage](#usage)
- [Troubleshooting](#troubleshooting)
- [Roadmap](#roadmap)
- [License](#license)
- [Notes](#notes)

## Project Details

There is no object called `ofxMultiXbo`. Rather, it represents the collection of other buffer objects, which are individually described in detail below:

### ofxMultiFbo

This is a helper class for handling frame buffer objects (FBOs) with multiple texture targets. It is useful in combination with shaders that output multiple textures.

Internally, it contains two FBOs for every one allocated. These two FBOs are used to ping pong textures back and forth, so that one can be written to (the utility FBO) while the other is read from (the data FBO). Between `fbo.begin()` and `fbo.end()` calls, the texture `fbo.getTex(i)` contains the last rendered texture (this is analogous to `fbo.getDataFbo()`). After the call to `fbo.end()`, textures are swapped and `fbo.getTex(i)` contains the just-rendered texture (this is analogous to `fbo.getDataFbo()`).

_Note: This object is limited to 8 attached buffer objects on a single ofxMultiFbo instance._

An example of usage includes:

**_main.cpp_**

_Note: Your main.cpp file needs to be set up like this, enabling support for GL version 3.2_

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

**_ofApp.h_**

```c++
class ofApp : public ofBaseApp {
    ofxMultiFbo fbo;
	ofShader shader;
}
```

**_ofApp.cpp_**

```c++
ofApp::Setup() {

    // Allocate a frame buffer consisting of two RGBA textures
	// at 100 x 100 pixels initialized with the color white
	fbo.allocate(100, 100, GL_RGBA, 2, ofColor(255));

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

**_myShader.frag_**

_Note: Your fragment shader must have this header to specify its version and a number of extensions required for operation. Every texture generated within ofxMultiFbo will be accessible in order as an output. The name `outTex0` can be user-specified within the shader._

```c
#version 150
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

in vec2 texCoordVarying;

layout (location = 0) out vec4 outTex0;	// First output texture
layout (location = 1) out vec4 outTex1;	// Second output texture

void main() {

    outTex0 = vec4(1,0,0,1);
    outTex1 = vec4(texCoordVarying.x/100.0,texCoordVarying.y/100.0,0,1);
}
```

**_myShader.vert_**

_Note: You must also include the header version in the vertex shader._

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

### ofxMultiFboShader

This class extends `ofxMultiFbo` by integrating an `ofShader`. It simplifies the process of using a shader with multiple render targets by automatically managing the FBOs and textures.

The class handles:

- Setting up the FBOs and shader in one step
- Managing texture name assignments for the shader uniforms
- Automatically passing textures to the shader when beginning rendering
- Handling the shader execution and FBO buffer swapping

Example usage:

```c++
// Setup a multi-buffer shader FBO with 2 RGBA buffers and a shader
renderer.setup(width, height, GL_RGBA, 2, ofColor(0),
               "shaders/myShader", "tex");

// Begin rendering (clear with black and begin the shader)
renderer.begin(true, ofColor(0));

// Set any additional shader uniforms
renderer.setUniform1f("time", ofGetElapsedTimef());

// End rendering (automatically runs the shader and swaps buffers)
renderer.end();

// Access the rendered textures
renderer.getTex(0).draw(0, 0, width/2, height/2);
renderer.getTex(1).draw(width/2, 0, width/2, height/2);
```

### ofxMultiPbo

This class facilitates efficient GPU-to-CPU texture data transfer for multiple textures using Pixel Buffer Objects (PBOs). It supports both standard byte-format textures and floating-point textures.

The class manages double-buffered PBOs to allow asynchronous data transfer, which avoids GPU stalls and improves performance. It can work directly with specified textures or with an `ofxMultiFbo` object.

Key features:

- Supports both standard (byte) and floating-point texture formats
- Double-buffered design for optimal GPU-CPU transfer
- Direct integration with `ofxMultiFbo` for easy texture access
- Thread-safe pixel data access

Example usage with an ofxMultiFbo:

```c++
// Set up FBO with multiple buffers
fbo.allocate(512, 512, GL_RGB, 3, ofColor(0));

// Set up PBO to read from the DATA_FBO (most recent rendered frame)
pbo.setup(&fbo, ofxMultiPbo::DATA_FBO);

// In update():
// Render to the FBO...
// ...
// Update PBO with latest texture data
pbo.update();

// Access pixel data for processing or display
ofImage image;
image.setFromPixels(pbo.getPixels(0));
image.update();
```

Example usage with direct textures:

```c++
// Create textures
vector<ofTexture*> textures;
textures.push_back(&texture1);
textures.push_back(&texture2);

// Set up PBO for these textures
pbo.setup(textures);

// In update():
// Update PBO (reads the current texture data)
pbo.update();
// Or update a specific texture's PBO
pbo.update(1); // Updates only the second texture

// Get pixel data
ofPixels& pixels = pbo.getPixels(0);
// For float textures
ofFloatPixels& floatPixels = pbo.getFloatPixels(1);
```

### ofxSinglePbo

This is a lower-level class used by `ofxMultiPbo` to manage a single PBO for one texture. It handles the technical details of PBO creation, mapping, and data transfer. Most users will interact with `ofxMultiPbo` rather than this class directly.

Key features:

- Handles various GL texture formats including float formats
- Double-buffered design for non-blocking texture reads
- Thread-safe data access

#### Dependencies & Frameworks

- OpenGL Version 3.2

## Installation

1. Clone this repository into your openFrameworks/addons folder:

   ```
   cd openFrameworks/addons
   git clone https://github.com/username/ofxMultiXbo.git
   ```

2. Include the addon in your project:

   - For Project Generator: Add ofxMultiXbo to your addons
   - For manual projects: Add the addon to your include paths and link against the necessary libraries

3. Make sure your project is set up to use OpenGL 3.2 or higher. See the main.cpp example above.

## Usage

The best way to understand how to use the library is to explore the included examples:

- **example-multi-fbo**: Basic usage of ofxMultiFbo for multi-target rendering
- **example-multi-fbo-shader**: Using ofxMultiFboShader for simplified shader-based rendering
- **example-single-pbo**: Basic PBO usage with individual textures
- **example-multi-fbo-multi-pbo**: Example showing integration of ofxMultiFbo with ofxMultiPbo
- **example-multi-fbo-shader-multi-pbo**: Complete example showing integration of ofxMultiFboShader with ofxMultiPbo

## Troubleshooting

- **Black textures**: Ensure that PBOs are properly unbound after use. The ofxSinglePbo class handles this automatically, but if you're working with OpenGL directly, remember to unbind your PBOs.

- **GL Errors**: Check that you're properly setting up OpenGL 3.2 context in your main.cpp file.

- **Performance issues**: When using PBOs, remember that they work most efficiently when used in a double-buffered fashion, as implemented in this library. If you're still experiencing performance issues, check if your texture formats and sizes are optimal.

## Roadmap

### TODO:

- [x] How to load data into fbo
- [x] How to draw into fbo
- [x] How to use with shaders -- what shader settings to use, etc.
- [x] How to retrieve data from fbo (pbo)
- [x] Package a shader into the fbo (via ofxMultiFboShader)
- [ ] Add support for compute shaders
- [x] Improve documentation and add more examples

## License

MIT License - See LICENSE file for details.

---

### Notes

For questions and contributions, please open an issue or pull request on the repository.
