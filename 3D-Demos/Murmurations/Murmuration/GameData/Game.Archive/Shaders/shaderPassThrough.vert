attribute vec4 position;
attribute vec4 colour;
attribute vec4 tex0;

varying vec2 myTex;
varying vec4 myColour;

void main(void)
{
	gl_Position    = position;
	myTex		   = tex0.xy;
	myColour       = colour;
}
