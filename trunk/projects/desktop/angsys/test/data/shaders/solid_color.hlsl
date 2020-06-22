

struct ps_input
{
	float4 position : SV_Position;
	float4 color : COLOR;
    uint ViewId : SV_RenderTargetArrayIndex;
};

struct vs_input
{
    float4 position : POSITION;
    float4 color : COLOR;
    uint InstId : SV_InstanceID;
};

cbuffer wvp
{
	matrix wm;
    matrix vpm[2];
};

ps_input vs(vs_input input)
{
	ps_input output;
    const int idx = input.InstId % 2;
    output.ViewId = idx;
    output.position = mul(mul(float4(input.position.xyz, 1), wm), vpm[idx]);
    output.color = input.color;
	return output;
}

float4 ps(ps_input input) : SV_Target
{ 
    return float4(1,0,1, 1.0);
}