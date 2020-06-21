
//#define VERTEX_SHADER
//#define PIXEL_SHADER

struct ps_input
{
    float4 vposition : SV_POSITION;
    float4 vcolor : COLOR;
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
    float4 color : COLOR;
};

ps_input main_vs(vs_input input)
{
    ps_input output;
    // matrix wvp = proj * (view * world );
    output.vposition = mul(mul(mul(float4(input.position.xyz, 1), world), view), proj);
    output.vcolor = float4(input.color.xyz, 1);
    return output;
}

#endif
#ifdef PIXEL_SHADER


float4 main_ps(ps_input input ) : SV_Target
{
    return input.vcolor; 
}

#endif
