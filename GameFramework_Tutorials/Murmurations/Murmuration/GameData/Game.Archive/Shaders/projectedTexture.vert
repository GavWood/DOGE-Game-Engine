attribute vec4 position;
attribute vec4 normal;
attribute vec4 colour;
attribute vec4 tex0;

uniform mat4 s_m4ProjectionTexture;
uniform vec4 s_lightAmbient;
uniform vec4 s_lightDirection;
uniform mat4 s_m4WorldViewScreen;
uniform mat4 s_m4WorldViewInverseTranspose;
uniform mat4 s_m4ViewInverseTranspose;
uniform mat4 s_m4World;

varying vec4 myTex;
varying vec4 myColour;

//  output.v4Pos    = mul( input.v4Position, s_m4WorldViewScreen );
//	float4 v4NewPosition = mul( input.v4Position, s_m4World );
//  output.v4TexPos = mul( v4NewPosition, s_m4ProjectionTexture );

void main(void)
{
	gl_Position = s_m4WorldViewScreen * position;

	vec4 v4NewPosition = s_m4World * position;
	vec4 v4TexPos = s_m4ProjectionTexture * v4NewPosition;

	myColour = colour;
	myTex =  v4TexPos;
}
