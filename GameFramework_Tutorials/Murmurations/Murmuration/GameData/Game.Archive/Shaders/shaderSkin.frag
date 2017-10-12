precision mediump float;

uniform sampler2D myTexture;

varying vec2 myTex;
varying vec4 myColour;

void main(void)
{
	gl_FragColor = myColour * texture2D(myTexture, myTex);
}
