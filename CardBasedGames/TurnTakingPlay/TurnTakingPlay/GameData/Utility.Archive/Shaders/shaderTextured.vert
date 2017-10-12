attribute vec4 position;
attribute vec4 normal;
attribute vec4 colour;
attribute vec4 tex0;

uniform vec4 s_lightAmbient;
uniform vec4 s_lightDirection;
uniform mat4 s_m4WorldViewScreen;
uniform mat4 s_m4WorldViewInverseTranspose;
uniform mat4 s_m4ViewInverseTranspose;

varying vec2 myTex;
varying vec4 myColour;

void main(void)
{
	gl_Position = s_m4WorldViewScreen * position;
	myColour = colour;
	myTex =  tex0.xy;
}
