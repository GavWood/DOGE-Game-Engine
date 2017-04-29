uniform sampler2D myTexture;

in vec4 myTex;
in vec4 myColour;
out vec4 fragColor;

void main(void)
{
	vec4 pixel_color = texture2DProj( myTexture, myTex );
	
	pixel_color.rgb = 0.0f;
	pixel_color.a = pixel_color.a * 0.5f;

    fragColor = pixel_color * myColour;
}
