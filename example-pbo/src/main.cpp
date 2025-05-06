#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main()
{
    // Setup OpenGL context with GL 3.2
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.setSize(1000, 1000);

    auto window = ofCreateWindow(settings);

    // Run the app
    ofRunApp(window, make_shared<ofApp>());
    ofRunMainLoop();
}