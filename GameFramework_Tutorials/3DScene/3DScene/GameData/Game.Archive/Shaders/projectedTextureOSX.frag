uniform sampler2D myTexture;
uniform sampler2D myTexture2;

varying vec2 myTex;
varying vec4 myColour;
varying vec4 lightAmbient;
varying vec4 myShadowTex;

void main(void)
{
	vec4 colour = myColour;

	vec4 pixel_color1 = texture2D( myTexture, myTex );
	vec4 pixel_color2 = texture2DProj( myTexture2, myShadowTex );				// non shadowed 0, 0, 0, 0, or shadowed 0, 0, 0, intensity

	colour.xyz -= pixel_color2.a;

    gl_FragColor = pixel_color1 * colour;
}
