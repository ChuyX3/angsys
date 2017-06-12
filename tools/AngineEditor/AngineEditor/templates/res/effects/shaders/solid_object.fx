#ifdef HLSL
#if defined VSHADER
cbuffer wvpMatrix : register(b0)
{
	matrix uWorldMatrix;
	matrix uViewMatrix;
	matrix uProjMatrix;
};

struct vs_input
{
	float4 position : POSITION;
	float4 color : COLOR;
};
#endif

typedef struct fs_input
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
}vs_output;


#if defined VSHADER

vs_output main(vs_input input)
{
	vs_output output;
	output.color = input.color;
	output.position = mul(input.position, uWorldMatrix);
	output.position = mul(output.position, uViewMatrix);
	output.position = mul(output.position, uProjMatrix);
	return output;
}

#elif defined PSHADER

float4 main(fs_input input) : SV_Target
{
	return input.color;
}

#endif
#elif defined GLSL
#if defined VSHADER

uniform mat4 uWorldMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

attribute vec4 position;
attribute vec4 color;
#endif

//varying vec4 position;
varying vec4 vColor;

#if defined VSHADER

void main()
{
	vColor = color;
	vec4 pos = uWorldMatrix * position;
	pos = uViewMatrix * pos;
	pos = uProjMatrix * pos;
	gl_Position = pos;
}

#elif defined PSHADER

void main()
{
	gl_FragColor = vColor;
}

#endif
#endif//HLSL