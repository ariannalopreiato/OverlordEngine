//DX10 - FLAT SHADER
//Digital Arts & Entertainment

float4x4 m_MatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 m_MatrixWorld : WORLD;
float3 m_LightDirection : DIRECTION
<
	string Object = "TargetLight";
> = float3(0.577f, -0.577f, 0.577f);

float3 gColorDiffuse : COLOR = float3(1, 1, 1);

float gSpikeLength
<
	string UIWidget = "Slider";
	float UIMin = 0.0f;
	float UIMax = 0.5f;
	float UIStep = 0.0001f;
> = 0.2f;

RasterizerState FrontCulling
{
	CullMode = FRONT;
};

//**********
// STRUCTS *
//**********
struct VS_DATA
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

//****************
// VERTEX SHADER *
//****************
VS_DATA MainVS(VS_DATA vsData)
{
	//Step 1.
	//Delete this transformation code and just return the VS_DATA parameter (vsData)
	//Don't forget to change the return type!
	
	return vsData;
}

//******************
// GEOMETRY SHADER *
//******************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal, float2 texCoord)
{
	//Step 1. Create a GS_DATA object
	GS_DATA object;

	//Step 2. Transform the position using the WVP Matrix and assign it to (GS_DATA object).Position (Keep in mind: float3 -> float4)
	object.Position = mul(float4(pos, 1.0f), m_MatrixWorldViewProj);

	//Step 3. Transform the normal using the World Matrix and assign it to (GS_DATA object).Normal (Only Rotation, No translation!)
	object.Normal = mul(normal, (float3x3)m_MatrixWorld);

	//Step 4. Append (GS_DATA object) to the TriangleStream parameter (TriangleStream::Append(...))
	triStream.Append(object);
}

[maxvertexcount(9)]
void SpikeGenerator(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
	//Use these variable names
	float3 centerPoint, top, left, right, spikeNormal;

	left = vertices[0].Position;
	top = vertices[1].Position;
	right = vertices[2].Position;

	//Step 1. Calculate CENTER_POINT
	centerPoint = (vertices[0].Position + vertices[1].Position + vertices[2].Position) / 3;

	//Step 2. Calculate Face Normal (Original Triangle)
	float3 edge0 = top - left;
	float3 edge1 = right - left;
	spikeNormal = normalize(cross(edge1, edge0));

	//Step 3. Offset CENTER_POINT (use gSpikeLength)
	centerPoint += gSpikeLength * spikeNormal;

	//Step 4 + 5. Calculate Individual Face Normals (Cross Product of Face Edges) & Create Vertices for every face

	//FACE 1
	edge0 = centerPoint - top;
	edge1 = top - left;
	float3 faceNormalOne = normalize(cross(edge0, edge1));

	CreateVertex(triStream, top, faceNormalOne, (float2)0);
	CreateVertex(triStream, centerPoint, faceNormalOne, (float2)0);
	CreateVertex(triStream, left, faceNormalOne, (float2)0);

	//Restart Strip! >> We want to start a new triangle (= interrupt trianglestrip)
	triStream.RestartStrip();

	//FACE 2
	edge0 = centerPoint - right; 
	edge1 = right - top;
	float3 faceNormalTwo = normalize(cross(edge0, edge1));

	CreateVertex(triStream, right, faceNormalTwo, (float2)0);
	CreateVertex(triStream, centerPoint, faceNormalTwo, (float2)0);
	CreateVertex(triStream, top, faceNormalTwo, (float2)0);

	triStream.RestartStrip();

	//Face 3
	edge0 = centerPoint - left;
	edge1 = left - right;
	float3 faceNormalThree = normalize(cross(edge0, edge1));

	CreateVertex(triStream, left, faceNormalThree, (float2)0);
	CreateVertex(triStream, centerPoint, faceNormalThree, (float2)0);
	CreateVertex(triStream, right, faceNormalThree, (float2)0);

	//Step 6. Complete code in CreateVertex(...)
	//Step 7. Bind this Geometry Shader function to the effect pass (See Technique Struct)
}

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET
{
	input.Normal = normalize(input.Normal);
	float diffuseStrength = max(dot(normalize(m_LightDirection),-input.Normal),0.2f);
	return float4(gColorDiffuse * diffuseStrength,1.0f);
}

//*************
// TECHNIQUES *
//*************
technique11 Default //FXComposer >> Rename to "technique10 Default"
{
	pass p0
	{
		SetRasterizerState(FrontCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, SpikeGenerator()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}