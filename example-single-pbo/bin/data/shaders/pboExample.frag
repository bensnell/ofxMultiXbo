#version 150

// Define uniform at global scope
uniform vec2 canvasDims;
uniform float timeSec;

out vec4 outputColor;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
    // Use the uniform values instead of hardcoded dimensions
	float r = gl_FragCoord.x / canvasDims.x;
	float g = gl_FragCoord.y / canvasDims.y;
	float b = 0.5 + 0.5 * sin(timeSec * 2.0); // Oscillate blue value between 0 and 1 using sine wave
	float a = 1.0;
	outputColor = vec4(r, g, b, a);
}
