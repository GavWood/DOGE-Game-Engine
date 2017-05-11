precision mediump float;

uniform sampler2D myTexture;

varying vec2 myTex;
varying vec4 myColour;

float near = 1.0; 
float far  = 100.0; 
  
void main(void)
{
	gl_FragColor = myColour * texture2D(myTexture, myTex);
}
