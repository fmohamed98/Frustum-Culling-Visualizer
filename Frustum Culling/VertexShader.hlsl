cbuffer CBuf
{
	matrix transform;	
};

//new
struct VSOut
{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

//new
VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f), transform);
	vso.tex = tex;
	return vso;
}

//float4 main(float3 pos : Position) : SV_Position
//{
//	return  mul(float4(pos, 1.0f),transform);
//}