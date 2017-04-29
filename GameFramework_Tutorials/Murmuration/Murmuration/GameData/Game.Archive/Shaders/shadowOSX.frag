uniform sampler2D myTexture;

in vec2 myTex;
in vec4 myColour;
out vec4 fragColor;

void main(void)
{
	vec4 pixel_color = texture(myTexture, myTex);

    fragColor = pixel_color *  myColour;
}
