//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;
float4x4 gMatrixViewInverse : VIEWINVERSE;

float3 gLightDirection : DIRECTION
<
string UIName = "Light Direction";
string Object = "TargetLight";
> = float3(-0.577f, -0.577f, 0.577f);

float3 gEyePosW;

//AMBIENT
float4 gColorAmbient : COLOR = float4(1, 1, 1, 1);

float gAmbientIntensity
<
string UIName = "Ambient Intensity";
string UIWidget = "Slider";
float UIMin = 0.0;
float UIMax = 1.0;
float UIStep = 0.01;
> = 0.0f;

//DIFFUSE
//decide whether to use texture or color
bool gUseTextureDiffuse
<
string UIName = "Diffuse Texture";
string UIWidget = "Bool";
> = true;

float4 gColorDiffuse : COLOR = float4(0.9, 0.9, 0.1, 1.0);

Texture2D gDiffuseTexture
<
string UIName = "Diffuse Texture";
string ResourceType = "Texture";
>;

//SPECULAR
bool gUseTextureSpecular
<
string UIName = "Specular Texture";
string UIWidget = "Bool";
> = false;

float4 gColorSpecular : COLOR = float4(1.0, 1.0, 1.0, 1.0);

int gShininess : INT = 50;

float gSpecularIntensity = 1.0f;

Texture2D gSpecularTexture
<
string UIName = "Specular Texture";
string ResourceType = "2D";
>;

bool gUseBlinn 
<
string UIName = "Use Blinn";
string UIWidget = "Bool";
> = true;

bool gUsePhong
<
string UIName = "Use Phong";
string UIWidget = "Bool";
> = true;

//NORMAL
bool gUseNormalTexture
<
string UIName = "Normal Texture";
string UIWidget = "Bool";
> = true;

bool gFlip
<
string UIName = "Flip";
string UIWidget = "Bool";
> = false;

Texture2D gNormalTexture
<
string UIName = "Normal Map";
string ResourceType = "2D";
>;

//ENVIRONMENT MAPPING
bool gUseEnvironmentMapping
<
string UIName = "Environment Mapping";
string UIWidget = "Bool";
> = false;

TextureCube gTextureEnv
<
string UIName = "Environment Map";
string ResourceType = "Cube";
>;

float gReflectionStrength : FLOAT = 0.f;
float gRefractionStrength : FLOAT = 0.f;
float gRefractionIndex : FLOAT =  0.3f;

//FRESNEL
bool gUseFresnelFalloff
<
    string UIName = "Fresnel FallOff";
    string UIWidget = "Bool";
> = false;


float4 gColorFresnel : COLOR = float4(1,1,1,1);

float gFresnelPower : FLOAT = 1.0f;

float gFresnelMultiplier : FLOAT = 1.0f;

float gFresnelHardness : FLOAT = 0.f;

//OPACITY
float gOpacity : OPACITY = float(1.f);

bool gUseOpacityMap
<
    string UIName = "Opacity Map";
    string UIWidget = "Bool";
> = true;

Texture2D gOpacityTexture
<
    string UIName = "Opacity Texture";
    string UIWidget = "Texture";
>;

//INITIALIZE FUNCTIONS
float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord);
float3 CalculateSpecular(float3 viewDirection, float3 newNormal, float2 texCoord);
float3 CalculatePhong(float3 viewDirection, float3 newNormal, float specularIntensity);
float3 CalculateBlinn(float3 viewDirection, float3 newNormal, float specularIntensity);
float3 CalculateDiffuse(float3 newNormal, float2 texCoord);
float3 CalculateEnvironment(float3 viewDirection, float3 newNormal);
float3 CalculateFresnellFalloff(float3 newNormal, float3 viewDirection, float3 environmentColor);
float CalculateOpacity(float2 texCoord);

//STATES
//******
RasterizerState gRS_NoCulling { CullMode = NONE; };

RasterizerState gRS_FrontCulling { CullMode = FRONT; };

SamplerState gDiffuseSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState gNormalSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState gEnvMapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

BlendState gEnableBlending
{
	BlendEnable[0] = TRUE;
	DestBlend = INV_SRC_ALPHA;
	SrcBlend = SRC_ALPHA;
};

//IN/OUT Struct
struct VS_INPUT
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	float3 Tangent : TANGENT;
	float2 Lighting : COLOR1;
};	

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float4 WorldPosition : COLOR1;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	float3 Tangent : TANGENT;
	//float2 Lighting : COLOR1;
};	

//MAIN VERTEX SHADER
//******************
VS_OUTPUT MainVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Position = mul(float4(input.Position, 1.f), gMatrixWorldViewProj);
	output.WorldPosition = mul(float4(input.Position, 1.0f), gMatrixWorld);
	output.Normal = normalize(mul(input.Normal, (float3x3)gMatrixWorld));
	output.Color = float4(0.9, 0.9, 0.9, 1);
	output.Tangent = mul(normalize(input.Tangent), (float3x3) gMatrixWorldViewProj);
	output.TexCoord = input.TexCoord;
	//output.Lighting = input.Lighting;
	
	return output;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(VS_OUTPUT input) : SV_TARGET 
{
	//NORMALIZE
	float3 normal = normalize(input.Normal);
	float3 tangent = normalize(input.Tangent);
	float3 viewDirection = normalize(input.WorldPosition.xyz - gEyePosW);
	
	//NORMAL
	float3 newNormal = CalculateNormal(tangent, normal, input.TexCoord);
	
	//SPECULAR
	float3 specColor = CalculateSpecular(viewDirection, newNormal, input.TexCoord);
	
	//DIFFUSE
	float3 diffColor = CalculateDiffuse(newNormal, input.TexCoord);
	
	//AMBIENT
	float3 ambientColor = gColorAmbient * gAmbientIntensity;
	
	//ENVIRONMENT MAPPING
	float3 environmentColor = CalculateEnvironment(viewDirection, newNormal);
	
	//FRESNELL FALLOFF
	environmentColor = CalculateFresnellFalloff(newNormal, viewDirection, environmentColor);
	
	//FINAL COLOR CALCULATION
	float3 finalColor = diffColor + specColor + environmentColor + ambientColor;

	//OPACITY
	float opacity = CalculateOpacity(input.TexCoord);
	
	return float4(finalColor, opacity);
}

