precision mediump float;

uniform sampler2D myTexture;

varying vec2 myTex;
varying vec4 myColour;

void main(void)
{
	vec4 pixel_color = texture2D( myTexture, myTex );

    gl_FragColor = pixel_color * myColour;
}
