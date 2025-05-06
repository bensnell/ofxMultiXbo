#include "ofApp.h"

void ofApp::setup() {

	// Allocate all buffers within the fbo
	fbo.allocate(100, 100, GL_RGBA, 5, ofColor(255), false);

	// Load the shader from file
	shader.load("shaders/myShader");

	// Initialize fbos with whatever information you need
	ofSetCircleResolution(32);
	// Here, we'll draw a blue circle on a white background at buffer index 2
	fbo.begin(2);
	ofSetColor(0, 0, 255, 255);
	ofDrawCircle(50, 50, 50);
	fbo.end(2);
	// ... And a black circle on a transparent background at buffer index 3
	fbo.begin(3);
	ofClear(0, 0, 0, 0);
	ofSetColor(255, 255, 255, 255);
	ofDrawCircle(50, 50, 50);
	fbo.end(3);
	// When individually writing to buffers, remember to end all at once or call swap
	fbo.end(); // or fbo.swap()
}

void ofApp::update() {

	// Begin rendering to the fbo
	fbo.begin();
	// Clear the current buffers in order to have a clean slate to render to
	ofClear(0, 0, 0, 0);
	// Begin the shader
	shader.begin();

	// Pass to the shader all textures from the fbo as inputs
	shader.setUniformTexture("inTex0", fbo.getTex(0), 0);
	shader.setUniformTexture("inTex1", fbo.getTex(1), 1);
	shader.setUniformTexture("inTex2", fbo.getTex(2), 2);
	shader.setUniformTexture("inTex3", fbo.getTex(3), 3);
	shader.setUniformTexture("inTex4", fbo.getTex(4), 4);

	// Pass any uniforms you need in the shader
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniform1f("width", fbo.width());
	shader.setUniform1f("height", fbo.height());

	// Call the shader to run by drawing the first texture
	fbo.getTex(0).draw(0, 0);

	// End the shader
	shader.end();
	// End the fbo (this will also swap textures automatically)
	fbo.end();
}

void ofApp::draw(){

	// First texture should be all red
	fbo.getTex(0).draw(0, 0, 100, 100);

	// Second texture should be a red/yellow/black gradient
	fbo.getTex(1).draw(100, 0, 100, 100);

	// Third texture is a blue circle
	fbo.getTex(2).draw(200, 0, 100, 100);

	// Fourth texture is a gradient circle on transparent background
	fbo.getTex(3).draw(300, 0, 100, 100);

	// Fifth texture is an animated gradient circle on transparent background
	fbo.getTex(4).draw(400, 0, 100, 100);
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

