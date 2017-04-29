in vec4 position;
in vec4 normal;
in vec4 colour;
in vec4 tex0;

uniform vec4 s_lightAmbient;
uniform vec4 s_lightDirection;
uniform mat4 s_m4WorldViewScreen;
uniform mat4 s_m4WorldViewInverseTranspose;
uniform mat4 s_m4ViewInverseTranspose;

out vec2 myTex;
out vec4 myColour;

void main(void)
{
	gl_Position = s_m4WorldViewScreen * position;
	myColour = colour;
	myTex =  tex0.xy;
}
