
//#define VERTEX_SHADER
//#define PIXEL_SHADER

struct ps_input
{
    float4 vposition : SV_POSITION;
    float3 vtexcoord : TEXCOORD0;
    uint ViewId : SV_RenderTargetArrayIndex;
};

#ifdef VERTEX_SHADER

cbuffer ConstantBuffer : register(b0)
{
    matrix wm;
    matrix vpm[2];
}

struct vs_input
{
    float4 position : POSITION;
    uint InstId : SV_InstanceID;
};

ps_input vs(vs_input input)
{
    ps_input output;
    const int idx = input.InstId % 2;
    output.ViewId = idx;
    output.vposition = mul(mul(float4(input.position.xyz, 1), wm), vpm[idx]);
    output.vtexcoord = input.position.xyz;
    return output;
}

#endif
#ifdef PIXEL_SHADER

TextureCube skybox_tex : register(t0);
SamplerState sam_linear : register(s0);

float4 ps(ps_input input ) : SV_Target
{
    return float4(skybox_tex.Sample(sam_linear, input.vtexcoord).xyz, 1);
}

#endif
