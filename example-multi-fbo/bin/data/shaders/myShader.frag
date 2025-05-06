#version 150
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

// ============================================================================
// =============================== P A R A M S ================================
// ============================================================================

// -------------------------------- I N P U T ---------------------------------

in vec2 texCoordVarying;

// ----------------------------- U N I F O R M S ------------------------------

uniform sampler2DRect inTex0;
uniform sampler2DRect inTex1;
uniform sampler2DRect inTex2;
uniform sampler2DRect inTex3;
uniform sampler2DRect inTex4;

uniform float time;
uniform float width;
uniform float height;

// ------------------------------ O U T P U T --------------------------------

layout (location = 0) out vec4 outTex0;
layout (location = 1) out vec4 outTex1;
layout (location = 2) out vec4 outTex2;
layout (location = 3) out vec4 outTex3;
layout (location = 4) out vec4 outTex4;



// ============================================================================
// ================================ U T I L S =================================
// ============================================================================

// ----------------------------- G E N E R A L -------------------------------
float wrap(float a) {
	return (int(a)%2 == 0) ? (a-int(a)) : (1-(a-int(a)));
}

// --------------------------- E N C O D I N G S -----------------------------


// ------------------------------- D E B U G ---------------------------------


// ============================================================================
// ================================= M A I N ==================================
// ============================================================================

void main() {
	
	outTex0 = vec4(1,0,0,1);
    outTex1 = vec4(texCoordVarying.x/width,texCoordVarying.y/height,0,1);
    outTex2 = texture(inTex2, texCoordVarying);
	outTex3 = texture(inTex3, texCoordVarying).a == 0 ? vec4(0,0,0,0) : vec4(0, texCoordVarying.x/width,texCoordVarying.y/height,1);
	outTex4 = texture(inTex3, texCoordVarying).a == 0 ? vec4(0,0,0,0) : vec4(wrap(time), wrap(time*0.5+texCoordVarying.x/width), wrap(time+texCoordVarying.y/height),1);

}