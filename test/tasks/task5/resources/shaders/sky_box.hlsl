
#define VERTEX_SHADER
#define PIXEL_SHADER

struct ps_input
{
    float4 vposition : SV_POSITION;
    float3 vtexcoord : TEXCOORD0;
};

#ifdef VERTEX_SHADER

cbuffer ConstantBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
}

struct vs_input
{
    float4 position : POSITION;
};

ps_input main_vs(vs_input input)
{
    ps_input output;
    output.vposition = mul(mul(mul(float4(input.position.xyz, 1), world), view), proj);
    output.vtexcoord = input.position.xyz;
    return output;
}

#endif
#ifdef PIXEL_SHADER

TextureCube skybox_tex : register(t0);
SamplerState sam_linear : register(s0);

float4 main_ps(ps_input input ) : SV_Target
{
    return float4(skybox_tex.Sample(sam_linear, input.vtexcoord).xyz, 1);
}

#endif
