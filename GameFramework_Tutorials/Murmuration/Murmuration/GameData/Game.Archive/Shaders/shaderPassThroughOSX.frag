uniform sampler2D myTexture;

in vec2 myTex;
in vec4 myColour;
out vec4 fragColor;

void main(void)
{
	fragColor = myColour * texture(myTexture, myTex);
}
