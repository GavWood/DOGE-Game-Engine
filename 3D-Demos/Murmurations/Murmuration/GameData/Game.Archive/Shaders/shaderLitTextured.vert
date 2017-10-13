attribute vec4 position;
attribute vec4 normal;
attribute vec4 colour;
attribute vec4 tex0;

uniform vec4 s_lightAmbient;
uniform vec4 s_lightDirection;
uniform mat4 s_m4WorldViewScreen;
uniform mat4 s_m4WorldViewInverseTranspose;
uniform mat4 s_m4ViewInverseTranspose;

varying vec2 myTex;
varying vec4 myColour;

void main(void)
{
	gl_Position = s_m4WorldViewScreen * position;

	myTex = tex0.xy;

	vec4 normal4  = s_m4WorldViewInverseTranspose * normal;
	vec3 myNormal = normalize( normal4.xyz );

	vec4 lightDir4  = s_m4ViewInverseTranspose * s_lightDirection;
	vec3 lightDir   = normalize( lightDir4.xyz );

	// Calculate the incidence of the normal of the triangle to the light direction in camera space
	float NdotL = max(dot(myNormal, lightDir), 0.0);
	
	// Compute the diffuse term
	myColour = NdotL * colour;
	myColour += s_lightAmbient;
	myColour.a = colour.a;
}
