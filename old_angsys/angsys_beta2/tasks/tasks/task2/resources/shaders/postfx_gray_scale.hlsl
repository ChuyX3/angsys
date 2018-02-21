
//#define VERTEX_SHADER
//#define PIXEL_SHADER

struct ps_input
{
    float4 vposition : SV_POSITION;
    float2 vtexcoord : TEXCOORD0;
};

#ifdef VERTEX_SHADER

cbuffer ConstantBuffer : register(b0)
{
    matrix world;
   // matrix view;
  //  matrix proj;
}

ps_input main_vs(float4 aposition : POSITION)
{
    ps_input output;
    output.vposition = mul(float4(aposition.xyz, 1), world); // mul(mul(mul(float4(aposition.xyz, 1), world), view), proj);
    output.vtexcoord = aposition.xy * + float2(0.5, -0.5) + float2(0.5, 0.5);
    return output;
}

#endif
#ifdef PIXEL_SHADER

Texture2D difusse_tex : register(t0);
SamplerState sam_linear : register(s0);

float4 main_ps(ps_input input ) : SV_Target
{
    float4 color = difusse_tex.Sample(sam_linear, input.vtexcoord);
    color.xyz = (color.x + color.y + color.z) / 3.0;
    return color;
}

#endif
