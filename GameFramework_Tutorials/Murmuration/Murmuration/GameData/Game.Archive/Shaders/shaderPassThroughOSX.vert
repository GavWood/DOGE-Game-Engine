in vec4 position;
in vec4 colour;
in vec4 tex0;

out vec2 myTex;
out vec4 myColour;

void main(void)
{
	gl_Position    = position;
	myTex		   = tex0.xy;
	myColour       = colour;
}
