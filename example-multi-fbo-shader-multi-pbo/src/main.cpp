#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main()
{

    // ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);

    ofGLFWWindowSettings winSettings;
    winSettings.setGLVersion(3, 2);
    winSettings.numSamples = 8;
    winSettings.setSize(800, 600);

    shared_ptr<ofAppBaseWindow> win = ofCreateWindow(winSettings); // sets up the opengl context!
    ((ofAppGLFWWindow *)win.get())->setMultiDisplayFullscreen(true);

    // 10 x 1 screens
    // ofSetupOpenGL(&win, w, h, OF_WINDOW /*OF_FULLSCREEN*/);	// <-------- setup the GL context

    ofRunApp(win, shared_ptr<ofBaseApp>(new ofApp()));
    ofRunMainLoop();
}
