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
    float4 sceneColor = gTexture.Sample(samPoint, input.TexCoord);

    // Calculate the brightness of the pixel
    float3 brightness = dot(sceneColor.rgb, float3(0.2126, 0.7152, 0.0722));

    // Check if the pixel is bright enough to contribute to the bloom effect
    float bloomThreshold = 0.5f; // Adjust this threshold to control the amount of bloom
    float4 bloomColor = float4(saturate((brightness - bloomThreshold) / (1.0f - bloomThreshold)), 1.0f);

    // Store the bright pixels in the brightTexture
    float4 finalColor = sceneColor + bloomColor;

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