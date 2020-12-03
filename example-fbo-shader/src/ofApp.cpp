#include "ofApp.h"

void ofApp::setup() {

	// Setup the fbo shader (renderer)
	renderer.setup(100, 100, GL_RGBA, 5, ofColor(255), false, "shaders/myShader", "inTex");

	// Initialize fbos with whatever information you need
	ofSetCircleResolution(32);
	// Here, we'll draw a blue circle on a white background at buffer index 2
	renderer.beginFbo(2);
	ofSetColor(0, 0, 255, 255);
	ofDrawCircle(50, 50, 50);
	renderer.endFbo(2);
	// ... And a black circle on a transparent background at buffer index 3
	renderer.beginFbo(3);
	ofClear(0, 0, 0, 0);
	ofSetColor(255, 255, 255, 255);
	ofDrawCircle(50, 50, 50);
	renderer.endFbo(3);
	// When individually writing to buffers, remember to end all at once or call swap
	renderer.endFbo();
}

void ofApp::update() {

	// Begin rendering
	renderer.begin(true, ofColor(0,0,0,0));
	
	// Pass any uniforms you need in the shader
	renderer.setUniform1f("time", ofGetElapsedTimef());
	renderer.setUniform1f("width", renderer.width());
	renderer.setUniform1f("height", renderer.height());

	// End rendering
	renderer.end();
}

void ofApp::draw(){

	// First texture should be all red
	renderer.getTex(0).draw(0, 0, 100, 100);

	// Second texture should be a red/yellow/black gradient
	renderer.getTex(1).draw(100, 0, 100, 100);

	// Third texture is a blue circle
	renderer.getTex(2).draw(200, 0, 100, 100);

	// Fourth texture is a gradient circle on transparent background
	renderer.getTex(3).draw(300, 0, 100, 100);

	// Fifth texture is an animated gradient circle on transparent background
	renderer.getTex(4).draw(400, 0, 100, 100);
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){
	
}

