//#define VERTEX_SHADER
//#define PIXEL_SHADER
//#define LIGHTING
//#define ILUM 0
//#define NORMAL_MAP

struct ps_input
{
    float4 position : SV_Position;
#ifdef LIGHTING
    float4 eyepos : TEXCOORD1;
    float4 normal : NORMAL0;
#ifdef NORMAL_MAP
    float4 binormal : BINORMAL0;
    float4 tangent : TANGENT0;
#endif
#endif
    float2 texcoord : TEXCOORD0;
    uint ViewId : SV_RenderTargetArrayIndex;
};

#ifdef VERTEX_SHADER

struct vs_input
{
    float4 position : POSITION;
#ifdef LIGHTING
    float4 normal : NORMAL0;
    #ifdef NORMAL_MAP
    float4 binormal : BINORMAL0;
    float4 tangent : TANGENT0;
    #endif
#endif
    float2 texcoord : TEXCOORD0;
    uint InstId : SV_InstanceID;
};

cbuffer wvp : register(b0)
{
    matrix wm;
    matrix vpm[2];
};

ps_input vs(vs_input input)
{
    ps_input output = (ps_input)0;
    const int idx = input.InstId % 2;
    output.ViewId = idx;
    output.position = mul(mul(float4(input.position.xyz, 1), wm), vpm[idx]);
#ifdef LIGHTING
    output.eyepos = mul(float4(input.position.xyz, 1), wm);
    output.normal = mul(float4(input.normal.xyz, 0), wm);
#endif
    output.texcoord = input.texcoord;
	return output;
}

#endif

#ifdef PIXEL_SHADER

Texture2D map_Kd : register(t0);
SamplerState sam_linear : register(s0);

cbuffer material : register(b0)
{
    float3 Ka : packoffset(c0);
    float3 Kd : packoffset(c1);
    float d : packoffset(c1.w);
    float3 Ks : packoffset(c2);
    float Ns : packoffset(c2.w);
//  uint ilum : packoffset(c3);
}

#ifdef LIGHTING
cbuffer light_data : register(b1)
{
	float3 ambient_color : packoffset(c0);
	uint light_count : packoffset(c0.w);
	struct {
		float4 color;
		float3 position;
		uint type;
	}light_data[10] : packoffset(c1);
}
#endif

float4 ps(ps_input input ) : SV_Target
{
    float4 total;
    float4 difuse = map_Kd.Sample(sam_linear, input.texcoord);
#ifdef LIGHTING
    total = float4(difuse.xyz * normalize(Kd), difuse.w * d);

    total = difuse;
#else
    total = float4(difuse.xyz* normalize(Kd), difuse.w* d);
#endif
   
#ifdef NO_ALPHA
    return float4(total.xyz, 1);  
#else
    return total;
#endif//NO_ALPHA

}

#endif

