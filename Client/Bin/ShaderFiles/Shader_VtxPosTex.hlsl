

// float, float2, float3, float4 == vector
//float3		vTmp = (float3)0;
//
//float2		vTmp1 = vTmp.xy;
//
//vTmp.xz = 1.f;
//
//vTmp.rgb = 1.f;

// float1x4, float3x3, float4x4 == matrix


/* ������̺� ConstantTable */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;

/* D3D11_SAMPLER_DESC */
sampler			LinearSampler = sampler_state 
{	
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

/* VertexShader */

/* �������̴��� ���� ����. */
/* m_pContext->DrawIndexed() */
/* �׸��� ����ϰ����ߴ� ��������, �ε�������(0, 1, 2, 0, 2, 3)�� �������ְ� �ִ� ������ ��� */
/* VS_MAIN�Լ��� ȣ���ϸ� ���ڷ� �������ش�. */

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;	
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

/* 1. ������ġ�� ���� �������� ��ȯ������ �����Ѵ�.(���庯ȯ, �亯ȯ, ) */

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;

	return Out;
}



/* ������ ��ġ���͸� ��ġ������ w�� ������. */
/* ����Ʈ �������� �������� ��ȯ�Ѵ�. */
/* �����Ͷ�����(���� ������ �����Ͽ� �ȼ��� ������ �����ȴ�.) */
/* �̷��� �ȼ��� PS_MAIN() ȣ���ϸ鼭 �������ش�. */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	/*Out.vColor = g_Texture.Sample(��Ծ��÷�?, In.vTexcoord);*/
	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}


technique11			DefaultTechnique
{
	/* �츮�� �����ؾ��� ����, �ȼ� ���̴��� ������ �Լ��� �����Ѵ�. */
	pass DefaultPass
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}


}
