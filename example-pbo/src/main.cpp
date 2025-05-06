#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	// Setup OpenGL context with GL 3.2
	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1024, 768);
	
	auto window = ofCreateWindow(settings);
	
	// Run the app
	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}