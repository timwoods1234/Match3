
// Generic light-texture effect

struct D3DMATERIAL9 {
    float4   diffuse;
    float4   ambient;
    float4   spec;
    float4   emissive;
    float    specPower;
};

uniform extern texture  	gTex;
uniform extern float4x4  	gWVP;
uniform extern float4x4  	gWorld;
uniform extern float3    	gEyePosW;
uniform extern D3DMATERIAL9 	gMtrl;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP; 
    AddressV  = WRAP;
};

void LitTexVS(float3 posL         : POSITION0,
              float3 normalL      : NORMAL0,
              float2 tex0         : TEXCOORD0,
              out float4 oPosH    : POSITION0,
              out float3 oPosW    : TEXCOORD0,
              out float3 oToEyeW  : TEXCOORD2,
              out float2 oTex0    : TEXCOORD3)
{
	oPosH = mul(float4(posL, 1.0f), gWVP);
	oPosW = mul(float4(posL, 1.0f), gWorld).xyz;

	oToEyeW = gEyePosW - oPosW;
	
	oTex0 = tex0;
}

float4 LitTexPS(float3 posW    : TEXCOORD0,
                float3 normalW : TEXCOORD1,
                float3 toEyeW  : TEXCOORD2,
                float2 tex0    : TEXCOORD3) : COLOR
{
	toEyeW  = normalize(toEyeW);

	float3 spec = gMtrl.spec.rgb;
	float3 diffuse = gMtrl.diffuse;
	float3 ambient = gMtrl.ambient;

	float4 texColor = tex2D(TexS, tex0); 

	float3 litColor = ambient*texColor.rgb + (diffuse*texColor.rgb + spec);

	return float4(litColor, gMtrl.diffuse.a*texColor.a);
}

technique LitTex
{
    pass P0
    {
        vertexShader = compile vs_2_0 LitTexVS();
        pixelShader  = compile ps_2_0 LitTexPS();
    }
}

