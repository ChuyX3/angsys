
//#define VERTEX_SHADER
#define PIXEL_SHADER
//#define LIGHTING
//#define USE_NORMAL_MAP

//#undef LIGHTING

struct ps_input
{
    float4 vposition : SV_POSITION;
    float4 vnormal : NORMAL0;
#ifdef USE_NORMAL_MAP
    float4 vbinormal : BINORMAL0;
    float4 vtangent : TANGET0;
#endif
    float2 vtexcoord : TEXCOORD0;
#ifdef LIGHTING
    float4 vpos_world : TEXCOORD1;
#endif
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
    float4 aposition : POSITION;
    float4 anormal : NORMAL0;
#ifdef USE_NORMAL_MAP
    float4 abinormal : BINORMAL0;
    float4 atangent : TANGENT0;
#endif
    float2 atexcoord : TEXCOORD0;
};

ps_input main_vs(vs_input input)
{
    ps_input output;
    output.vposition =mul(mul(mul(float4(input.aposition.xyz, 1), world), view), proj);
    output.vnormal = mul(float4(input.anormal.xyz ,0), world);
#ifdef USE_NORMAL_MAP
    output.vbinormal = mul(float4(input.abinormal.xyz, 0), world);
     output.vtangent = mul(float4(input.atangent.xyz, 0), world);
#endif
    output.vtexcoord = input.atexcoord;
#ifdef LIGHTING
    output.vpos_world = mul(float4(input.aposition.xyz, 1), world);
#endif
    return output;
}

#endif
#ifdef PIXEL_SHADER

#ifdef LIGHTING
cbuffer light_data : register(b0)
{
	float4 ambient_color : packoffset(c0);
	uint light_count : packoffset(c1.x);
	float specular_power : packoffset(c1.y);
	struct {
		float4 color;
		float3 position;
		uint type;
	}light_data[10] : packoffset(c2);
}
#endif

Texture2D difusse_tex : register(t0);
SamplerState sam_linear : register(s0);

#ifdef LIGHTING

float3 get_light_direction(float3 world_pos, float3 dir, uint type)
{
    if(type == 0)
        return normalize(dir);
    else
        return normalize(world_pos - dir);
}

float3 get_light_difusse(float3 normal, float3 world_pos)
{
	float3 difusse = { 0,0,0 };
	for (uint i = 0; i < light_count; ++i)
	{
		float3 light_dir = get_light_direction(world_pos, light_data[i].position, light_data[i].type);
        float3 reflection = reflect(light_dir, normal);
        difusse += light_data[i].color.xyz * max(dot(reflection, normal), 0.0);
    }
	return difusse;
}

float3 get_light_specular(float3 normal, float3 world_pos)
{
	float3 total = { 0,0,0 };
	for (uint i = 0; i < light_count; ++i)
	{
        float3 light_dir = get_light_direction(world_pos, light_data[i].position, light_data[i].type);
		float3 specular = light_data[i].color.xyz;
		float3 reflection = reflect(light_dir, normal);
		total += specular * pow(max(dot(reflection, normal), 0.0), specular_power);
	}
	return total;
}
#endif

float4 main_ps(ps_input input ) : SV_Target
{
    float3 normal = normalize(input.vnormal).xyz;
#ifdef LIGHTING
    float3 difusse = get_light_difusse(normal, input.vpos_world.xyz);
    difusse = difusse / light_count;
#endif

    float4 color = difusse_tex.Sample(sam_linear, input.vtexcoord);
   //color.xyz = float3(.5,.5,.5);
   // color.xyz = lerp(color.xyz, float3(1,1,1), .5) * 1.5;
#ifdef LIGHTING
    float3 total = (ambient_color.xyz * ambient_color.w + difusse * (1.0 - ambient_color.w)) * color.xyz;
    if (specular_power > 0)
        total = total + get_light_specular(normal, input.vpos_world.xyz);
    color.xyz = total;
#endif
   // color.xyz = lerp(color.xyz, float3(normalize(input.vtexcoord)/2, normal.z/2), 0.3);
    float alpha = lerp(color.a, 1, 0.5);
    return float4(color.xyz, alpha);
}

#endif
