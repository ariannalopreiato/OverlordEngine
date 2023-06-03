//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;  
SamplerState samLinear : register(s0);

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
    CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};


// VERTEX SHADER
// -------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    // Set the Position
    output.Position = float4(input.Position, 1.0f);

    // Set the TexCoord
    output.TexCoord = input.TexCoord;

    return output;
}


// PIXEL SHADER
// ------------
float4 PS(PS_INPUT input) : SV_Target
{
    // Calculate the brightness of the pixel

    // Check if the pixel is bright enough to contribute to the bloom effect
    float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);

    // BLOOM
    float4 sceneColor = gTexture.Sample(samPoint, input.TexCoord);

    // Calculate the brightness of the pixel
    float brightness = dot(sceneColor.rgb, float3(0.2126, 0.7152, 0.0722));

    // Check if the pixel is bright enough to contribute to the bloom effect
    float bloomThreshold = 0.15f; // Adjust to control the amount of bloom
    float4 bloomColor = float4(saturate((brightness - bloomThreshold) / (1.0f - bloomThreshold)), 0.0f, 0.0f, 1.0f);

    // Combine the scene color and bloom color
    finalColor = sceneColor + bloomColor;

    // Output the final color
    return finalColor;
}


// TECHNIQUE
// ---------
technique11 Bloom
{
    pass P0
    {
        // Set states...
        SetRasterizerState(BackCulling);
        SetDepthStencilState(EnableDepth, 0);
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}