float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	float3 outputNormal = normal;
	
	if(gUseNormalTexture)
	{
		float3 sampledNormal = gNormalTexture.Sample(gNormalSampler, texCoord);
		sampledNormal *= 2 - float3(1.f, 1.f, 1.f);
		float3 binormal = cross(normal, tangent);
		if(gFlip)
			binormal *= -1;
		float3x3 localAxis = float3x3(tangent, binormal, normal);
		float3 newNormal = normalize(mul(sampledNormal, localAxis));
	}
	
	return outputNormal;
}

float3 CalculatePhong(float3 viewDirection, float3 normal, float specularIntensity)
{
	float3 reflected = reflect(-gLightDirection, normal);
	float specularStrength = dot(reflected, viewDirection);
	specularStrength = saturate(specularStrength);
	float phong = pow(specularStrength, gShininess);
	
	return gColorSpecular * phong * specularIntensity;	
}

float3 CalculateBlinn(float3 viewDirection, float3 normal, float specularIntensity)
{
	float3 halfVector = -normalize(viewDirection + gLightDirection); 
 	float specularStrength = dot(halfVector, normal); 
 	specularStrength = saturate(specularStrength); 
 	specularStrength = pow(specularStrength, gShininess);
	
	return gColorSpecular * specularStrength * specularIntensity;
}

float3 CalculateSpecular(float3 viewDirection, float3 normal, float2 texCoord)
{	
	float specularIntensity = gSpecularIntensity;
	if(gUseTextureSpecular)
		specularIntensity = gSpecularTexture.Sample(gDiffuseSampler, texCoord).r;
		
	float3 color = float3(0,0,0);
	
	if(gUseBlinn)
		color += CalculateBlinn(-viewDirection, normal, specularIntensity);

	if(gUsePhong)
		color += CalculatePhong(-viewDirection, normal, specularIntensity);
	
	return color;
}

float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	float diffuseStrength = dot(normal, -gLightDirection); 
	diffuseStrength = saturate(diffuseStrength);	
	float3 diffuseColor= gColorDiffuse * diffuseStrength;
	
	if(gUseTextureDiffuse)
	{
		diffuseColor *= gDiffuseTexture.Sample(gDiffuseSampler, texCoord);
	}
	
	return diffuseColor * diffuseStrength;
}

float3 CalculateEnvironment(float3 viewDirection, float3 normal)
{
	float3 environment = float3(0.f, 0.f, 0.f);
	
	if(gUseEnvironmentMapping)
	{
		float3 reflectedVector = reflect(normal, viewDirection);
		float3 refractedVector = refract(normal, viewDirection, gRefractionIndex);
		//float3 texCoord = normalize(reflectedVector).xyz;

		float3 reflection = gTextureEnv.Sample(gEnvMapSampler, reflectedVector) * gReflectionStrength;
		float3 refraction = gTextureEnv.Sample(gEnvMapSampler, refractedVector) * gRefractionStrength;
	
		environment = reflection + refraction;
	}
	
	return environment;
}

float3 CalculateFresnellFalloff(float3 normal, float3 viewDirection, float3 environmentColor)
{
	if(gUseFresnelFalloff)
	{
		float fresnel = pow(1 - saturate(abs(dot(normal,viewDirection))), gFresnelPower) * gFresnelMultiplier;
		float fresnelMask = pow((1 - saturate(float3(0, -1, 0) * normal)), gFresnelHardness);
		return fresnel*fresnelMask*environmentColor;
	}
	return float3(0,0,0);
}

float CalculateOpacity(float2 texCoord)
{
	float opacity = gOpacity;
	
	if(gUseOpacityMap) 
 	{
		opacity = gOpacityTexture.Sample(gDiffuseSampler, texCoord).r; 
 	}

	return gOpacity;
}

//TECHNIQUES
//**********
technique11 DefaultTechnique {
	pass p0 {
		SetRasterizerState(gRS_NoCulling);	
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}

technique11 LightTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}

//technique10 WithAlphaBlending {
	//pass p0 {
		//SetRasterizerState(gRS_NoCulling);	
 		//SetVertexShader(EnableBlending,
		//				float4(0.0f, 0.0f, 0.0f, 0.0f),
		//				0xFFFFFFFF);
		//SetVertexShader(CompileShader(vs_4_0, MainVS()));
		//SetPixelShader(CompileShader(ps_4_0, MainPS()));
	//}
//}

technique11 WithoutAlphaBlending {
	pass p0 {
		SetRasterizerState(gRS_NoCulling);	
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}