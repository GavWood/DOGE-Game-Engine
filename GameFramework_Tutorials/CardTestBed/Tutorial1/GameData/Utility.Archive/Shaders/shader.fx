cbuffer params
{
	float4	 m_colour;
	float4x4 m_m4World;
	float4x4 m_m4WorldViewScreen;
	float4	 m_lightAmbient;
	float4	 m_lightDirection;
	float4x4 m_m4WorldViewInverseTranspose;
	float4x4 m_m4ViewInverseTranspose;
	float4x4 m_m4BoneMatrices[10];
}

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
	float4 Colour : TEXCOORD2;
};

VS_OUTPUT shaderPassThrough( float4 inPos : POSITION,
		   				     float3 normal : NORMAL,
						     float4 colour : COLOR,
						     float2 inTexCoord : TEXCOORD
						   )
{
    VS_OUTPUT output;

    output.Pos = inPos;
    output.TexCoord = inTexCoord;
	output.Colour = m_colour * colour;
    return output;
}

VS_OUTPUT shaderTextured( float4 inPos : POSITION,
		   				  float3 normal : NORMAL,
						  float4 colour : COLOR,
						  float2 inTexCoord : TEXCOORD
						)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, m_m4WorldViewScreen);
    output.TexCoord = inTexCoord;
	output.Colour = m_colour * colour;
    return output;
}

VS_OUTPUT shaderLit( float4 inPos : POSITION,
		   			 float3 normal : NORMAL,
					 float4 colour : COLOR,
					 float2 inTexCoord : TEXCOORD
				   )
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, m_m4WorldViewScreen);

	float3 v3Normal  = mul( normal, m_m4WorldViewInverseTranspose );
	
	float3 lightDir  = mul( m_lightDirection, m_m4ViewInverseTranspose );
	lightDir = normalize( lightDir);

	// Calculate the incidence of the normal of the triangle to the light direction in camera space
	float NdotL = max( dot(v3Normal, lightDir), 0.0);
	
	// Compute the diffuse term
	float4 myColour = NdotL * colour;
	myColour += m_lightAmbient;
	myColour.a = 1.0;

    output.TexCoord = inTexCoord;

	output.Colour = m_colour * colour * myColour;
    return output;
}

VS_OUTPUT shaderSkin( float4 inPos : POSITION,
		   			  float3 normal : NORMAL,
					  float4 colour : COLOR,
					  float2 inTexCoord : TEXCOORD,
					  float3 tangent : TANGENT,
					  float3 binormal : BINORMAL,				  
					  float4 weights : BLENDWEIGHTS,
					  uint4  boneIndices : BLENDINDICES
				   )
{
    VS_OUTPUT output;

	float4 v4Pos = float4( 0, 0, 0, 1 );

	for ( int iBone = 0; iBone < 4; ++iBone )
	{
		uint iMat = boneIndices[iBone];

		float3 bonePosition3 = mul( inPos, m_m4BoneMatrices[ iMat ] ).xyz * weights[iBone];
		v4Pos.xyz += bonePosition3;
	}

	output.Pos = mul(v4Pos, m_m4WorldViewScreen);

	float3 v3Normal  = mul( normal, m_m4WorldViewInverseTranspose );
	
	float3 lightDir  = mul( m_lightDirection, m_m4ViewInverseTranspose );
	lightDir = normalize( lightDir);

	// Calculate the incidence of the normal of the triangle to the light direction in camera space
	float NdotL = max( dot(v3Normal, lightDir), 0.0);
	
	// Compute the diffuse term
	float4 myColour = NdotL * colour;
	myColour += m_lightAmbient;
	myColour.a = 1.0;

    output.TexCoord = inTexCoord;

	output.Colour = m_colour * colour * myColour;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float4 colour = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
	return colour * input.Colour;
}


struct VS_OUTPUT2
{
	float4 Pos : SV_POSITION;
	float4 TexCoord : TEXCOORD1;
	float4 Colour : TEXCOORD2;
};

float4 PS2(VS_OUTPUT2 input) : SV_TARGET
{
	float2 texCoord = input.TexCoord.xy /  input.TexCoord.w;
    float4 colour = ObjTexture.Sample( ObjSamplerState, texCoord );
	return colour * input.Colour;
}

VS_OUTPUT2 shaderProjected( float4 inPos : POSITION,
		   				    float3 normal : NORMAL,
						    float4 colour : COLOR,
						    float2 inTexCoord : TEXCOORD
					      )
{
    VS_OUTPUT2 output;

    output.Pos = mul( inPos, m_m4WorldViewScreen );
	output.TexCoord = mul( inPos, m_m4BoneMatrices[0] );
	output.Colour = float4(0, 0, 0, 1 );
    return output;
}
