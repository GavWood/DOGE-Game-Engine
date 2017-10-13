precision mediump float;

uniform sampler2D myTexture;

varying vec4 myTex;
varying vec4 myColour;

void main(void)
{
	vec4 pixel_color = texture2DProj( myTexture, myTex );
	
	pixel_color.rgb = 0.0f;
	pixel_color.a = pixel_color.a * 0.5;

    gl_FragColor = pixel_color * myColour;
}
