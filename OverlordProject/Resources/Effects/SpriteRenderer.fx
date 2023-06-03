float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;
bool gIsTransparent;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
    DepthEnable = FALSE;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
    uint TextureId : TEXCOORD0;
    float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
    float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
    //Step 1.
    //Quad Creation
    //Position, Scale & TexCoord

    //Step 2.

    //Step 3.
    //Do rotation calculations
    if (rotation == 0)
    {
        //Apply pivot offset  
        pos.x += pivotOffset.x;
        pos.y += pivotOffset.y;
    }
    if (rotation != 0)
    {
        //Transform to origin
        pos.x -= offset.x;
        pos.y -= offset.y;

        pos.x += pivotOffset.x;
        pos.y += pivotOffset.y;

        //Rotate
        float3 startPos = pos;
        pos.x = (startPos.x * rotCosSin.x) - (startPos.y * rotCosSin.y);
        pos.y = (startPos.y * rotCosSin.x) + (startPos.x * rotCosSin.y);

        //Retransform to initial position
        pos.x += offset.x;
        pos.y += offset.y;
    }

    //Geometry Vertex Output
    GS_DATA geomData = (GS_DATA)0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
    //Given Data (Vertex Data)
    float3 position = { vertex[0].TransformData.xyz }; //Extract the position data from the VS_DATA vertex struct
    float2 offset = { vertex[0].TransformData.xy }; //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
    float rotation = vertex[0].TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
    float2 pivot = { vertex[0].TransformData2.xy }; //Extract the pivot data from the VS_DATA vertex struct
    float2 scale = { vertex[0].TransformData2.zw }; //Extract the scale data from the VS_DATA vertex struct
    float2 texCoord = vertex[0].TextureId; //Initial Texture Coordinate
    float4 color = vertex[0].Color;

    pivot.x = -(pivot.x * gTextureSize.x * scale.x) + ((gTextureSize.x / 2) * scale.x);
    pivot.y = -(pivot.y * gTextureSize.y * scale.y) + ((gTextureSize.y / 2) * scale.y);

    // LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
    // |          / |
    // |       /    |
    // |    /       |
    // | /          |
    // LB----------RB

    float2 rotCosSin;

    if (rotation == 0)
    {
        rotCosSin = float2(0, 0);
    }
    if (rotation != 0)
    {
        rotCosSin = float2(cos(rotation), sin(rotation));
    }

    //VERTEX 1 [LT]
    float3 vertexOnePos = { position.x - ((gTextureSize.x / 2) * scale.x), position.y + ((gTextureSize.y / 2) * scale.y), 0 };
    CreateVertex(triStream, vertexOnePos, color, float2(0, 1), rotation, rotCosSin, offset, pivot);

    //VERTEX 3 [LB]
    float3 vertexThreePos = { position.x - ((gTextureSize.x / 2) * scale.x), position.y - ((gTextureSize.y / 2) * scale.y), 0 };
    CreateVertex(triStream, vertexThreePos, color, float2(0, 0), rotation, rotCosSin, offset, pivot);

    //VERTEX 2 [RT]
    float3 vertexTwoPos = { position.x + ((gTextureSize.x / 2) * scale.x), position.y + ((gTextureSize.y / 2) * scale.y), 0 };
    CreateVertex(triStream, vertexTwoPos, color, float2(1, 1), rotation, rotCosSin, offset, pivot);

    //VERTEX 4 [RB]
    float3 vertexFourPos = { position.x + ((gTextureSize.x / 2) * scale.x), position.y - ((gTextureSize.y / 2) * scale.y), 0 };
    CreateVertex(triStream, vertexFourPos, color, float2(1, 0), rotation, rotCosSin, offset, pivot);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    float4 output = gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;

    if (output.a < 1.f && gIsTransparent)
        discard;

    return output;
}

// Default Technique
technique11 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        //SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
